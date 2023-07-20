#include "gp_proc.h"

#include <format>
#include <iostream>
#include <vector>

//-------------------------------------------------------------------------------------------------

static void report_string_error()
{
    DWORD dwErrorCode = GetLastError();

    std::cerr << std::endl;
    std::cerr << std::format("ERROR CODE: 0x{0:08x}", dwErrorCode);
    std::cerr << std::endl;

    std::exit(1);
}

static std::wstring convert_string_to_wstring(const std::string& s)
{
    UINT codePage = CP_UTF8;
    DWORD dwFlags = MB_ERR_INVALID_CHARS;

    LPCCH lpStrS = s.c_str();
    int cbStrS = (int)s.size();
    int ccStrW = MultiByteToWideChar(codePage, dwFlags, lpStrS, cbStrS, NULL, 0);
    if (ccStrW == 0)
    {
        report_string_error();
    }

    std::vector<wchar_t> buffer(ccStrW);

    LPWSTR lpStrW = buffer.data();
    int result = MultiByteToWideChar(codePage, dwFlags, lpStrS, cbStrS, lpStrW, ccStrW);
    if (result == 0)
    {
        report_string_error();
    }

    return std::wstring(buffer.begin(), buffer.end());
}

static std::string convert_wstring_to_string(const std::wstring& w)
{
    UINT codePage = CP_UTF8;
    DWORD dwFlags = WC_ERR_INVALID_CHARS;

    LPCWCH lpStrW = w.c_str();
    int ccStrW = (int)w.size();
    int cbStrS = WideCharToMultiByte(codePage, dwFlags, lpStrW, ccStrW, NULL, 0, NULL, NULL);
    if (cbStrS == 0)
    {
        report_string_error();
    }

    std::vector<char> buffer(cbStrS);

    LPSTR lpStrS = buffer.data();
    int result = WideCharToMultiByte(codePage, dwFlags, lpStrW, ccStrW, lpStrS, cbStrS, NULL, NULL);
    if (result == 0)
    {
        report_string_error();
    }

    return std::string(buffer.begin(), buffer.end());
}

//-------------------------------------------------------------------------------------------------

static void report_result_error()
{
    DWORD dwErrorCode = GetLastError();

    std::cerr << std::endl;
    std::cerr << std::format("ERROR CODE: 0x{0:08x}", dwErrorCode);

    LPWSTR lpErrorMessage = NULL;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwErrorCode,
        LANG_USER_DEFAULT,
        (LPWSTR)&lpErrorMessage,
        0,
        NULL
    );

    std::cerr << std::endl;
    std::cerr << std::format("ERROR MESSAGE: {0}", convert_wstring_to_string(lpErrorMessage));

    LocalFree(lpErrorMessage);

    std::exit(1);
}

static void result_hstd(HANDLE result)
{
    if (result == INVALID_HANDLE_VALUE)
    {
        report_result_error();
    }
}

static void result_hjob(HANDLE result)
{
    if (result == NULL)
    {
        report_result_error();
    }
}

static void result_bool(BOOL result)
{
    if (result == FALSE)
    {
        report_result_error();
    }
}

static void result_fail(DWORD result)
{
    if (result == (DWORD)-1)
    {
        report_result_error();
    }
}

static void result_wait(DWORD result)
{
    if (result == WAIT_FAILED)
    {
        report_result_error();
    }
    else if (result == WAIT_ABANDONED)
    {
        std::cerr << std::endl;
        std::cerr << std::format("UNEXPECTED: 0x{0:08x} {1}", result, "WAIT_ABANDONED");
        std::cerr << std::endl;
    }
    else if (result == WAIT_TIMEOUT)
    {
        std::cerr << std::endl;
        std::cerr << std::format("UNEXPECTED: 0x{0:08x} {1}", result, "WAIT_TIMEOUT");
        std::cerr << std::endl;
    }
    else if (result != WAIT_OBJECT_0)
    {
        std::cerr << std::endl;
        std::cerr << std::format("UNEXPECTED: 0x{0:08x}", result);
        std::cerr << std::endl;
    }
}

//-------------------------------------------------------------------------------------------------

gp::proc::proc(const std::string& gnuplot_exe_path)
{
    HANDLE hJob = CreateJobObject(NULL, NULL);
    result_hjob(hJob);

    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobInfo = {};
    jobInfo.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

    result_bool(SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &jobInfo, sizeof(jobInfo)));

    SECURITY_ATTRIBUTES securityAttributes = {};
    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securityAttributes.bInheritHandle = TRUE;
    securityAttributes.lpSecurityDescriptor = NULL;

    result_bool(CreatePipe(&hPipeStdInR, &hPipeStdInW, &securityAttributes, 0));

    HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    result_hstd(hStdOutput);

    HANDLE hStdError = GetStdHandle(STD_ERROR_HANDLE);
    result_hstd(hStdError);

    STARTUPINFO startupInfo = {};
    startupInfo.cb = sizeof(STARTUPINFO);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = hPipeStdInR;
    startupInfo.hStdOutput = hStdOutput;
    startupInfo.hStdError = hStdError;

    result_bool(CreateProcess(
        convert_string_to_wstring(gnuplot_exe_path).c_str(),
        NULL,
        NULL,
        NULL,
        TRUE,
        CREATE_SUSPENDED,
        NULL,
        NULL,
        &startupInfo,
        &processInformation
    ));

    result_bool(AssignProcessToJobObject(hJob, processInformation.hProcess));
    result_fail(ResumeThread(processInformation.hThread));
}

void gp::proc::write(const std::string& plot)
{
    result_bool(WriteFile(hPipeStdInW, plot.c_str(), (DWORD)plot.length(), NULL, NULL));
    result_bool(FlushFileBuffers(hPipeStdInW));
}

void gp::proc::exit_wait()
{
    write("exit\n");
    result_wait(WaitForSingleObject(processInformation.hProcess, INFINITE));
}

gp::proc::~proc()
{
    result_bool(CloseHandle(processInformation.hProcess));
    result_bool(CloseHandle(processInformation.hThread));
    result_bool(CloseHandle(hPipeStdInR));
    result_bool(CloseHandle(hPipeStdInW));
}

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

    LPCCH lpMultiByteStr = s.c_str();
    int cbMultiByte = (int)s.size();
    int cchWideChar = MultiByteToWideChar(codePage, dwFlags, lpMultiByteStr, cbMultiByte, NULL, 0);
    if (cchWideChar == 0)
    {
        report_string_error();
    }

    std::vector<wchar_t> buffer(cchWideChar);

    LPWSTR lpWideCharStr = buffer.data();
    int returnValue = MultiByteToWideChar(codePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
    if (returnValue == 0)
    {
        report_string_error();
    }

    return std::wstring(buffer.begin(), buffer.end());
}

static std::string convert_wstring_to_string(const std::wstring& w)
{
    UINT codePage = CP_UTF8;
    DWORD dwFlags = WC_ERR_INVALID_CHARS;

    LPCWCH lpWideCharStr = w.c_str();
    int cchWideChar = (int)w.size();
    int cbMultiByte = WideCharToMultiByte(codePage, dwFlags, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL);
    if (cbMultiByte == 0)
    {
        report_string_error();
    }

    std::vector<char> buffer(cbMultiByte);

    LPSTR lpMultiByteStr = buffer.data();
    int returnValue = WideCharToMultiByte(codePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, NULL, NULL);
    if (returnValue == 0)
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

static void handle_bool(BOOL result)
{
    if (result == FALSE)
    {
        report_result_error();
    }
}

static void handle_wait(DWORD result)
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
    SECURITY_ATTRIBUTES securityAttributes = {};
    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securityAttributes.bInheritHandle = TRUE;
    securityAttributes.lpSecurityDescriptor = NULL;

    handle_bool(CreatePipe(&hPipeStdInR, &hPipeStdInW, &securityAttributes, 0));

    STARTUPINFO startupInfo = {};
    startupInfo.cb = sizeof(STARTUPINFO);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = hPipeStdInR;

    handle_bool(CreateProcess(
        convert_string_to_wstring(gnuplot_exe_path).c_str(),
        NULL,
        NULL,
        NULL,
        TRUE,
        NORMAL_PRIORITY_CLASS,
        NULL,
        NULL,
        &startupInfo,
        &processInformation
    ));
}

void gp::proc::write(const std::string& plot)
{
    handle_bool(WriteFile(hPipeStdInW, plot.c_str(), (DWORD)plot.length(), NULL, NULL));
    handle_bool(FlushFileBuffers(hPipeStdInW));
}

void gp::proc::exit_wait()
{
    write("exit\n");
    handle_wait(WaitForSingleObject(processInformation.hProcess, INFINITE));
}

gp::proc::~proc()
{
    handle_bool(CloseHandle(processInformation.hProcess));
    handle_bool(CloseHandle(processInformation.hThread));
    handle_bool(CloseHandle(hPipeStdInR));
    handle_bool(CloseHandle(hPipeStdInW));
}

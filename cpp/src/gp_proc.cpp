#include "gp_proc.h"

#include <format>
#include <iostream>

//-------------------------------------------------------------------------------------------------

static void report_error()
{
    DWORD dwLastErrorCode = GetLastError();
    LPSTR lpBufferMessage = NULL;

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwLastErrorCode,
        LANG_USER_DEFAULT,
        (LPSTR)&lpBufferMessage,
        0,
        NULL
    );

    std::cerr << std::endl;
    std::cerr << std::format("ERROR 0x{0:08x}: {1}", dwLastErrorCode, lpBufferMessage);

    LocalFree(lpBufferMessage);

    std::exit(1);
}

static void handle_bool(BOOL result)
{
    if (result == FALSE)
    {
        report_error();
    }
}

static void handle_wait(DWORD result)
{
    if (result == WAIT_ABANDONED)
    {
        std::cerr << std::endl;
        std::cerr << std::format("UNEXPECTED 0x{0:08x}: {1}", result, "WAIT_ABANDONED");
        std::cerr << std::endl;
    }

    if (result == WAIT_TIMEOUT)
    {
        std::cerr << std::endl;
        std::cerr << std::format("UNEXPECTED 0x{0:08x}: {1}", result, "WAIT_TIMEOUT");
        std::cerr << std::endl;
    }

    if (result == WAIT_FAILED)
    {
        report_error();
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

    STARTUPINFOA startupInfo = {};
    startupInfo.cb = sizeof(STARTUPINFOA);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = hPipeStdInR;

    handle_bool(CreateProcessA(
        gnuplot_exe_path.c_str(),
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

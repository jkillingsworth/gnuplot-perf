#include "gp_proc.h"

#include <format>
#include <iostream>

//-------------------------------------------------------------------------------------------------

static void handle_error(const auto)
{
    DWORD dwLastErrorCode = GetLastError();
    LPSTR lpBufferMessage = NULL;

    if (dwLastErrorCode == ERROR_SUCCESS)
    {
        return;
    }

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

//-------------------------------------------------------------------------------------------------

gp::proc::proc(const std::string& gnuplot_exe_path)
{
    SECURITY_ATTRIBUTES securityAttributes = {};
    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securityAttributes.bInheritHandle = TRUE;
    securityAttributes.lpSecurityDescriptor = NULL;

    handle_error(CreatePipe(&hPipeStdInR, &hPipeStdInW, &securityAttributes, 0));

    STARTUPINFOA startupInfo = {};
    startupInfo.cb = sizeof(STARTUPINFOA);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = hPipeStdInR;

    handle_error(CreateProcessA(
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
    handle_error(WriteFile(hPipeStdInW, plot.c_str(), (DWORD)plot.length(), NULL, NULL));
    handle_error(FlushFileBuffers(hPipeStdInW));
}

void gp::proc::exit_wait()
{
    write("exit\n");
    handle_error(WaitForSingleObject(processInformation.hProcess, INFINITE));
}

gp::proc::~proc()
{
    handle_error(CloseHandle(processInformation.hProcess));
    handle_error(CloseHandle(processInformation.hThread));
    handle_error(CloseHandle(hPipeStdInR));
    handle_error(CloseHandle(hPipeStdInW));
}

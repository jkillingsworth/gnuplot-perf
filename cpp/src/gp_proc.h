#pragma once

#include <windows.h>
#include <string>

//-------------------------------------------------------------------------------------------------

namespace gp
{
    class proc final
    {
    public:
        proc(const std::string& gnuplot_exe_path);
        void write(const std::string& plot);
        void exit_wait();
        ~proc();

    private:
        PROCESS_INFORMATION processInformation = {};
        HANDLE hPipeStdInR = NULL;
        HANDLE hPipeStdInW = NULL;
        HANDLE hJob = NULL;
    };
}

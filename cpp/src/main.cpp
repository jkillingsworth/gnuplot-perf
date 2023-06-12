#include <chrono>
#include <execution>
#include <filesystem>
#include <fstream>
#include <format>
#include <iostream>
#include <windows.h>
#include "chart.h"

//-------------------------------------------------------------------------------------------------

constexpr auto n = 100;
constexpr auto gnuplot_exe_path_542 = "C:\\gnuplot542\\bin\\gnuplot.exe";
constexpr auto gnuplot_exe_path_546 = "C:\\gnuplot546\\bin\\gnuplot.exe";

std::string plots[n];

//-------------------------------------------------------------------------------------------------

void handle_error(const auto)
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

void render(const std::string& gnuplot_exe_path, const std::string& plot)
{
    HANDLE hPipeStdInR = NULL;
    HANDLE hPipeStdInW = NULL;

    SECURITY_ATTRIBUTES securityAttributes = {};
    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securityAttributes.bInheritHandle = TRUE;
    securityAttributes.lpSecurityDescriptor = NULL;

    handle_error(CreatePipe(&hPipeStdInR, &hPipeStdInW, &securityAttributes, 0));

    STARTUPINFOA startupInfo = {};
    startupInfo.cb = sizeof(STARTUPINFOA);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = hPipeStdInR;

    PROCESS_INFORMATION processInformation = {};

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

    handle_error(WriteFile(hPipeStdInW, plot.c_str(), (DWORD)plot.length(), NULL, NULL));
    handle_error(FlushFileBuffers(hPipeStdInW));
    handle_error(WaitForSingleObject(processInformation.hProcess, INFINITE));
    handle_error(CloseHandle(processInformation.hProcess));
    handle_error(CloseHandle(processInformation.hThread));
    handle_error(CloseHandle(hPipeStdInR));
    handle_error(CloseHandle(hPipeStdInW));
}

//-------------------------------------------------------------------------------------------------

void run_542()
{
    std::cout << std::endl;
    std::cout << "Gnuplot version: 5.4.2" << std::endl;
    std::cout << "Execution mode: sequential" << std::endl;
    std::cout << "Running";
    auto time_start = std::chrono::high_resolution_clock::now();

    for (const auto& plot : plots)
    {
        std::cout << ".";
        render(gnuplot_exe_path_542, plot);
    }

    auto time_final = std::chrono::high_resolution_clock::now();
    auto time_total = std::chrono::duration<double>(time_final - time_start);
    std::cout << std::endl;
    std::cout << "Charts generated: " << n << std::endl;
    std::cout << "Elapsed time: " << time_total << std::endl;
}

void run_542_parallel()
{
    std::cout << std::endl;
    std::cout << "Gnuplot version: 5.4.2" << std::endl;
    std::cout << "Execution mode: parallel" << std::endl;
    std::cout << "Running";
    auto time_start = std::chrono::high_resolution_clock::now();

    auto action = [](const auto& plot) -> void
    {
        std::cout << ".";
        render(gnuplot_exe_path_542, plot);
    };

    std::for_each(std::execution::par, std::begin(plots), std::end(plots), action);

    auto time_final = std::chrono::high_resolution_clock::now();
    auto time_total = std::chrono::duration<double>(time_final - time_start);
    std::cout << std::endl;
    std::cout << "Charts generated: " << n << std::endl;
    std::cout << "Elapsed time: " << time_total << std::endl;
}

void run_546()
{
    std::cout << std::endl;
    std::cout << "Gnuplot version: 5.4.6" << std::endl;
    std::cout << "Execution mode: sequential" << std::endl;
    std::cout << "Running";
    auto time_start = std::chrono::high_resolution_clock::now();

    for (const auto& plot : plots)
    {
        std::cout << ".";
        render(gnuplot_exe_path_546, plot);
    }

    auto time_final = std::chrono::high_resolution_clock::now();
    auto time_total = std::chrono::duration<double>(time_final - time_start);
    std::cout << std::endl;
    std::cout << "Charts generated: " << n << std::endl;
    std::cout << "Elapsed time: " << time_total << std::endl;
}

void run_546_parallel()
{
    std::cout << std::endl;
    std::cout << "Gnuplot version: 5.4.6" << std::endl;
    std::cout << "Execution mode: parallel" << std::endl;
    std::cout << "Running";
    auto time_start = std::chrono::high_resolution_clock::now();

    auto action = [](const auto& plot) -> void
    {
        std::cout << ".";
        render(gnuplot_exe_path_546, plot);
    };

    std::for_each(std::execution::par, std::begin(plots), std::end(plots), action);

    auto time_final = std::chrono::high_resolution_clock::now();
    auto time_total = std::chrono::duration<double>(time_final - time_start);
    std::cout << std::endl;
    std::cout << "Charts generated: " << n << std::endl;
    std::cout << "Elapsed time: " << time_total << std::endl;
}

//-------------------------------------------------------------------------------------------------

int main()
{
    std::filesystem::create_directory("./output/");
    std::ifstream data("./data.csv");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(data, line))
    {
        lines.push_back(line);
    }

    for (auto i = 0; i < n; i++)
    {
        plots[i] = chart::create_plot(std::format("./output/chart-{0:02}.svg", i), lines, i);
    }

    run_542();
    run_542_parallel();
    run_546();
    run_546_parallel();

    return 0;
}

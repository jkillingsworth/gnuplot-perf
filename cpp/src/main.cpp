#include "gp_chart.h"
#include "gp_proc.h"

#include <chrono>
#include <execution>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

//-------------------------------------------------------------------------------------------------

constexpr auto n = 100;

constexpr auto gnuplot_exe_path_542 = "C:\\gnuplot542\\bin\\gnuplot.exe";
constexpr auto gnuplot_exe_path_548 = "C:\\gnuplot548\\bin\\gnuplot.exe";

constexpr auto enable_singular_proc_seq = true;
constexpr auto enable_multiple_proc_seq = false;
constexpr auto enable_multiple_proc_par = false;

//-------------------------------------------------------------------------------------------------

void run_singular_proc_seq(const std::string& gnuplot_exe_path, const std::vector<std::string>& plots)
{
    auto proc = gp::proc(gnuplot_exe_path);

    for (const auto& plot : plots)
    {
        std::cout << ".";
        proc.write(plot);
    }

    proc.exit_wait();
}

void run_multiple_proc_seq(const std::string& gnuplot_exe_path, const std::vector<std::string>& plots)
{
    for (const auto& plot : plots)
    {
        std::cout << ".";
        auto proc = gp::proc(gnuplot_exe_path);
        proc.write(plot);
        proc.exit_wait();
    }
}

void run_multiple_proc_par(const std::string& gnuplot_exe_path, const std::vector<std::string>& plots)
{
    auto action = [&](const auto& plot) -> void
    {
        std::cout << ".";
        auto proc = gp::proc(gnuplot_exe_path);
        proc.write(plot);
        proc.exit_wait();
    };

    std::for_each(std::execution::par, std::begin(plots), std::end(plots), action);
}

//-------------------------------------------------------------------------------------------------

struct run_args final
{
    const std::function<void(const std::string&, const std::vector<std::string>&)>& run;
    const std::string& mode;
};

void execute(const std::string& gnuplot_exe_path, const std::vector<std::string>& plots, run_args args)
{
    std::cout << std::endl;
    std::cout << "Gnuplot executable path: " << gnuplot_exe_path << std::endl;
    std::cout << "Execution mode for test: " << args.mode << std::endl;
    std::cout << "Running";
    auto time_start = std::chrono::high_resolution_clock::now();

    args.run(gnuplot_exe_path, plots);

    auto time_final = std::chrono::high_resolution_clock::now();
    auto time_total = std::chrono::duration<double>(time_final - time_start);
    std::cout << std::endl;
    std::cout << "Number of plots created: " << plots.size() << std::endl;
    std::cout << "Elapsed time in seconds: " << time_total.count() << std::endl;
}

//-------------------------------------------------------------------------------------------------

int main()
{
    std::filesystem::create_directory("./output/");
    std::ifstream data("./data.csv");
    std::vector<std::string> lines;
    std::vector<std::string> plots;
    std::string line;

    while (std::getline(data, line))
    {
        lines.push_back(line);
    }

    for (auto i = 0; i < n; i++)
    {
        auto path = std::format("./output/chart-{0:03}.svg", i);
        auto plot = gp::chart::create_plot(path, lines, i);
        plots.push_back(plot);
    }

    auto paths =
    {
        gnuplot_exe_path_542,
        gnuplot_exe_path_548
    };

    for (auto gnuplot_exe_path : paths)
    {
        if (enable_singular_proc_seq)
            execute(gnuplot_exe_path, plots, { run_singular_proc_seq, "singular-proc-seq" });

        if (enable_multiple_proc_seq)
            execute(gnuplot_exe_path, plots, { run_multiple_proc_seq, "multiple-proc-seq" });

        if (enable_multiple_proc_par)
            execute(gnuplot_exe_path, plots, { run_multiple_proc_par, "multiple-proc-par" });
    }

    return 0;
}

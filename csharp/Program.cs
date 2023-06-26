using System;
using System.Diagnostics;
using System.IO;
using System.Threading.Tasks;

//-------------------------------------------------------------------------------------------------

const int n = 100;
const string gnuplotExePath542 = "C:\\gnuplot542\\bin\\gnuplot.exe";
const string gnuplotExePath546 = "C:\\gnuplot546\\bin\\gnuplot.exe";

//-------------------------------------------------------------------------------------------------

void RunSingularProcSeq(string gnuplotExePath, string[] plots)
{
    using var proc = new Gp.Proc(gnuplotExePath);

    foreach (var plot in plots)
    {
        Console.Write(".");
        proc.Write(plot);
    }

    proc.ExitWait();
}

void RunMultipleProcSeq(string gnuplotExePath, string[] plots)
{
    foreach (var plot in plots)
    {
        Console.Write(".");
        using var proc = new Gp.Proc(gnuplotExePath);
        proc.Write(plot);
        proc.ExitWait();
    }
}

void RunMultipleProcPar(string gnuplotExePath, string[] plots)
{
    void Action(string plot)
    {
        Console.Write(".");
        using var proc = new Gp.Proc(gnuplotExePath);
        proc.Write(plot);
        proc.ExitWait();
    }

    Parallel.ForEach(plots, Action);
}

//-------------------------------------------------------------------------------------------------

void Execute(string gnuplotExePath, string[] plots, Action<string, string[]> run, string mode)
{
    Console.WriteLine("");
    Console.WriteLine("Gnuplot executable path: {0}", gnuplotExePath);
    Console.WriteLine("Execution mode for test: {0}", mode);
    Console.Write("Running");
    var sw = Stopwatch.StartNew();

    run(gnuplotExePath, plots);

    sw.Stop();
    Console.WriteLine("");
    Console.WriteLine("Number of plots created: {0}", plots.Length);
    Console.WriteLine("Elapsed time in seconds: {0}", sw.Elapsed.TotalSeconds);
}

//-------------------------------------------------------------------------------------------------

Directory.CreateDirectory("./output/");
var lines = File.ReadAllLines("./data.csv");
var plots = new string[n];

for (var i = 0; i < n; i++)
{
    plots[i] = Gp.Chart.CreatePlot($"./output/chart-{i:d2}.svg", lines, i);
}

var paths = new string[]
{
    gnuplotExePath542,
    gnuplotExePath546
};

foreach (var gnuplotExePath in paths)
{
    Execute(gnuplotExePath, plots, RunSingularProcSeq, "singular-proc-seq");
    Execute(gnuplotExePath, plots, RunMultipleProcSeq, "multiple-proc-seq");
    Execute(gnuplotExePath, plots, RunMultipleProcPar, "multiple-proc-par");
}

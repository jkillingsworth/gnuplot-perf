using System;
using System.Diagnostics;
using System.IO;
using System.Threading.Tasks;

//-------------------------------------------------------------------------------------------------

const int n = 100;
const string gnuplotExePath542 = "C:\\gnuplot542\\bin\\gnuplot.exe";
const string gnuplotExePath546 = "C:\\gnuplot546\\bin\\gnuplot.exe";

//-------------------------------------------------------------------------------------------------

void Render(string gnuplotExePath, string plot)
{
    using var proc = new Gp.Proc(gnuplotExePath);
    proc.Write(plot);
    proc.ExitWait();
}

//-------------------------------------------------------------------------------------------------

void RunSeq(string gnuplotExePath, string[] plots)
{
    Console.WriteLine("");
    Console.WriteLine("Gnuplot executable path: {0}", gnuplotExePath);
    Console.WriteLine("Execution mode for test: sequential");
    Console.Write("Running");
    var sw = Stopwatch.StartNew();

    foreach (var plot in plots)
    {
        Console.Write(".");
        Render(gnuplotExePath, plot);
    }

    sw.Stop();
    Console.WriteLine("");
    Console.WriteLine("Number of plots created: {0}", n);
    Console.WriteLine("Elapsed time in seconds: {0}", sw.Elapsed.TotalSeconds);
}

void RunPar(string gnuplotExePath, string[] plots)
{
    Console.WriteLine("");
    Console.WriteLine("Gnuplot executable path: {0}", gnuplotExePath);
    Console.WriteLine("Execution mode for test: parallel");
    Console.Write("Running");
    var sw = Stopwatch.StartNew();

    void Action(string plot)
    {
        Console.Write(".");
        Render(gnuplotExePath, plot);
    }

    Parallel.ForEach(plots, Action);

    sw.Stop();
    Console.WriteLine("");
    Console.WriteLine("Number of plots created: {0}", n);
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

RunSeq(gnuplotExePath542, plots);
RunPar(gnuplotExePath542, plots);
RunSeq(gnuplotExePath546, plots);
RunPar(gnuplotExePath546, plots);

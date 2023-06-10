using System;
using System.IO;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Text;

//-------------------------------------------------------------------------------------------------

Directory.CreateDirectory("./output/");

var n = 100;

var lines = File.ReadAllLines("./data.csv");
var plots = new string[n];

for (var i = 0; i < n; i++)
{
    plots[i] = Chart.CreatePlot($"./output/chart-{i:d2}.svg", lines, i);
}

var gnuplotExePath542 = @"C:\gnuplot542\bin\gnuplot.exe";
var gnuplotExePath546 = @"C:\gnuplot546\bin\gnuplot.exe";

void Render(string gnuplotExePath, string plot)
{
    using var proc = new Process();
    proc.StartInfo.FileName = gnuplotExePath;
    proc.StartInfo.UseShellExecute = false;
    proc.StartInfo.RedirectStandardInput = true;
    proc.StartInfo.StandardInputEncoding = new UTF8Encoding();
    proc.Start();
    proc.StandardInput.Write(plot);
    proc.StandardInput.Flush();
    proc.WaitForExit();
}

//-------------------------------------------------------------------------------------------------

void Run542()
{
    Console.WriteLine("");
    Console.WriteLine("Gnuplot version: 5.4.2");
    Console.WriteLine("Execution mode: sequential");
    Console.Write("Running");
    var sw = Stopwatch.StartNew();

    foreach (var plot in plots)
    {
        Console.Write(".");
        Render(gnuplotExePath542, plot);
    }

    sw.Stop();
    Console.WriteLine("");
    Console.WriteLine("Charts generated: {0}", n);
    Console.WriteLine("Elapsed time: {0}", sw.Elapsed);
}

//-------------------------------------------------------------------------------------------------

void Run542Parallel()
{
    Console.WriteLine("");
    Console.WriteLine("Gnuplot version: 5.4.2");
    Console.WriteLine("Execution mode: parallel");
    Console.Write("Running");
    var sw = Stopwatch.StartNew();

    void Action(string plot)
    {
        Console.Write(".");
        Render(gnuplotExePath542, plot);
    }

    Parallel.ForEach(plots, Action);

    sw.Stop();
    Console.WriteLine("");
    Console.WriteLine("Charts generated: {0}", n);
    Console.WriteLine("Elapsed time: {0}", sw.Elapsed);
}

//-------------------------------------------------------------------------------------------------

void Run546()
{
    Console.WriteLine("");
    Console.WriteLine("Gnuplot version: 5.4.6");
    Console.WriteLine("Execution mode: sequential");
    Console.Write("Running");
    var sw = Stopwatch.StartNew();

    foreach (var plot in plots)
    {
        Console.Write(".");
        Render(gnuplotExePath546, plot);
    }

    sw.Stop();
    Console.WriteLine("");
    Console.WriteLine("Charts generated: {0}", n);
    Console.WriteLine("Elapsed time: {0}", sw.Elapsed);
}

//-------------------------------------------------------------------------------------------------

void Run546Parallel()
{
    Console.WriteLine("");
    Console.WriteLine("Gnuplot version: 5.4.6");
    Console.WriteLine("Execution mode: parallel");
    Console.Write("Running");
    var sw = Stopwatch.StartNew();

    void Action(string plot)
    {
        Console.Write(".");
        Render(gnuplotExePath546, plot);
    }

    Parallel.ForEach(plots, Action);

    sw.Stop();
    Console.WriteLine("");
    Console.WriteLine("Charts generated: {0}", n);
    Console.WriteLine("Elapsed time: {0}", sw.Elapsed);
}

//-------------------------------------------------------------------------------------------------

Run542();
Run542Parallel();
Run546();
Run546Parallel();

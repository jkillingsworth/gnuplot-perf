module Program

open System.IO
open System.Diagnostics

//-------------------------------------------------------------------------------------------------

let n = 100
let gnuplotExePath542 = "C:\\gnuplot542\\bin\\gnuplot.exe";
let gnuplotExePath546 = "C:\\gnuplot546\\bin\\gnuplot.exe";

//-------------------------------------------------------------------------------------------------

let render gnuplotExePath plot =

    use proc = new Gp.Proc(gnuplotExePath)
    proc.Write(plot)
    proc.ExitAndWait()

//-------------------------------------------------------------------------------------------------

let runSeq gnuplotExePath plots =

    printfn ""
    printfn "Gnuplot executable path: %s" gnuplotExePath
    printfn "Execution mode for test: sequential"
    printf "Running"
    let sw = Stopwatch.StartNew()

    for plot in plots do
        printf "."
        render gnuplotExePath plot

    sw.Stop()
    printfn ""
    printfn "Number of plots created: %i" n
    printfn "Elapsed time in seconds: %f" sw.Elapsed.TotalSeconds

let runPar gnuplotExePath plots =

    printfn ""
    printfn "Gnuplot executable path: %s" gnuplotExePath
    printfn "Execution mode for test: parallel"
    printf "Running"
    let sw = Stopwatch.StartNew()

    let action plot =
        printf "."
        render gnuplotExePath plot

    Array.Parallel.iter action plots

    sw.Stop()
    printfn ""
    printfn "Number of plots created: %i" n
    printfn "Elapsed time in seconds: %f" sw.Elapsed.TotalSeconds

//-------------------------------------------------------------------------------------------------

Directory.CreateDirectory("./output/") |> ignore
let lines = File.ReadAllLines("./data.csv")
let plots = Array.init n (fun i -> Gp.Chart.createPlot $"./output/chart-{i:d2}.svg" lines i)

runSeq gnuplotExePath542 plots
runPar gnuplotExePath542 plots
runSeq gnuplotExePath546 plots
runPar gnuplotExePath546 plots

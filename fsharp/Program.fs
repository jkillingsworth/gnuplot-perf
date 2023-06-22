module Program

open System.IO
open System.Diagnostics

//-------------------------------------------------------------------------------------------------

let n = 100
let gnuplotExePath542 = "C:\\gnuplot542\\bin\\gnuplot.exe";
let gnuplotExePath546 = "C:\\gnuplot546\\bin\\gnuplot.exe";

//-------------------------------------------------------------------------------------------------

let runSeq gnuplotExePath plots =

    for plot in plots do
        printf "."
        use proc = new Gp.Proc(gnuplotExePath)
        proc.Write(plot)
        proc.ExitAndWait()

let runPar gnuplotExePath plots =

    let action plot =
        printf "."
        use proc = new Gp.Proc(gnuplotExePath)
        proc.Write(plot)
        proc.ExitAndWait()

    Array.Parallel.iter action plots

//-------------------------------------------------------------------------------------------------

let execute gnuplotExePath plots run mode =

    printfn ""
    printfn "Gnuplot executable path: %s" gnuplotExePath
    printfn "Execution mode for test: %s" mode
    printf "Running"
    let sw = Stopwatch.StartNew()

    run gnuplotExePath plots

    sw.Stop()
    printfn ""
    printfn "Number of plots created: %i" (Array.length plots)
    printfn "Elapsed time in seconds: %f" sw.Elapsed.TotalSeconds

//-------------------------------------------------------------------------------------------------

Directory.CreateDirectory("./output/") |> ignore
let lines = File.ReadAllLines("./data.csv")
let plots = Array.init n (fun i -> Gp.Chart.createPlot $"./output/chart-{i:d2}.svg" lines i)

execute gnuplotExePath542 plots runSeq "seq"
execute gnuplotExePath542 plots runPar "par"
execute gnuplotExePath546 plots runSeq "seq"
execute gnuplotExePath546 plots runPar "par"

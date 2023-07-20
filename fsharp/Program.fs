module Program

open System.IO
open System.Diagnostics

//-------------------------------------------------------------------------------------------------

let n = 100

let gnuplotExePath542 = "C:\\gnuplot542\\bin\\gnuplot.exe";
let gnuplotExePath548 = "C:\\gnuplot548\\bin\\gnuplot.exe";

let enableSingularProcSeq = true
let enableMultipleProcSeq = false
let enableMultipleProcPar = false

//-------------------------------------------------------------------------------------------------

let runSingularProcSeq gnuplotExePath plots =

    use proc = new Gp.Proc(gnuplotExePath)

    for plot in plots do
        printf "."
        proc.Write(plot)

    proc.ExitAndWait()

let runMultipleProcSeq gnuplotExePath plots =

    for plot in plots do
        printf "."
        use proc = new Gp.Proc(gnuplotExePath)
        proc.Write(plot)
        proc.ExitAndWait()

let runMultipleProcPar gnuplotExePath plots =

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
let plots = Array.init n (fun i -> Gp.Chart.createPlot $"./output/chart-{i:d3}.svg" lines i)

let paths = [|
    gnuplotExePath542
    gnuplotExePath548
|]

for gnuplotExePath in paths do

    if enableSingularProcSeq then
        execute gnuplotExePath plots runSingularProcSeq "singular-proc-seq"

    if enableMultipleProcSeq then
        execute gnuplotExePath plots runMultipleProcSeq "multiple-proc-seq"

    if enableMultipleProcPar then
        execute gnuplotExePath plots runMultipleProcPar "multiple-proc-par"

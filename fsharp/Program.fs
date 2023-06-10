module Program

open System.IO
open System.Diagnostics
open System.Text

//-------------------------------------------------------------------------------------------------

Directory.CreateDirectory("./output/") |> ignore

let n = 100

let lines = File.ReadAllLines("./data.csv")
let plots = Array.init n (fun i -> Chart.createPlot $"./output/chart-{i:d2}.svg" lines i)

let gnuplotExePath542 = @"C:\gnuplot542\bin\gnuplot.exe";
let gnuplotExePath546 = @"C:\gnuplot546\bin\gnuplot.exe";

let render gnuplotExePath plot =

    use proc = new Process()
    proc.StartInfo.FileName <- gnuplotExePath
    proc.StartInfo.UseShellExecute <- false
    proc.StartInfo.RedirectStandardInput <- true
    proc.StartInfo.StandardInputEncoding <- new UTF8Encoding()
    proc.Start() |> ignore
    proc.StandardInput.Write(plot : string)
    proc.StandardInput.Flush()
    proc.WaitForExit()
    proc.Close()

//-------------------------------------------------------------------------------------------------

let run542 () =

    printfn ""
    printfn "Gnuplot version: 5.4.2"
    printfn "Execution mode: sequential"
    printf "Running"
    let sw = Stopwatch.StartNew()

    for plot in plots do
        printf "."
        render gnuplotExePath542 plot

    sw.Stop()
    printfn ""
    printfn "Charts generated: %i" n
    printfn "Elapsed time: %A" sw.Elapsed

//-------------------------------------------------------------------------------------------------

let run542Parallel () =

    printfn ""
    printfn "Gnuplot version: 5.4.2"
    printfn "Execution mode: parallel"
    printf "Running"
    let sw = Stopwatch.StartNew()

    let action plot =
        printf "."
        render gnuplotExePath542 plot

    Array.Parallel.iter action plots

    sw.Stop()
    printfn ""
    printfn "Charts generated: %i" n
    printfn "Elapsed time: %A" sw.Elapsed

//-------------------------------------------------------------------------------------------------

let run546 () =

    printfn ""
    printfn "Gnuplot version: 5.4.6"
    printfn "Execution mode: sequential"
    printf "Running"
    let sw = Stopwatch.StartNew()

    for plot in plots do
        printf "."
        render gnuplotExePath546 plot

    sw.Stop()
    printfn ""
    printfn "Charts generated: %i" n
    printfn "Elapsed time: %A" sw.Elapsed

//-------------------------------------------------------------------------------------------------

let run546Parallel () =

    printfn ""
    printfn "Gnuplot version: 5.4.6"
    printfn "Execution mode: parallel"
    printf "Running"
    let sw = Stopwatch.StartNew()

    let action plot =
        printf "."
        render gnuplotExePath546 plot

    Array.Parallel.iter action plots

    sw.Stop()
    printfn ""
    printfn "Charts generated: %i" n
    printfn "Elapsed time: %A" sw.Elapsed

//-------------------------------------------------------------------------------------------------

run542 ()
run542Parallel ()
run546 ()
run546Parallel ()

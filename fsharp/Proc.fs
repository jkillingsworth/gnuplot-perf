namespace global

open System
open System.Diagnostics
open System.Text

//-------------------------------------------------------------------------------------------------

[<Sealed>]
type Proc(gnuplotExePath) =

    let proc = new Process()
    do
        proc.StartInfo.FileName <- gnuplotExePath
        proc.StartInfo.UseShellExecute <- false
        proc.StartInfo.RedirectStandardInput <- true
        proc.StartInfo.StandardInputEncoding <- new UTF8Encoding()
        proc.Start() |> ignore

    member this.Write(plot : string) =
        proc.StandardInput.Write(plot)
        proc.StandardInput.Flush()

    member this.ExitAndWait() =
        this.Write("exit\n")
        proc.WaitForExit()

    interface IDisposable with
        member this.Dispose() = proc.Dispose()

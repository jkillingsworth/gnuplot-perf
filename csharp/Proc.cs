using System;
using System.Diagnostics;
using System.Text;

//-------------------------------------------------------------------------------------------------

internal sealed class Proc : IDisposable
{
    private readonly Process proc;

    public Proc(string gnuplotExePath)
    {
        proc = new Process();
        proc.StartInfo.FileName = gnuplotExePath;
        proc.StartInfo.UseShellExecute = false;
        proc.StartInfo.RedirectStandardInput = true;
        proc.StartInfo.StandardInputEncoding = new UTF8Encoding();
        proc.Start();
    }

    public void Write(string value)
    {
        proc.StandardInput.Write(value);
        proc.StandardInput.Flush();
    }

    public void ExitWait()
    {
        Write("exit\n");
        proc.WaitForExit();
    }

    public void Dispose()
    {
        proc.Dispose();
    }
}

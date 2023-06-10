using System.IO;
using System.Linq;

//-------------------------------------------------------------------------------------------------

internal static class Chart
{
    private const string template = @"
set terminal svg size 720 405 font 'monospace'
set encoding utf8
set output '{0}'

$data0 << EOD
{1}
EOD

$data1 << EOD
{2}
EOD

count = {3}
index = {4}
lower = 5.0
upper = 6.0

set style line 1 linewidth 1 linecolor rgb '#e6e6e6'
set style line 2 linewidth 2 linecolor rgb '#e6e6e6'

set xtics scale 0, 0.0001
set ytics scale 0, 0.0001

set border linewidth 1.2

set grid linestyle 1
set grid xtics mxtics
set grid ytics mytics

set key box linecolor rgb '#808080'
set key opaque
set key samplen 1
set key top left
set key reverse Left

set xlabel 'x'
set xrange [-(count*0.01):+(count*1.01)]
set xtics 0, (count / 8), count

set ylabel 'y'
set yrange [(lower-(upper-lower)*0.01):(upper+(upper-lower)*0.01)]
set ytics lower, ((upper - lower) / 8), upper
set format y '%5.3f'

set linetype 1 linewidth 1 linecolor rgb '#808080'
set linetype 2 linewidth 1 linecolor rgb '#ff4040'
set linetype 3 linewidth 1 linecolor rgb '#ffa0a0'
set linetype 4 linewidth 1 linecolor rgb '#ffa0a0'

plot $data0 using 1:2:3:4:5 with financebars title sprintf('Chart %02i', index),\
     $data1 using 1:2 with lines notitle,\
     $data1 using 1:3 with lines notitle,\
     $data1 using 1:4 with lines notitle

exit
";

    public static string CreatePlot(string outputPath, string[] lines, int index)
    {
        string MapSignal(string line, int i)
        {
            var items = line.Split(",");
            var a = items[0];
            var b = items[1];
            var c = items[2];
            var d = items[3];
            return string.Format("{0} {1} {2} {3} {4}", i, a, b, c, d);
        }

        string MapMetric(string line, int i)
        {
            var items = line.Split(",");
            var e = items[4];
            var f = items[5];
            var g = items[6];
            return string.Format("{0} {1} {2} {3}", i, e, f, g);
        }

        var data0 = string.Join("\n", lines.Select(MapSignal));
        var data1 = string.Join("\n", lines.Select(MapMetric));
        var count = lines.Length;

        var path = Path.GetFullPath(outputPath);
        var plot = string.Format(template, path, data0, data1, count, index);

        return plot;
    }
}

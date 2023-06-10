module Chart

open System
open System.IO

//-------------------------------------------------------------------------------------------------

let private template = @"
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
"

let createPlot outputPath lines index =

    let mapSignal i (line : string) =
        let items = line.Split(",")
        let a = items.[0]
        let b = items.[1]
        let c = items.[2]
        let d = items.[3]
        sprintf "%O %O %O %O %O" i a b c d

    let mapMetric i (line : string) =
        let items = line.Split(",")
        let e = items.[4]
        let f = items.[5]
        let g = items.[6]
        sprintf "%O %O %O %O" i e f g

    let data0 = lines |> Array.mapi mapSignal |> String.concat "\n"
    let data1 = lines |> Array.mapi mapMetric |> String.concat "\n"
    let count = lines.Length

    let path = Path.GetFullPath(outputPath)
    let plot = String.Format(template, path, data0, data1, count, index)

    plot

#include <filesystem>
#include <format>
#include <functional>
#include <numeric>
#include <string>
#include <vector>

//-------------------------------------------------------------------------------------------------

namespace chart
{
    constexpr auto plot_template = R"(
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
)";

    std::string map_signal(const std::string& line, int i)
    {
        std::vector<std::string> items;
        std::stringstream stream(line);
        std::string s;
        while (std::getline(stream, s, ',')) items.push_back(s);
        auto& a = items[0];
        auto& b = items[1];
        auto& c = items[2];
        auto& d = items[3];
        return std::format("{0} {1} {2} {3} {4}", i, a, b, c, d);
    }

    std::string map_metric(const std::string& line, int i)
    {
        std::vector<std::string> items;
        std::stringstream stream(line);
        std::string s;
        while (std::getline(stream, s, ',')) items.push_back(s);
        auto& e = items[4];
        auto& f = items[5];
        auto& g = items[6];
        return std::format("{0} {1} {2} {3}", i, e, f, g);
    }

    std::string reduce(const std::vector<std::string>& lines, const std::function<std::string(const std::string&, int)>& map_func)
    {
        std::string s;

        for (auto it = lines.begin(); it != lines.end(); it++)
        {
            int i = static_cast<int>(std::distance(lines.begin(), it));
            if (i != 0) s.append("\n");
            s.append(map_func(*it, i));
        }

        return s;
    }

    std::string create_plot(const std::string& outputPath, const std::vector<std::string>& lines, int index)
    {
        auto data0 = reduce(lines, map_signal);
        auto data1 = reduce(lines, map_metric);
        auto count = lines.size();

        auto path = std::filesystem::absolute(outputPath).string();
        auto args = std::make_format_args(path, data0, data1, count, index);
        auto plot = std::vformat(plot_template, args);

        return plot;
    }
}

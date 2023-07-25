#!/bin/bash

set -e

gnuplot542="C:\\gnuplot542\\bin\\gnuplot.exe"
gnuplot548="C:\\gnuplot548\\bin\\gnuplot.exe"

testfile="test-100.plt"

mkdir -p output

echo
echo "Gnuplot executable path: $gnuplot542"
echo "Execution mode for test: direct-from-file"
time $gnuplot542 $testfile

echo
echo "Gnuplot executable path: $gnuplot548"
echo "Execution mode for test: direct-from-file"
time $gnuplot548 $testfile

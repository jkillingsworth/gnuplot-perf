#!/bin/bash

set -e

gnuplot542="C:\\gnuplot542\\bin\\gnuplot.exe"
gnuplot548="C:\\gnuplot548\\bin\\gnuplot.exe"

testfile="test-100.plt"

mkdir -p output

echo
echo "Gnuplot executable path: $gnuplot542"
echo "Execution mode for test: piped-from-stdin"
time cat $testfile | $gnuplot542

echo
echo "Gnuplot executable path: $gnuplot548"
echo "Execution mode for test: piped-from-stdin"
time cat $testfile | $gnuplot548

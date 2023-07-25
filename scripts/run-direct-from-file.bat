@echo off

set gnuplot542=C:\gnuplot542\bin\gnuplot.exe
set gnuplot548=C:\gnuplot548\bin\gnuplot.exe

set testfile=test-100.plt

if not exist output mkdir output

echo:
echo Gnuplot executable path: %gnuplot542%
echo Execution mode for test: direct-from-file
echo %time%
%gnuplot542% %testfile%
echo %time%

echo:
echo Gnuplot executable path: %gnuplot548%
echo Execution mode for test: direct-from-file
echo %time%
%gnuplot548% %testfile%
echo %time%

## Overview

This repo contains a set of tests that compare the performance of two different versions of Gnuplot running on a Windows system.

To run the tests, the following prerequisites need to be in place:

* Download [version 5.4.2](https://sourceforge.net/projects/gnuplot/files/gnuplot/5.4.2/gp542-win64-mingw.7z/download) and unpack into `C:\gnuplot542\`
* Download [version 5.4.8](https://sourceforge.net/projects/gnuplot/files/gnuplot/5.4.8/gp548-win64-mingw.7z/download) and unpack into `C:\gnuplot548\`

You can use [7-Zip](https://www.7-zip.org/) to unpack the files.

The test suite is implemented in three different programming languages. The build and run instructions are detailed below. In addition, there are scripts that can run the tests directly from either a Bash prompt or a Windows command prompt without building any of the test programs from source.

## C++

#### Visual Studio

* Navigate to the `cpp` folder
* Open the `ConsoleApp.sln` file
* Press `Ctrl`+`F5` to build and run the program

#### MSBuild

Open the developer command prompt for Visual Studio and enter the following commands:

```
cd cpp
msbuild ConsoleApp.sln -p:Configuration=Release
bin\console_app.exe
```

#### MSYS2 (MINGW64)

Install [MSYS2](https://www.msys2.org/) and then install the prerequisite packages:

```
pacman -S make
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-tbb
```

Enter the following commands to build and run:

```
cd cpp
make all
bin/console_app.exe
```

#### MSYS2 (UCRT64)

Install [MSYS2](https://www.msys2.org/) and then install the prerequisite packages:

```
pacman -S make
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-tbb
```

Enter the following commands to build and run:

```
cd cpp
make all
bin/console_app.exe
```

## C#

#### Visual Studio

* Navigate to the `csharp` folder
* Open the `ConsoleApp.sln` file
* Press `Ctrl`+`F5` to build and run the program

#### .NET Command Line Interface

Open the command prompt and enter the following commands:

```
cd csharp
dotnet run --project ConsoleApp.csproj -c Release
```

## F#

#### Visual Studio

* Navigate to the `fsharp` folder
* Open the `ConsoleApp.sln` file
* Press `Ctrl`+`F5` to build and run the program

#### .NET Command Line Interface

Open the command prompt and enter the following commands:

```
cd fsharp
dotnet run --project ConsoleApp.fsproj -c Release
```

## Scripts

#### Windows Command Prompt

Enter the following commands:

```
cd scripts
run-piped-from-stdin.bat
run-direct-from-file.bat
```

#### MSYS2 Bash Prompt

Enter the following commands:

```
cd scripts
./run-piped-from-stdin.sh
./run-direct-from-file.sh
```

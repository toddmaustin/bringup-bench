# Bringup-Bench Benchmark Suite

Bringup /bring-up/ **verb**
Refers to the process of bringing a newly design and implement chip, compiler, or operating system to life and verifying its functionality.

## Introduction

The Bringup-Bench Benchmark Suite is a collection of C code benchmarks that are purpose-built to be useful in "bringing up" newly designed CPUs, acclerators, compilers, and operating systems. Bringup-Bench facilitates the bringup process because i) its benchmarks have zero library dependencies since the "libmin" library included in the repo implements all necessary library calls, ii) its benchmarks have near-zero system dependencies since only four simple system calls must be implement to support all the benchmarks, and iii) the benchmark build process supports code-based read-only file access, which allows benchmark file inputs to be packaged into the benchmark build.

I (Todd Austin) have used this benchmark suite to bring up new CPUs, new compiler infrastructure and even recently a Turing-complete encrypted computation accelerator. Most developers probably don't need bringup-bench, but if you do need it, you probably need it very badly!

## Bringing up Bringup-Bench
To build and test a benchmark, simply enter one of the benchmark directories and execute the following makefile command:
```
make TARGET=<target> clean build test
```
This command will first "clean" the benchmark directory and then "build" the application, and "test" that it is running correctly. The \<target> indicates the specific target that the application should be built for. Currently, Bringup-Bench support the following targets: 

- **Linux host target - TARGET=host** - This target builds the benchmark to run as a Linux application.

- **Standalone target - TARGET=sa** - This target builds the benchmark to run as a memory-only standalone application. For this target, all benchmark output is spooled to a pre-defined memory buffer, and the libmin\_success() and libmin\_fail() intefaces result in the application spinning at a specific code address. This mode is designed for bringing up CPUs and accelerators that do not yet have any OS or device I/O support.

Each benchmark support three standard Makefile targets: build, test, and clean

- **build** - Builds the benchmark

- **test** - Runs the benchmark and validates its output.

- **clean** - Deleted all derived files.

For example, to build, test and then clean the Bubble Sort benchmark in encrypted mode:
```
make TARGET=host build
make TARGET=host test
make TARGET=host clean
```

To assist in running experiments, the top-level Makefile includes a few useful targets:
```
make run-tests   # clean, build, and test all benchmarks in all target modes (host,sa)
make all-clean   # clean all benchmark directories
```
You should be able to adapt these targets to your own project-specific tasks.

## Maximum benchmarks

The Bringup-Bench benchmarks were selected for their minimal library and system dependencies, while still being interesting and non-trival codes.i Currently, the benchmark suite supports the following benchmarks. Note that the benchmarks tagged with (FP) require some form of floating point support, and the remaining benchmarks only require integer and string computation.

- **ackermann** - Calculates the Ackermann function for a variety of input values.

- **anagram** - Computes anagrams for the phrases in "input.txt" using the diction in the "words". This benchmark uses code-based read-only file access for multiple files.

- **banner** - Prints out a fancy vertical banner.

- **boyer-moore-search** - Performs a Boyer-Moore optimized search, given a test string and a large string to search.

- **bubble-sort** - Performs a bubble sort on a randomly generated internal list of integers.

- **c-interp** - A C language interpreter that interprets the test program "hello.c". This benchmark uses code-based read-only file access.

- **cipher** - A TEA cipher implementation the encrypts and decrypts some fixed test data.

- **dhrystone** - An old-school Dhrystone benchmark.

- **distinctness** - Computes if every element of an array is unique.

- **fft-int** - Performs an integer fast-Fourier-transform on fixed integer input data.

- **flood-fill** - Performs a color-based flood fill of a fixed tw-dimensional text array.

- **frac-calc** - Computes calculations on proper and improper fractions.

- **hanoi** - Solves the Tower's of Hanoi problem for a variable number of towers.

- **heapsort** - Performs a heap sort on a randomly generated data set

- **kepler** - Calculates an orbital path for a planetary body based on the Kepler parameters.

- **longdiv** - Computes a long division using the pencil-on-paper method.

- **mandelbrot** - Calculate and print using ASCII graphics a Mendelbrot fractal.

- **mersenne** - Generate a sequence of pseudo-random numbers using the Mersenne Twister algorithm.

- **natlog** - Compute the value of natural log e, using an iterative method.

- **nr-solver** - Computes a square-root value using a Newton-Raphson solver.

- **parrondo** - A game theory based solver that simulates Parrondo's paradox.

- **pascal** - Compute Pascal's triangle, to a specified depth.

- **shortest-path** - Solves the all-pairs shortest path problem using the Floyd-Warshall algorithm.

- **sieve** - Computes the prime values within a specified range, using the Sieve of Eratosthenes algorithmm

- **skeleton** - A minimal program, for use a starting point for new application ports and developments.

- **totient** - Calculates the Euler totient function phi.

## Minimal library dependencies

Bringup-Bench has no library dependencies, to reduce the amount of system infrastructure needed to get your first application running. Instead of needing system libraries, Bringup-bench implements its own library in "libmin". "libmin" includes most of what simple applications need, including:

- printing values
- parsing numbers from text
- options parsing
- string processing
- memory copy and setting
- program exit interfaces
- pseudo-random number generation
- dynamic storage allocator
- code-based read-only file access functions
- sorting functions
- character class tests (from ctype.h)
- floating matho functions

See the file "common/libmin.h" for more details.

## Minimal system dependencies

To minimize the system OS requirements, the Bringup-Bench only requires four system call interfaces to be implement. The interfaced required are as follows:
```
/* benchmark completed successfully */
void libtarg_success(void);

/* benchmark completed with error CODE */
void libtarg_fail(int code);

/* output a single character, to whereever the target wants to send it... */
void libtarg_putc(char c);

/* get some memory */
void *libtarg_sbrk(size_t inc);
```
Once these four interfaces are implemented, all of the Bringup-Bench benchmarks can be built and run. To facilitate in testing, the "TARGET=host" target defines the four required system interfaces by passing them on to the Linux OS. In addition, the repo also provides a standalone target "TARGET=sa" which only requires that the target support provbable memory.

## Using the code-based read-only file system

Using the code-based read-only file system, it is possible for a benchmark to access a read-only file that is incorporated into its code. To convert an input file to a read-only code-based file, use the following command (shown for the benchmark "anagram"):
```
python3 scriptsr/file2hex.py words words.h __words
```
Where "words" is the file to convert, "words.h" is the name of the output header file with the data, and "__words" is the name of the variable defined in the header file "words.h". The resulting file produces two values: __words_sz is the size of the data in the __words array. To access the file, include into a MFILE definition in the benchmark file, for example:
```
MFILE __mwords = {
  "words",
  __words_sz,
  __words,
  0
};
MFILE *mwords = &__mwords;
```
Now the code-based read-only memory file "mwords" is now available for opening, reading, and closing. The following interfaces are available to access memory files:
```
/* open an in-memory file */
void libmin_mopen(MFILE *mfile, const char *mode);

/* return in-memory file size */
size_t libmin_msize(MFILE *mfile);

/* at end of file */
int libmin_meof(MFILE *mfile);

/* close the in-memory file */
void libmin_mclose(MFILE *mfile);

/* read a buffer from the in-memory file */
size_t libmin_mread(void *ptr, size_t size, MFILE *mfile);

/* get a string from the in-memory file */
char *libmin_mgets(char *s, size_t size, MFILE *mfile);

/* read a character from the in-memory file */
int libmin_mgetc(MFILE *mfile);
```

## Porting the Bringup-Bench to other targets

To port the Bringup-bench to your new CPU, accelerator, compiler, or operating system, you need only concern yourself with the "libtarg.h" and "libmin.c" files. First define a new target specifier in "Makefile" and then add it to the "libtarg.h" and "libtarg.c" files. Inside the "libtarg.h" file you will need to define basic data type sizes plus define how the benchmarks access "vararg" parameter arguments. Inside the "libtarg.c" file, you will need to define the following four system call interfaces:
```
/* benchmark completed successfully */
void libtarg_success(void);

/* benchmark completed with error CODE */
void libtarg_fail(int code);

/* output a single character, to whereever the target wants to send it... */
void libtarg_putc(char c);

/* get some memory */
void *libtarg_sbrk(size_t inc);
```

## Licensing details

The portions of the benchmark suite that was built by the benchmark team are (C) 2021-2023 and available for use under
the [Apache License, version 2.0](https://www.apache.org/licenses/LICENSE-2.0) 

And, thanks to the respective authors of the benchmarks that were adapted for the VIP Benchmark Suite from other efforts.








This directory tree contains the WorkBench benchmark suite, industrially
hardened by BitRaker, and ported to Linux/x86 and ARM ADS (advanced
development system).  This brief document describes the contents of the 
WorkBench, the configuration of the WorkBench, how to run WorkBench 
experiments, and how to use Bitraker tools in conjunction with the WorkBench. 

WorkBench DIRECTORY STRUCTURE:
------------------------------

The WorkBench benchmark directories are arranged as follows:

WorkBench-X.X/		- top level directory

  README		- This file.
  Makefile.arm-ads	- ARM ADS build configuration, emulation with ARMulator
  Makefile.x86		- Linux/x86 GNU GCC build configuration, native x86 
                          execution
  Makefile.simforge	- ARM ADS build configuration, emulation with SimForge

  automotive/		- automotive/industrial benchmarks

    basicmath/		- basic math
    bitc/		- bit counting
    ellf/		- digital filters
    qsort/		- sorting routines
    susan/		- industrial imaging application

  common/		- WorkBench-wide source files (for portability)

  consumer/		- consumer-oriented applications

    forth/		- forth interpreter
    jpeg/		- jpeg graphics encoding/decoding
    lout/		- html typesetting
    mad/		- MP3 decoding
    tiff/		- digital image processing (b/w, quant, dither, etc)

  network/		- networking applications

    dijkstra/		- Dijkstra's packet routing algorithm
    patricia/		- Patricia tree based routing lookup

  office/		- office appliance applications

    grep/		- text searches using regular expressions
    ispell/		- spell checking

  security/		- security applications

    blowfish/		- blowfish bulk cipher encode/decode
    rijndael/		- Rijndael (AES) standard bulk encode/decode
    rsa/		- RSA public key cipher sign/seal
    sha/		- SHA data digest algorithm

  telecom/		- telecommunication oriented applications

    adpcm/		- adaptive differential pulse code modulation codec
    crc32/		- 32-bit cyclic redundancy checksum algorithm
    fft/		- fast fourier transform algorithm
    gsm/		- GSM lossy speech compression encode/decode


WorkBench INDIVIDUAL BENCHMARK DIRECTORY STRUCTURE:
----------------------------------------

Individual benchmark directories are arranged as follows:

Makefile.<variant>	- Makefile, one per unit benchmark "experiment"
src/			- benchmark source code
tests/			- test inputs, reference outputs
dist/			- original benchmark copyrights, distribution files


CONFIGURING THE WorkBench TARGET MACHINE:
---------------------------------

To start using WorkBench, you need to configure it for the type of machine 
you will be building for and running on.  This is accomplished by issuing a 
single command to create a symbolic link file called Makefile.config that
refers to Makefile.<yourTarget>.  The currently supported targets and their 
configuration commands are as follows:

Linux/x86:
  ln -sf Makefile.x86 Makefile.config

ARM ADS (advanced development system with semi-hosting)
  ln -sf Makefile.arm-ads Makefile.config

SimForge (advanced development system with semi-hosting)
  ln -sf Makefile.simforge Makefile.config

Makefile.arm-ads is a top-level configuration makefile for compilation
on ARM ADS tools and emulation with ARMulator.  Makefile.x86 is a
top-level configuration makefile for compilation with GNU GCC on
Linux/x86 and native execution.  Makefile.simforge is a top-level
makefile for compilation on ARM ADS tools and emulation on SimForge.

WorkBench currently ships with all of the Benchmark executabls compiled 
on ARM ADS tools.  If you plan on running WorkBench Experiments on Windows
or Linux, you will probably be using Simforge as your ARM emulator, so 
'ln -sf Makefile.simforge Makefile.config' 
would be the configuration command to use. Makefile.simforge also includes 
the targets used to drive Anvil testing.  The use of Anvil and Simforge 
is described later in this document.


RUNNING INDIVIDUAL WorkBench EXPERIMENTS:
-----------------------------------------

WorkBench supports a large number of programs, and even a larger number
of "experiments".  Experiments are a unique combination of program,
inputs, and validated results.  To see the experiments supported,
run the following command:

  ./runexps.sh

Specific experiments can be invoked by specifying experiment
"tag", followed by a sequence of Makefile targets.  For example, to
run the large experiments for the benchmark "susan":

  ./runexps.sh susan:large run-tests

Experiment tags are a sequence of tokens separated by ":"'s.  Commonly
supported tags are "all", "large",  and "small"; and also program
class tags "automotive", "consumer", "network", "office", "security",
and "telecom".  Individual program names are also supported.

All benchmark makefiles support a variety of Makefile targets,
including:

  clean			- clean up all build/test generated outputs
  clean-build		- clean all outputs (except executable)
  build			- build the test program
  run-tests		- run all tests and validate outputs


WorkBench TOPLEVEL RUN SCRIPTS:
-------------------------------

The "runexps.sh" script can be used invoke a specific makefile
target for all WorkBench experiments.  For example, to clean
all the experiment directories:

  ./runexps.sh all clean

The toplevel script supports up to 8 targets, executed in the
sequence specified.  So, for example, to clean, build and
run tests for all experiments, execute the following command:

  ./runexps.sh all clean build run-tests

Errors in the output can be identified with the top-level script
"runcheck.sh".  For example, the following commands will run all
experiments with full validation:

  ./runexps.sh all clean build run-tests |& tee MAKE.log
  ./runcheck.sh MAKE.log

If the runcheck.sh command has any output, at least one of the
experiments failed.  Details are in the log file...


USING ANVIL AND SIMFORGE IN CONJUNCTION WITH THE WorkBench 
----------------------------------------------------------

Several files need to be installed and some environment variables 
need to be modified before WorkBench executables can be instrumented 
in order to perform fast targeted program analysis. 

anvil.dll - This is the Anvil API and is used by your Tool.exe to
instrument/path a binary file.   The anvil.dll should be found
in your environment variable (PATH on windows and LD_LIBRARY_PATH on linux).

simforge.exe - This is the Bitraker ARM simulator, and is used to
run the ARM program on an x86 desktop machine.  The directory
with the simforge.exe should be found in your PATH environment variable.

anvilanalysis.dll or anvilanalysis.so - This is the BitRaker analysis
shared library that is used to enable your code to run efficiently
when using simforge.exe.  Make sure that the directory with this
shared library is found on your search environment variables 
(PATH for windows and LD_LIBRARY_PATH for linux).

Note, that these 3 files are found in the /bin directory for the
Anvil install.  You need to make sure that this directory is found
on the corresponding environment variables listed above.


In addition to the above files, there are 2 files created by the
user of Anvil that are required to perform targeted program analysis.

<yourToolName>.exe - This it the executable you created to perform your
targeted ARM binary analysis containing all of your instrumentation
code.  This is either compiled for windows or linux, depending upon
your Desktop target.  The directory containing this binary should be
specified in your PATH environment variable.

<yourToolName>.dll or <yourToolName>.so - This is the shared library 
(.dll on windows and.so on linux) that contains your analysis code.  
This shared library must be locatable on your search environment variables 
(PATH for windows and LD_LIBRARY_PATH for linux).

Note, that <yourToolName>.exe must have the -i flag specified, and the 
-o flag specified as in the AnvilTemplate tool example.  "-i" specifies 
the input ARM binary name, and "-o" is used to specify the output binary
name.


Anvil SPECIFIC RUN OPTIONS:
---------------------------

Two shell scripts, anvil-build.sh and anvil-run.sh, have been included 
with Workbench to facilitate instrumentation of WorkBench executables 
using Anvil created tools and emulation of instrumented WorkBench 
experiments on Simforge.  

USE OF anvil-build.sh

To instrument all of the executables in a particular benchmark directory
with an Anvil created tool: 

  ./anvil-build.sh <benchMarkName>:build <yourToolName>.exe

To instrument all executables in the Workbench with an Anvil created tool: 

  ./anvil-build.sh all:build <yourToolName>.exe

USE OF anvil-run.sh

To run a particular experiment (that has been instrumented) on Simforge:

  ./anvil-run.sh <experimentName> anvilanalysis.dll

To run all experiments  (all executables in theWorkBench must already be 
instrumented) 

  ./anvil-run.sh all anvilanalysis.dll 

If necessary, special simulator arguments can be passed to anvil-run.sh, e.g.:

  ./anvil-run.sh <experimentName> anvilanalysis.dll -sim_opt1 -sim_opt2 ...

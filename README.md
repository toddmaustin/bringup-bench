# Bringup-Bench Benchmark Suite

Bringup */bring-up/* **verb**
Refers to the process of bringing a newly designed and implemented CPU, accelerator, compiler, or operating system to life and verifying its functionality.

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

/* output a single character, to wherever the target wants to send it... */
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

/* output a single character, to wherever the target wants to send it... */
void libtarg_putc(char c);

/* get some memory */
void *libtarg_sbrk(size_t inc);
```

## Licensing details

The portions of the benchmark suite that was built by the benchmark team are (C) 2021-2023 and available for use under
the [Apache License, version 2.0](https://www.apache.org/licenses/LICENSE-2.0) 

And, thanks to the respective authors of the benchmarks that were adapted for the Bringup-Bench Benchmark Suite from other efforts.


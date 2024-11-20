# Bringup-Bench Benchmark Suite

Bringup */bring-up/* **verb**
Refers to the process of bringing a newly designed and implemented CPU, accelerator, compiler, or operating system to life and verifying its functionality.

## Introduction

The Bringup-Bench Benchmark Suite is a collection of C code benchmarks that are purpose-built to be useful in "bringing up" newly designed CPUs, acclerators, compilers, and operating systems. Bringup-Bench facilitates the bringup process because i) its benchmarks have zero library dependencies since the "libmin" library included in the repo implements all the necessary library calls, ii) its benchmarks have near-zero system dependencies since only four simple system calls must be implement to support all the benchmarks, and iii) the benchmark build process supports code-based read-only file access, which allows benchmark file inputs to be packaged into the benchmark build.

I (Todd Austin) have used this benchmark suite to bring up new CPUs, new compiler infrastructure and even recently a Turing-complete encrypted computation accelerator. Most developers probably don't need bringup-bench, but if you do need it, you probably need it very badly!

## Bringing up Bringup-Bench
To build and test a benchmark, simply enter one of the benchmark directories and execute the following makefile command:
```
make TARGET=<target> clean build test
```
This command will first "clean" the benchmark directory and then "build" the application, and "test" that it is running correctly. The \<target> indicates the specific target that the application should be built for. Currently, Bringup-Bench support the following targets: 

- **Linux host target - TARGET=host** - This target builds the benchmarks to run as a Linux application.

- **Standalone target - TARGET=standalone** - This target builds the benchmarks to run as a memory-only standalone application. For this target, all benchmark output is spooled to a pre-defined memory buffer, and the libmin\_success() and libmin\_fail() intefaces result in the application spinning at a specific code address. This mode is designed for bringing up CPUs and accelerators that do not yet have any OS or device I/O support. See common/libtarg.c for the internal intefaces used to spool program output to internal buffers. This particular target is useful in bringing up CPUs when they still have no I/O support, simply spool benchmark output to DRAM, and dump the DRAM after the benchmark completes.

- **Simple_System target - TARGET=simple** - This target build the benchmarks to run in the RISC-V Simple_System simulation environment. Simple_system allows hardware developers to do SystemVerilog development on Verilator, with fast SystemVerilog simulation using the Simple_System target. The Simple_System target supports a character output device, plus a simple memory system. By default, this is an integer computation only mode, so any FP in the benchmarks will be emulated with GCC's soft-float support. To learn more about the RISC-V Simple_System, go here: https://github.com/lowRISC/ibex/blob/master/examples/simple_system/README.md. The current version of the Simple_System target was tested with: 1) Ibex "small" core, 2) Simple_System default devices and memory configuration.

- **RISC-V Spike target = TARGET=spike** - This target is identical to the "simple" target, as it build RISC-V binaries to be run on the Spike instruction set simulator (ISS). Spike is configured to support the Simple_system RISC-V I/O devices. This target is useful as a "golden" model to compare against execution traces occurring on a (perhaps buggy) RTL design target. Before first running a simulation, do a "make spike-build" in the top-level bringup-bench directory to make the Spike device DLL.

- **HashAlone Host target - TARGET=hashalone-host** - This target builds the benchmarks to run on x86/Linux with a hashing output device. Instead of producing output, hash-alone binaries simply send the program output to a hash function. When the program completes it prints the final value of the hash function, which is cryptographically unique for every possible output of the program.

- **HashAlone Spike target - TARGET=hashalone-spike** - This target builds the benchmarks to run on bare-metal RISC-V with a hashing output device. Instead of producing output, hash-alone binaries simply send the program output to a hash function. When the program completes it prints the final value of the hash function, which is cryptographically unique for every possible output of the program. Use this target to enhance your "golden model" to support reference hash-alone signatures for the bringup-bench benchmarks.

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
make TARGET=<target> run-tests   # clean, build, and test all benchmarks in the specified target mode (host, standalone, simple)
make all-clean   # clean all benchmark directories for all supported targets
```
You should be able to adapt these targets to your own project-specific tasks.

## Benchmarks


The Bringup-Bench benchmarks were selected for their minimal library and system dependencies, while still being interesting and non-trival codes.i Currently, the benchmark suite supports the following benchmarks. Note that the benchmarks tagged with (FP) require some form of floating point support, and the remaining benchmarks only require integer and string computation.

- **ackermann** - Calculates the Ackermann function for a variety of input values.

- **anagram** - Computes anagrams for the phrases in "input.txt" using the diction in the "words". This benchmark uses code-based read-only file access for multiple files.

- **audio-codec** - Implements the A-Law compression algorithm for 16-bit PCM audio streams.

- **avl-tree** - An AVL tree implmenetation with test code.

- **banner** - Prints out a fancy vertical banner.

- **blake2b** - Reference implementation and test of BLAKE2b, a cryptographic hash function based on Daniel J. Bernstein's ChaCha stream cipher.

- **bloom-filter** - A Bloom filter implementation with test code that measures accuracy and false-positive rates.

- **boyer-moore-search** - Performs a Boyer-Moore optimized search, given a test string and a large string to search.

- **bubble-sort** - Performs a bubble sort on a randomly generated internal list of integers.

- **checkers** - Checkers game based on minimax search.

- **c-interp** - A C language interpreter that interprets the test program "hello.c". This benchmark uses code-based read-only file access.

- **cipher** - A TEA cipher implementation the encrypts and decrypts some fixed test data.

- **dhrystone** - An old-school Dhrystone benchmark.

- **distinctness** - Computes if every element of an array is unique.

- **donut** - A donut in code and action that defies proper explanation!

- **fft-int** - Performs an integer fast-Fourier-transform on fixed integer input data.

- **flood-fill** - Performs a color-based flood fill of a fixed tw-dimensional text array.

- **frac-calc** - Computes calculations on proper and improper fractions.

- **fuzzy-match** - Performs fuzzy matching of strings (e.g., slightly misspelled strings will match), with test code.

- **fy-shuffle** - A Fisher-Yates perfect random vector shuffle implementation.

- **gcd-list** - Computes the greatest common divisor for a list of integers using the division algorithm.

- **grad-descent** - Gradient descent with linear regression implementation, with test code.

- **graph-tests** - A graph data-structure manipulation library with many tests.

- **hanoi** - Solves the Tower's of Hanoi problem for a variable number of towers.

- **heapsort** - Performs a heap sort on a randomly generated data set

- **indirect-test** - A few specialized tests to validate indirect jumps, switch tables, and function returns.

- **kadane** - Implementation of Kadane's algorithm, which finds find the maximum sum of a contiguous subarray.

- **kepler** - Calculates an orbital path for a planetary body based on the Kepler parameters.

- **k-means** - A K-Means clustering algorithm running on synthetic data.

- **knapsack** - A knapsack packing algorithm running various tests.

- **knights-tour** - A dynamic programming implementation of the Knight's Tour problem (i.e., one chess knight visits all board squares).

- **life** - Conway's game of life simulation.

- **longdiv** - Computes a long division using the pencil-on-paper method.

- **lz-compress** - A compression/decompress tool based on the LZ compression algorithm.

- **mandelbrot** - Calculate and print using ASCII graphics a Mendelbrot fractal.

- **max-subseq** - Computes the longest subsequence common (LSC) to all sequences in a set of sequences using the dynamic programming method.

- **mersenne** - Generate a sequence of pseudo-random numbers using the Mersenne Twister algorithm.

- **minspan** - Finds the minimal spanning tree of a graph (via Kruscal's algorithm over the graph's adjacency matrix).

- **natlog** - Compute the value of natural log e, using an iterative method.

- **nr-solver** - Computes a square-root value using a Newton-Raphson solver.

- **parrondo** - A game theory based solver that simulates Parrondo's paradox.

- **pascal** - Compute Pascal's triangle, to a specified depth.

- **pi-calc** - An integer based high-precision PI calculator.

- **primal-test** - Performs the Miller-Rabin stochastic primarility test to extremely high certainty.

- **quine** - A C program that prints itself.

- **rabinkarp-search** - Implements the very efficient Rabin-Karp data-oblivious string search algorithm. This search algorithm is O(N) in the length of the string searched.

- **regex-parser** - A regular-expression parser running a battery of tests.

- **rho-factor** - A Pollard's Rho integer factorization algorithm.

- **rle-compress** - A run-length-encoding (RLE) compressor, with multiple tests.

- **shortest-path** - Solves the all-pairs shortest path problem using the Floyd-Warshall algorithm.

- **sieve** - Computes the prime values within a specified range, using the Sieve of Eratosthenes algorithmm

- **simple-grep** - A simplified implementation of the Unix grep command.

- **skeleton** - A minimal program, for use a starting point for new application ports and developments.

- **spelt2num** - A spelled-out number to binary number converter.

- **spirograph** - A spirograph simulation that produces a sequence of resulting data points.

- **strange** - A strange C program that acts strangely in an expected manner.

- **tiny-NN** - A deep neural net (DNN) implementation, with training and inference tests.

- **topo-sort** - Tolologically sorts a graph and prints the result in breadth-first order.

- **totient** - Calculates the Euler totient function phi.

- **vectors-3d** - A 3D vector library running a battery of tests.

- **weekday** - Given a year, month, and day, deterime the day of the week for the specified date.

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
- floating-point math functions

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
Once these four interfaces are implemented, all of the Bringup-Bench benchmarks can be built and run. To facilitate testing, the "TARGET=host" target defines the four required system interfaces by passing them on to the Linux OS. In addition, the repo also provides a standalone target "TARGET=sa" which only requires that the target support provbable memory.

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

## Hash-Alone execution

Hash-alone execution targets (e.g., hashalone-host, hashalone-spike) allow pure bare-metal benchmark execution. As such, benchmarks configured for the hash-alone targets can execution completely and verify their results with NO output or input devices. To run the benchmark, simply load its binary and jump to the start address specified in the ELF binary. When the libmin_success() interface is called, simply spin to terminate the program. At completion, the memory variable "__hashval" contains a hash signature of the output of the program as it run, since in this target mode all libtarg_putc() output goes to a FNV1a hash function. The final hash value will indicate the full output of the program. To verify the hash function, use one of the reference hash-alone targets (e.g., hashalone-host, hashalone-spike). For RISC-V targets, to debug a hash-alone output hash signature mismatch, simply use the hashalone-spike target as your golden model -- this target will run the RISC-V binaries deterministically and with the same addresses each time, so it is possible to perform a cycle-by-cycle comparisons against your design-under-test.

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

The portions of the benchmark suite that was built by the benchmark team are (C) 2021-2024 and available for use under
the [Apache License, version 2.0](https://www.apache.org/licenses/LICENSE-2.0) 

And, thanks to the respective authors of the benchmarks that were adapted for the Bringup-Bench Benchmark Suite from other efforts.


//===--- main.c -------------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "satomi.h"
#include "solver_api.h"

static satomi_t *solver;

static void satomi_usage(int status) __attribute__((noreturn));
static void exit_SIGINT(int sig_num) __attribute__((noreturn));

static void
satomi_usage(int status)
{
	if (status == EXIT_FAILURE)
		fprintf(stdout, "Try 'satomi -h' for more information\n");
	else
		fprintf(stdout, "Usage: satomi [-v] [-h] <input_file>\n\n" \
		        "Options:\n"                                   \
		        "\t-h"     "\t : display available options.\n" \
		        "\t-v"     "\t : version.\n\n");
	exit(status);
}

static void
exit_SIGINT(int sig_num)
{
	if (sig_num == SIGINT) {
		fprintf(stdout, "\n /!\\ SATOMI INTERRUPTED /!\\ \n");
	}
	exit(EXIT_SUCCESS);
}

int 
main(int argc, char **argv)
{
	int status;
	char *fname;
	char *dot;
	satomi_opts_t options;
	/* Opts parsing */
	int opt;
	extern int optind;
	extern int optopt;
	extern char* optarg;

	signal(SIGINT, exit_SIGINT);
	satomi_default_opts(&options);
	while ((opt = getopt(argc, argv, "wvh")) != -1) {
		switch (opt) {
		case 'w':
			options.verbose = 2;
			break;

		case 'v':
			fprintf(stdout, "[satomi] Version: 2.0\n");
			exit(EXIT_SUCCESS);

		case 'h':
			satomi_usage(EXIT_SUCCESS);

		default:
			break;
		}
	}
	if (optind == argc)
		satomi_usage(EXIT_FAILURE);

	fprintf(stdout, "[satomi] Version: 2.0\n");

	fname = strdup(argv[optind]);
	if ((dot = strrchr(fname, '.')) == NULL) {
		fprintf(stderr, "[satomi] Unrecognized file format.\n");
		return 1;
	}
	if (strcmp(dot, ".cnf")) {
		fprintf(stderr, "[satomi] Unsupported file format: %s\n", dot);
		return 1;
	}

	satomi_parse_dimacs(fname, &solver);
	satomi_configure(solver, &options);
	status = satomi_solve(solver);
	if (1)
		satomi_print_stats(solver);
	if (status == SATOMI_UNDEC)
		fprintf(stdout, "UNDECIDED    \n");
	else if (status == SATOMI_SAT)
		fprintf(stdout, "SATISFIABLE  \n");
	else
		fprintf(stdout, "UNSATISFIABLE\n");
	satomi_destroy(solver);
}


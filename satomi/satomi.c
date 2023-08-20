//===--- main.c -------------------------------------------------------------===
//
//                     satomi: Satisfiability solver
//
// This file is distributed under the BSD 2-Clause License.
// See LICENSE for details.
//
//===------------------------------------------------------------------------===
#include "libmin.h"
#include "satomi.h"
#include "solver_api.h"

static satomi_t *solver;

static void
satomi_usage(int status)
{
	if (status == EXIT_FAILURE)
		libmin_printf("Try 'satomi -h' for more information\n");
	else
		libmin_printf("Usage: satomi [-v] [-h] <input_file>\n\n" \
		        "Options:\n"                                   \
		        "\t-h"     "\t : display available options.\n" \
		        "\t-v"     "\t : version.\n\n");
	libmin_fail(status);
}

#include "cnffile.h"
MFILE __mcnffile = {
  "zebra_v155_c1135.cnf",
  __cnffile_sz,
  __cnffile,
  0
};
MFILE *mcnffile = &__mcnffile;


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

	satomi_default_opts(&options);
	while ((opt = libmin_getopt(argc, argv, "wvh")) != -1) {
		switch (opt) {
		case 'w':
			options.verbose = 2;
			break;

		case 'v':
			libmin_printf("[satomi] Version: 2.0\n");
			libmin_success();

		case 'h':
			satomi_usage(EXIT_SUCCESS);

		default:
			break;
		}
	}
	if (optind == argc)
		satomi_usage(EXIT_FAILURE);

	libmin_printf("[satomi] Version: 2.0\n");

	fname = libmin_strdup(argv[optind]);
	if ((dot = libmin_strrchr(fname, '.')) == NULL) {
		libmin_printf("[satomi] Unrecognized file format.\n");
		return 1;
	}
	if (libmin_strcmp(dot, ".cnf")) {
		libmin_printf("[satomi] Unsupported file format: %s\n", dot);
		return 1;
	}

	satomi_parse_dimacs(mcnffile, &solver);
	satomi_configure(solver, &options);
	status = satomi_solve(solver);
	if (1)
		satomi_print_stats(solver);
	if (status == SATOMI_UNDEC)
		libmin_printf("UNDECIDED    \n");
	else if (status == SATOMI_SAT)
		libmin_printf("SATISFIABLE  \n");
	else
		libmin_printf("UNSATISFIABLE\n");
	satomi_destroy(solver);
}


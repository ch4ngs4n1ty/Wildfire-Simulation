// file: wildfire.c
// wildfire.c fire game
// author: Ethan Chang



#include <stdio.h>
#include <stdlib.h>


#include <getopt.h>

static void layout() {

	fprintf(stderr, "\n");
	fprintf(stderr, "Simulation Configuration Options\n");
	fprintf(stderr, " -H  # View simulation options and quit.\n");
	fprintf(stderr, " -bN # proportion of trees that are already burning. 0 < N < 101.\n");
	fprintf(stderr, " -cN # probability that a tree will catch fire. 0 < N < 101.\n");
	fprintf(stderr, " -dN # density/proportion of trees in the grid. 0 < N < 101.\n");
	fprintf(stderr, " -nN # proportion of neighbors that influence a tree catching fire. -1 < N < 101.\n");
	fprintf(stderr, " -pN # number of states to print before quitting. -1 < N < ...\n");
	fprintf(stderr, " -sN # simulation grid size. 4 < n < 41.\n");
	fprintf(stderr, "\n");

}

int main(int argc, char * argv[]) {

	int opt;
	int tmpsize = 0;


	for (int i = 0; i < argc; ++i) {

		fprintf(stderr, "%s ", argv[i]);

	}

	fprintf(stderr, "\n");



	while ( ( opt = getopt( argc, argv, "Hs:b:c:d:n:p:s") ) != -1) {

		switch (opt) {

		case 'H':

			fprintf(stderr, "usage: wildfire [options]\n");
			fprintf(stderr, "By default, the simulation runs in overlay display mode.\n");
			fprintf(stderr, "The -pN option makes the simulation run in print mode to print N states.\n");

			layout();


			break;

		case 'b':

			tmpsize = (int) strtol(optarg, NULL, 10);
			fprintf(stderr, "%d\n" , tmpsize);



		}

	}

}



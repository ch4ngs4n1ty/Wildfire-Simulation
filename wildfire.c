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

	int b_chance = 10; //inital value of burn percent chance
	int c_chance = 30; //initial value of tree catching fire percent chance
	int d_chance = 50; //initial value of density of the forest percent thance
	int n_chance = 25; //initial value of neighbor effect that tree might catch fire
	int p_mode = 0; //number of runs to display
//	int o_mode = 1; //default mode, overlay mode
	int s_size = 10; //default size of the grid


	for (int i = 0; i < argc; ++i) {

		fprintf(stderr, "%s ", argv[i]);

	}

	fprintf(stderr, "\n");



	while ( ( opt = getopt( argc, argv, "Hs:b:c:d:n:p:s") ) != -1) {

		switch (opt) {

		case 'H': // outputs the Simulation Configuration Options

			fprintf(stderr, "usage: wildfire [options]\n");
			fprintf(stderr, "By default, the simulation runs in overlay display mode.\n");
			fprintf(stderr, "The -pN option makes the simulation run in print mode to print N states.\n");

			layout();


			break;

		case 'b': // gets grid population that is burning at start of simulataion

			tmpsize = (int) strtol(optarg, NULL, 10);
			b_chance = tmpsize;

			if (b_chance < 1 || b_chance > 100) {

				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

			fprintf(stderr, "%d\n" , b_chance);

			break;


		case 'c': // probability of tree catching fire, percentage chance

			tmpsize = (int) strtol(optarg, NULL, 10);
			c_chance = tmpsize;

			if (c_chance < 1 || c_chance > 100) {

				exit(EXIT_FAILURE);

			}

			fprintf(stderr, "%d\n" , c_chance);

			break;


		case 'd':
			tmpsize = (int) strtol(optarg, NULL, 10);
			d_chance = tmpsize;

			if (d_chance < 1 || d_chance > 100) {

				exit(EXIT_FAILURE);
			}

			tmpsize = 0;

			fprintf(stderr, "%d\n" , d_chance);

			break;

		case 'n':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 0 && tmpsize <= 100) {

				n_chance = tmpsize;


			} else {

				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

			fprintf(stderr, "%d\n" , n_chance);

			break;

		case 'p':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (p_mode >= 0) {

				p_mode = tmpsize;

			} else {

				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

			fprintf(stderr, "%d\n" , p_mode);

			break;

		case 's':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 5 && tmpsize <= 40) {

				s_size = tmpsize;

			} else {

				exit(EXIT_FAILURE);

			}

			fprintf(stderr, "%d\n" , s_size);

			break;


		default:

			exit(EXIT_FAILURE);

		}

	}

}



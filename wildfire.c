// file: wildfire.c
// wildfire.c fire game
// author: Ethan Chang

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>
#include <time.h>
#include <unistd.h>

#define EMPTY ' '
#define TREE 'Y'
#define BURNING '*'


static int b_chance = 10; //inital value of burn percent chance
static int c_chance = 30; //initial value of tree catching fire percent chance
static int d_chance = 50; //initial value of density of the forest percent thance
static int n_chance = 25; //initial value of neighbor effect that tree might catch fire
static int p_mode = 0; //number of runs to display
//	int o_mode = 1; //default mode, overlay mode
static int s_size = 10; //default size of the grid


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

static void command_parse(int argc, char *argv[]) {

	int opt;
	int tmpsize = 0;

	while ( ( opt = getopt( argc, argv, "Hs:b:c:d:n:p:s") ) != -1) {

		switch (opt) {

		case 'H': // outputs the Simulation Configuration Options

			fprintf(stderr, "usage: wildfire [options]\n");
			fprintf(stderr, "By default, the simulation runs in overlay display mode.\n");
			fprintf(stderr, "The -pN option makes the simulation run in print mode to print N states.\n");

			layout();

			exit(0); //stops running after help layout

			break;

		case 'b': // gets grid population that is burning at start of simulataion

			tmpsize = (int) strtol(optarg, NULL, 10);
			b_chance = tmpsize;

			if (b_chance < 1 || b_chance > 100) {

				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

//			fprintf(stderr, "%d\n" , b_chance);

			break;


		case 'c': // probability of tree catching fire, percentage chance

			tmpsize = (int) strtol(optarg, NULL, 10);
			c_chance = tmpsize;

			if (c_chance < 1 || c_chance > 100) {

				exit(EXIT_FAILURE);

			}

//			fprintf(stderr, "%d\n" , c_chance);

			break;


		case 'd':
			tmpsize = (int) strtol(optarg, NULL, 10);
			d_chance = tmpsize;

			if (d_chance < 1 || d_chance > 100) {

				exit(EXIT_FAILURE);
			}

			tmpsize = 0;

//			fprintf(stderr, "%d\n" , d_chance);

			break;

		case 'n':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 0 && tmpsize <= 100) {

				n_chance = tmpsize;

			} else {

				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

//			fprintf(stderr, "%d\n" , n_chance);

			break;

		case 'p':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (p_mode >= 0) {

				p_mode = tmpsize;

			} else {

				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

//			fprintf(stderr, "%d\n" , p_mode);

			break;

		case 's':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 5 && tmpsize <= 40) {

				s_size = tmpsize;

			} else {

				exit(EXIT_FAILURE);

			}

//			fprintf(stderr, "%d\n" , s_size);

			break;


		default:

			exit(EXIT_FAILURE);

		}
	}
}



void fy_shuffle(int *cells, int n) {

	for (int i = n - 1; i > 0; --i) {

		int j = rand() % (i + 1);

		int temp = cells[i];
		cells[i] = cells[j];
		cells[j] = temp;

	}
}


void start_grid(char **grid, int size, int d_chance, int b_chance) {

	srand(41); //random number generator

	printf("%d, %d \n", d_chance, b_chance);

	float d_prob = (float) d_chance/100;
	float b_prob = (float) b_chance/100;

	printf("%f, %f \n", d_prob, b_prob);

	int total_cell = size * size; //gets total number of cells in a grid

	//float d_prob = d_chance/100


	int treeT_num = (int)(d_prob * total_cell);

	int burnT_num = (int)(b_prob * treeT_num);

	int aliveT_num = treeT_num - burnT_num;

	int totalT_num = treeT_num;

//	int empty_cell = total_cell - t_num - b_num;

	for (int r = 0; r < size; ++r) {

		for (int c = 0; c < size; ++c) {

			grid[r][c] = EMPTY;

		}
	}

	int *cells = malloc(total_cell * sizeof(int)); //items representing all locations in grid

	for (int i = 0; i < total_cell; ++i) {

		cells[i] = i;

	}

	fy_shuffle(cells, total_cell);

	//printf("total cell %d, Tree: %d, Burn: %d\n" , total_cell, t_num, b_num);

	for (int i = 0; i < total_cell; i++) {

		int loc = cells[i];
		int row = loc / size;
		int col = loc % size;

		if (i < aliveT_num && grid[row][col] == EMPTY) {

			grid[row][col] = TREE;

		} else if (i < totalT_num && grid[row][col] == EMPTY) {

			grid[row][col] = BURNING;

		}
	}


	free(cells); //free allocated memory of cells

}


void display_grid(char **grid, int size) {

	for (int r = 0; r < size; r++) {

		for (int c = 0; c < size; c++) {

			printf("%c", grid[r][c]);

		}

		printf("\n");
	}

}

int main(int argc, char *argv[]) {

	command_parse(argc, argv);

	char **grid = malloc(s_size * sizeof(char*));

	for (int i = 0; i < s_size; i++) {

		grid[i] = malloc(s_size * sizeof(char));

	}

	//printf("%d, %d \n" , d_chance, b_chance);
	start_grid(grid, s_size, d_chance, b_chance);
	display_grid(grid, s_size);

	for (int i = 0; i < s_size; i++) {

		free(grid[i]);

	}

	free(grid);

}



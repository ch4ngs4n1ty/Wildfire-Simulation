// file: wildfire.c
// wildfire.c fire game
// author: Ethan Chang

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>
#include <time.h>
#include <unistd.h>

#include "display.h"

#define MAX_GRID 40 // maximum cell size grid

#define EMPTY ' '
#define TREE 'Y'
#define BURNING '*'
#define BURNED '.'

static int b_chance = 10; //inital value of burn percent chance
static int c_chance = 30; //initial value of tree catching fire percent chance
static int d_chance = 50; //initial value of density of the forest percent thance
static int n_chance = 25; //initial value of neighbor effect that tree might catch fire
static int p_mode = -1; //number of runs to display
//	int o_mode = 1; //default mode, overlay mode
static int s_size = 10; //default size of the grid

int counter[MAX_GRID][MAX_GRID];


void display_grid(char grid[MAX_GRID][MAX_GRID], int size, int c_chance, int d_chance, int b_chance, int n_chance) {

	for (int r = 0; r < size; r++) {

		for (int c = 0; c < size; c++) {

			printf("%c", grid[r][c]);

		}

		printf("\n");
	}

	float pCatch = (float) c_chance / 100.0;
	float density = (float) d_chance / 100.0;
	float pBurning = (float) b_chance / 100.0;
	float pNeighbor = (float) n_chance / 100.0;

	printf("size %d, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor %.2f \n", size, pCatch, density, pBurning, pNeighbor);
}


int spread(char grid[MAX_GRID][MAX_GRID], int n_chance, int c_chance, int row, int col, int *real_total_nbr, int *real_burn_nbr) {

	int total_nbr = 0;
	int burn_nbr = 0;

	int top = row - 1;
	int bottom = row + 1;
	int right = col + 1;
	int left = col - 1;

	if (grid[top][left] == TREE || grid[top][left] == BURNING) {

    		total_nbr++;

    		if (grid[top][left] == BURNING) {

        		burn_nbr++;
	    	}
	}

	if (grid[top][col] == TREE || grid[top][col] == BURNING) {

    		total_nbr++;

		if (grid[top][col] == BURNING) {

			burn_nbr++;
    		}
	}

	if (grid[top][right] == TREE || grid[top][right] == BURNING) {

	   total_nbr++;

	    if (grid[top][right] == BURNING) {

	        burn_nbr++;

	    }

	}

	if (grid[row][left] == TREE || grid[row][left] == BURNING) {

	    total_nbr++;

	    if (grid[row][left] == BURNING) {

	        burn_nbr++;

	    }

	}

	if (grid[row][right] == TREE || grid[row][right] == BURNING) {

	   total_nbr++;

	    if (grid[row][right] == BURNING) {

	        burn_nbr++;

	    }

	}

	if (grid[bottom][left] == TREE || grid[bottom][left] == BURNING) {

	    total_nbr++;

	    if (grid[bottom][left] == BURNING) {

	        burn_nbr++;

	    }

	}

	if (grid[bottom][col] == TREE || grid[bottom][col] == BURNING) {

	    total_nbr++;

	    if (grid[bottom][col] == BURNING) {

	       burn_nbr++;

	    }

	}

	if (grid[bottom][right] == TREE || grid[bottom][right] == BURNING) {

	    total_nbr++;

	    if (grid[bottom][right] == BURNING) {

	        burn_nbr++;

	    }

	}

//	printf("%d, %d\n" , total_nbr, burn_nbr);

	*real_total_nbr = total_nbr;
	*real_burn_nbr = burn_nbr;

//	printf("%d, %d \n", *real_total_nbr, *real_burn_nbr);

//	printf("%d total, %d burning \n", *total_nbr, *burn_nbr);

	if (*real_total_nbr > 0) {

		float proportion_nbr = (float) *real_burn_nbr / *real_total_nbr;

		float n_prob = (float) n_chance / 100;

//		printf("%.2f, %.2f \n", proportion_nbr, n_prob);

		if (proportion_nbr > n_prob) {

			float rand_val = (float) rand() / RAND_MAX;

			float c_prob = (float) c_chance / 100;

//			printf("%.2f randomval, %.2f c probability \n", rand_val, c_prob);

//			printf("\n");

			if (rand_val < c_prob) {

		//		printf("%.2f, %.2f \n", rand_val, c_prob);

				return 1;

			}

		}
	}

	return 0;

}



void update_grid(char grid[MAX_GRID][MAX_GRID], int size, int c_chance, int n_chance) {

	char copy_grid[MAX_GRID][MAX_GRID];

	for (int row = 0; row < size; row++) {

		for(int col = 0; col < size; col++) {

			copy_grid[row][col] = grid[row][col];

		}

	}

	for (int row = 0; row < size; row++) {

		for (int col = 0; col < size; col++) {

			char character = grid[row][col];

			if (character == EMPTY) {

				copy_grid[row][col] = EMPTY;

			} else if (character == TREE) {


				int total_nbr = 0;

				int burn_nbr = 0;

				if (spread(grid, n_chance, c_chance, row, col, &total_nbr, &burn_nbr)) {

					copy_grid[row][col] = BURNING;
					counter[row][col] = 0;

				}

			} else if (character == BURNING) {

				counter[row][col]++;

				if (counter[row][col] < 4) {

					copy_grid[row][col] = BURNING;
					counter[row][col]++;

				} else {

					copy_grid[row][col] = BURNED;

				}


			} else if (character == BURNED) {

				copy_grid[row][col] = BURNED;

			}
		}
	}

	for (int row = 0; row < size; row++) {

		for (int col = 0; col < size; col++) {

			grid[row][col] = copy_grid[row][col];

		}

	}

}

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

static void p_header(int step) {

	printf("===========================\n");
	printf("======== Wildfire =========\n");
	printf("===========================\n");
	printf("=== Print %d  Time Steps ===\n", step);
	printf("===========================\n");

}

void o_mode(char grid[MAX_GRID][MAX_GRID], int size, int c_chance, int d_chance, int b_chance, int n_chance) {

	//display_grid(grid, s_size, c_chance, d_chance, b_chance, n_chance);

	clear();

	update_grid(grid, size, c_chance, n_chance);

	set_cur_pos(1, 1);

	for(int row = 0; row < size; row++) {

		for (int col = 0; col < size; col++) {

			set_cur_pos(row + 1, col + 1);
			put(grid[row][col]);

		}

		//put('\n');

	//	set_cur_pos(1,1);

	}

	//display_grid(grid, s_size, c_chance, d_chance, b_chance, n_chance);

	set_cur_pos(size + 1, 1);

	float pCatch = (float) c_chance / 100.0;
	float density = (float) d_chance / 100.0;
	float pBurning = (float) b_chance / 100.0;
	float pNeighbor = (float) n_chance / 100.0;

	printf("size %d, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor %.2f \n", size, pCatch, density, pBurning, pNeighbor);

	usleep(75000);

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

			break;


		case 'c': // probability of tree catching fire, percentage chance

			tmpsize = (int) strtol(optarg, NULL, 10);
			c_chance = tmpsize;

			if (c_chance < 1 || c_chance > 100) {

				exit(EXIT_FAILURE);

			}

			break;


		case 'd':
			tmpsize = (int) strtol(optarg, NULL, 10);
			d_chance = tmpsize;

			if (d_chance < 1 || d_chance > 100) {

				exit(EXIT_FAILURE);
			}

			tmpsize = 0;

			break;

		case 'n':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 0 && tmpsize <= 100) {

				n_chance = tmpsize;

			} else {

				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

			break;

		case 'p':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (p_mode >= 0) {

				p_mode = tmpsize;

			} else {

				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

			break;

		case 's':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 5 && tmpsize <= MAX_GRID) {

				s_size = tmpsize;

			} else {

				exit(EXIT_FAILURE);

			}

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

void initialize_grid(char grid[MAX_GRID][MAX_GRID], int size, int d_chance, int b_chance) {

	srand(41); //random number generator

	float d_prob = (float) d_chance/100; // percentage chance of density

	float b_prob = (float) b_chance/100; // percentage chance of burning

	int total_cell = size * size; //gets total number of cells in a grid

	int treeT_num = (int)(d_prob * total_cell);

	int burnT_num = (int)(b_prob * treeT_num);

	//int totalT_num = treeT_num;

	for (int r = 0; r < size; ++r) {

		for (int c = 0; c < size; ++c) {

			grid[r][c] = EMPTY;

		}
	}

	int cells[MAX_GRID * MAX_GRID]; // max cell array

	for (int i = 0; i < total_cell; ++i) {

		cells[i] = i;

	}

	fy_shuffle(cells, total_cell);


	//Sets up burning characters in the grid

	for (int i = 0; i <= treeT_num; i++) {

		int loc = cells[i];
		int row = loc / size;
		int col = loc % size;

		grid[row][col] = TREE;

	}

	//Sets up tree characters in the grid

	for (int i = 0; i < burnT_num; i++) {

		int loc = cells[i];
		int row = loc / size;
		int col = loc % size;

		grid[row][col] = BURNING;

	}

}
/**
void initialize_grid(char grid[MAX_GRID][MAX_GRID], int size) {
    // Manually initialize the grid based on your example
    char temp_grid[MAX_GRID][MAX_GRID] = {
        {' ', ' ', 'Y', 'Y', ' '},
        {' ', ' ', 'Y', 'Y', 'Y'},
        {'Y', 'Y', '*', 'Y', ' '},
        {' ', 'Y', ' ', 'Y', ' '},
        {' ', 'Y', ' ', ' ', 'Y'}
    };

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            grid[row][col] = temp_grid[row][col];
        }
    }
}

*/

int fire_checker(char grid[MAX_GRID][MAX_GRID], int size) {

	for (int row = 0; row < size; row++) {

		for (int col = 0; col < size; col++) {

			char character = grid[row][col];

			if (character == BURNING) {

				return 1;

			}
		}
	}

	return 0;
}

int main(int argc, char *argv[]) {

	srand(41);

	int cycle = 0;

	/*
	int cur_change = 0;
	int cum_change = 0;
	*/


	command_parse(argc, argv);

	char grid[MAX_GRID][MAX_GRID];

	printf("%d\n" , p_mode);

	//start_grid(grid, s_size, d_chance, b_chance);

	initialize_grid(grid, s_size, d_chance, b_chance);

	while  (p_mode == -1) {

//		o_mode(grid, s_size, c_chance, d_chance, b_chance, n_chance);

		if (fire_checker(grid, s_size)) {

			//set_cur_pos(1, 1);

			o_mode(grid, s_size, c_chance, d_chance, b_chance, n_chance);

			//update_grid(grid, s_size, c_chance, n_chance);

		} else {

			printf("Fires are out\n");

			break;

		}
	}

	while (cycle < p_mode) {

		if (p_mode > 0 ) {

			if (cycle == 0) {

				p_header(p_mode);

			}

			display_grid(grid, s_size, c_chance, d_chance, b_chance, n_chance);

		} else {

			display_grid(grid, s_size, c_chance, d_chance, b_chance, n_chance);

		}

		update_grid(grid, s_size, c_chance, n_chance);

		if (fire_checker(grid, s_size)) {

//			update_grid(grid, s_size, c_chance, n_chance);
			continue;

		} else {

			display_grid(grid, s_size, c_chance, d_chance, b_chance, n_chance);

			printf("Fires are out\n");

			break;
		}

//		display_grid(grid, s_size, c_chance, d_chance, b_chance, n_chance);

		cycle++;

	}

	return 0;
}



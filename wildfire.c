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
static int p_mode = 0; //number of runs to display
//	int o_mode = 1; //default mode, overlay mode
static int s_size = 10; //default size of the grid

int counter[MAX_GRID][MAX_GRID];


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

void display_grid(char grid[MAX_GRID][MAX_GRID], int size, int c_chance, int d_chance, int b_chance, int n_chance, int cycle, int current_change, int cumulative_change) {

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
	printf("cycle %d, current changes %d, cumulative changes %d.\n", cycle, current_change, cumulative_change);

}


void update_grid(char grid[MAX_GRID][MAX_GRID], int size, int c_chance, int n_chance, int *current_change) {

	*current_change = 0;

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
					(*current_change)++;

				}

			} else if (character == BURNING) {

				counter[row][col]++;

				if (counter[row][col] < 4) {

					copy_grid[row][col] = BURNING;
					counter[row][col]++;

				} else {

					copy_grid[row][col] = BURNED;
					(*current_change)++;

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

void o_mode(char grid[MAX_GRID][MAX_GRID], int size, int c_chance, int d_chance, int b_chance, int n_chance, int cycle, int current_change, int cumulative_change) {

	set_cur_pos(1, 0);

	for(int row = 0; row < size; row++) {

		for (int col = 0; col < size; col++) {

			put(grid[row][col]);

		}

		put('\n');

	}

	set_cur_pos(size + 1, 0);

	float pCatch = (float) c_chance / 100.0;
	float density = (float) d_chance / 100.0;
	float pBurning = (float) b_chance / 100.0;
	float pNeighbor = (float) n_chance / 100.0;

	printf("size %d, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor %.2f \n", size, pCatch, density, pBurning, pNeighbor);
	printf("cycle %d, current changes %d, cumulative changes %d.\n", cycle, current_change, cumulative_change);

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

	float d_prob = (float) d_chance/100; // percentage chance of density

	float b_prob = (float) b_chance/100; // percentage chance of burning

	int total_cell = size * size; //gets total number of cells in a grid

	int treeT_num = (int)(d_prob * total_cell);

	int burnT_num = (int)(b_prob * treeT_num);

	int cells[total_cell]; // max cell array

	 for (int i = 0; i < total_cell; i++) {

		if (i < burnT_num) {

			cells[i] = BURNING;

		} else if (i < treeT_num) {

			cells[i] = TREE;

		} else {

			cells[i] = EMPTY;

		}

    	}

    	fy_shuffle(cells, total_cell);

	int i = 0;

    	for (int r = 0; r < size; r++) {

		for (int c = 0; c < size; c++) {

			grid[r][c] = cells[i++];

		}

	}

}

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

	int current_change = 0;
	int cumulative_change = 0;

	command_parse(argc, argv);

	char grid[MAX_GRID][MAX_GRID];

	if (p_mode > 0) {


		if (cycle == 0) {

			p_header(p_mode);

			initialize_grid(grid, s_size, d_chance, b_chance);

			display_grid(grid, s_size, c_chance, d_chance, b_chance, n_chance, cycle, current_change, cumulative_change);

        		update_grid(grid, s_size, c_chance, n_chance, &current_change);

			cumulative_change += current_change;

		}

		for (int i = 0; i < p_mode; i++) {

			display_grid(grid, s_size, c_chance, d_chance, b_chance, n_chance, cycle, current_change, cumulative_change);

        		update_grid(grid, s_size, c_chance, n_chance, &current_change);

			cumulative_change += current_change;

	        	if (!fire_checker(grid, s_size)) {

				cycle++;

				//cumulative_change += current_change;

				display_grid(grid, s_size, c_chance, d_chance, b_chance, n_chance, cycle, current_change, cumulative_change);

        	    		printf("Fires are out\n");
	            		break;
        		}


			cycle++;

		}

	} else {

		initialize_grid(grid, s_size, d_chance, b_chance);

		clear();

		while (1) {

			set_cur_pos(1, 0);

			o_mode(grid, s_size, c_chance, d_chance, b_chance, n_chance, cycle, current_change, cumulative_change);

//		        o_mode(grid, s_size, c_chance, d_chance, b_chance, n_chance);

		        usleep(750000);

		        update_grid(grid, s_size, c_chance, n_chance, &current_change);

			cumulative_change += current_change;

		        if (!fire_checker(grid, s_size)) {

				o_mode(grid, s_size, c_chance, d_chance, b_chance, n_chance, cycle, current_change, cumulative_change);

//				o_mode(grid, s_size, c_chance, d_chance, b_chance, n_chance);

		        	printf("Fires are out\n");

				break;

        		}

			cycle++;

		}
	}

	return 0;

}



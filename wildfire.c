// file: wildfire.c
// decsription: takes the inputs from users and starts the fire simulation
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
static int s_size = 10; //default size of the grid

int counter[MAX_GRID][MAX_GRID]; //used to count seconds for each burning tree

/**
* Decides whether this cell gets to be burned in fire or not
*
* @param grid: current grid of containing current cells
* @param n_chance: neighbor effect chance of tree catching fire
* @param c_chance: chance of tree catching fire
* @param row: the row of the cell
* @param col: the col of the cell
* @param total_nbr: number of total neighbors
* @param burn_nbr: number of burning neighbors
*
* @return: 1 if the tree is burnable, 0 if the tree is not burnable
*/
int spread(char grid[MAX_GRID][MAX_GRID], int n_chance, int c_chance, int row, int col, int *total_nbr, int *burn_nbr) {

	int total_count = 0;
	int burn_count = 0;

	int top = row - 1; //top cell
	int bottom = row + 1; //bottom cell
	int right = col + 1; //right cell
	int left = col - 1; //left cell

	if (grid[top][left] == TREE || grid[top][left] == BURNING) { //checks the top left coordinate

    		total_count++;

    		if (grid[top][left] == BURNING) {

        		burn_count++;
	    	}
	}

	if (grid[top][col] == TREE || grid[top][col] == BURNING) { //checks the top column coordinate

    		total_count++;

		if (grid[top][col] == BURNING) {

			burn_count++;
    		}
	}

	if (grid[top][right] == TREE || grid[top][right] == BURNING) { //checks the top right coordinate

		total_count++;

		if (grid[top][right] == BURNING) {

	      		burn_count++;

	    	}

	}

	if (grid[row][left] == TREE || grid[row][left] == BURNING) { //checks the row left coordinate

		total_count++;

		if (grid[row][left] == BURNING) {

	        	burn_count++;

	    	}

	}

	if (grid[row][right] == TREE || grid[row][right] == BURNING) { //checks the row right coordinate

		total_count++;

		if (grid[row][right] == BURNING) {

	        	burn_count++;

	    	}

	}

	if (grid[bottom][left] == TREE || grid[bottom][left] == BURNING) { //checks the bottom left coordinate

		total_count++;

		if (grid[bottom][left] == BURNING) {

	        	burn_count++;

	    	}

	}

	if (grid[bottom][col] == TREE || grid[bottom][col] == BURNING) { //checks the bottom col coordinate

		total_count++;

		if (grid[bottom][col] == BURNING) {

	     		burn_count++;

	    	}

	}

	if (grid[bottom][right] == TREE || grid[bottom][right] == BURNING) { //checks the bottom right coordinate

		total_count++;

		if (grid[bottom][right] == BURNING) {

	        	burn_count++;

	    	}

	}

	*total_nbr = total_count; //stores total count of neighbors into the pointer so we track the values
	*burn_nbr = burn_count; //store burn count of neighbors into the pointer so we track the values

	if (*total_nbr > 0) {

		float proportion_nbr = (float) *burn_nbr / *total_nbr; //gets the proportion of neighbors from dividing burning with total neighbors

		float n_prob = (float) n_chance / 100; // neighbor probability chance

		if (proportion_nbr > n_prob) { //if proportion is greater than neighbor percent chance, then we generate a random value

			float rand_val = (float) rand() / RAND_MAX;

			float c_prob = (float) c_chance / 100; //catch fire probability chance

			if (rand_val < c_prob) { //if the random value we generated is less than catch fire probability chance, then the tree starts burning

				return 1;

			}
		}
	}

	return 0;

}

/**
* Gives the output and list of commands to help.
*/
static void layout() {

	fprintf(stderr, "usage: wildfire [options]\n");
	fprintf(stderr, "By default, the simulation runs in overlay display mode.\n");
	fprintf(stderr, "The -pN option makes the simulation run in print mode for up to N states.\n");

	fprintf(stderr, "\n");
	fprintf(stderr, "Simulation Configuration Options:\n");
	fprintf(stderr, " -H  # View simulation options and quit.\n");
	fprintf(stderr, " -bN # proportion of trees that are already burning. 0 < N < 101.\n");
	fprintf(stderr, " -cN # probability that a tree will catch fire. 0 < N < 101.\n");
	fprintf(stderr, " -dN # density: the proportion of trees in the grid. 0 < N < 101.\n");
	fprintf(stderr, " -nN # proportion of neighbors that influence a tree catching fire. -1 < N < 101.\n");
	fprintf(stderr, " -pN # number of states to print before quitting. -1 < N < ...\n");
	fprintf(stderr, " -sN # simulation grid size. 4 < N < 41.\n");
	fprintf(stderr, "\n");

}

static void p_header(int step) {

	printf("===========================\n");
	printf("======== Wildfire =========\n");
	printf("===========================\n");
	printf("=== Print %d Time Steps ===\n", step);
	printf("===========================\n");

}


/*
* Gets the user inputs and stores value while command parsing
*
*
*
*/

static void command_parse(int argc, char *argv[]) {

	int opt;
	int tmpsize = 0;

	while ( ( opt = getopt( argc, argv, "Hs:b:c:d:n:p:s") ) != -1) {

		switch (opt) {

		case 'H': // outputs the Simulation Configuration Options

			layout();

			exit(0); //stops running after help layout

			break;

		case 'b': // gets grid population that is burning at start of simulataion

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 1 && tmpsize <= 100) {

				b_chance = tmpsize;

			} else {

				fprintf(stderr, "(-bN) proportion already burning must be an integer in [1...100].\n");
				layout();
				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

			break;


		case 'c': // probability of tree catching fire, percentage chance

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 1 && tmpsize <= 100) {

				c_chance = tmpsize;

			} else {

				fprintf(stderr, "(-cN) probability a tree will catch fire must be an integer in [1...100].\n");
				layout();
				exit(EXIT_FAILURE);

			}

			break;


		case 'd':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 1 && tmpsize <= 100) {

				d_chance = tmpsize;

			} else {

				fprintf(stderr, "(-dN) density of trees in the grid must be an integer in [1...100].\n");
				layout();
				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

			break;

		case 'n':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 0 && tmpsize <= 100) {

				n_chance = tmpsize;

			} else {

				fprintf(stderr, "(-nN) %%neighbors influence catching fire must be an integer in [0...100].\n");
				layout();
				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

			break;

		case 'p':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 0) {

				p_mode = tmpsize;

			} else {

				fprintf(stderr, "(-pN) number of states to print must be an integer in [0...10000].\n");
				layout();
				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

			break;

		case 's':

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 5 && tmpsize <= MAX_GRID) {

				s_size = tmpsize;

			} else {

				fprintf(stderr, "(-sN) simulation grid size must be an integer in [5...40].\n");
				layout();
				exit(EXIT_FAILURE);

			}

			break;

		default:

			break;

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

		} else if (i <= treeT_num) {

			cells[i] = TREE;

		} else {

			cells[i] = EMPTY;

		}

    	}

    	fy_shuffle(cells, total_cell);

	int cell = 0;

    	for (int r = 0; r < size; r++) {

		for (int c = 0; c < size; c++) {

			grid[r][c] = cells[cell++];

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

		        usleep(750000);

		        update_grid(grid, s_size, c_chance, n_chance, &current_change);

			cumulative_change += current_change;

		        if (!fire_checker(grid, s_size)) {

				o_mode(grid, s_size, c_chance, d_chance, b_chance, n_chance, cycle, current_change, cumulative_change);

		        	printf("Fires are out\n");

				break;

        		}

			cycle++;

		}
	}

	return 0;

}



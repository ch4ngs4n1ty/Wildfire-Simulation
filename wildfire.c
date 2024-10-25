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

#define MAX_GRID 40 ///< maximum cell size grid

#define MIN 1 ///< minimum value
#define MAX 100 ///< maximum value

#define EMPTY ' ' ///empty character
#define TREE 'Y' ///tree character
#define BURNING '*' //fire character
#define BURNED '.' //burned character

char grid[MAX_GRID][MAX_GRID]; //the grid

char copy_grid[MAX_GRID][MAX_GRID]; //the copy grid

int counter[MAX_GRID][MAX_GRID]; //used to count seconds for each burning tree, stores number of seconds for each cell


static int b_chance = 10; //inital value of burn percent chance
static int c_chance = 30; //initial value of tree catching fire percent chance
static int d_chance = 50; //initial value of density of the forest percent thance
static int n_chance = 25; //initial value of neighbor effect that tree might catch fire
static int p_mode = 0; //number of runs to display
static int s_size = 10; //default size of the grid

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

/**
* When the user calls for print mode, the function will output this first before displaying the grids
*/
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
* @param argc: the number of arguements that's being passed from command line
* @param argv: array of characters that has arguements from command line
*
* @return: the updated values the user inputted in command line
*/
static void command_parse(int argc, char *argv[]) {

	int opt;
	int tmpsize = 0;

	while ( ( opt = getopt( argc, argv, "Hs:b:c:d:n:p:s") ) != -1) { //loops the user inputs

		switch (opt) {

		case 'H': // outputs the Simulation Configuration Options

			layout();

			exit(0); //stops running after help layout

			break;

		case 'b': // gets grid population that is burning at start of simulataion

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= MIN && tmpsize <= MAX) {

				b_chance = tmpsize;

			} else {

				fprintf(stderr, "(-bN) proportion already burning must be an integer in [1...100].\n");
				layout();
				exit(EXIT_FAILURE);

			}

			tmpsize = 0;

			break;


		case 'c': //probability of tree catching fire, percentage chance

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 1 && tmpsize <= 100) {

				c_chance = tmpsize;

			} else {

				fprintf(stderr, "(-cN) probability a tree will catch fire must be an integer in [1...100].\n");
				layout();
				exit(EXIT_FAILURE);

			}

			break;


		case 'd': //percent of the density trees

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

		case 'n': //neighbor percent influence with catching fire

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

		case 'p': //a number of states that's being printed

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

		case 's': //number of the grid size

			tmpsize = (int) strtol(optarg, NULL, 10);

			if (tmpsize >= 5 && tmpsize <= MAX_GRID) {

				s_size = tmpsize;

			} else {

				fprintf(stderr, "(-sN) simulation grid size must be an integer in [5...40].\n");
				layout();
				exit(EXIT_FAILURE);

			}

			break;

		default: //does the default configurations with default settings

			break;

		}
	}
}

/**
* Displays the current grid
*
* @param grid: current grid of containing current cells
* @param size: current size of the grid
* @param c_chance: probability of tree catching fire
* @param d_chance: percentage of density trees
* @param b_chance: percentage of burning trees
* @param n_chance: neighbor effect percent
* @param cycle: step for each updated grid
* @param current_change: number of current changes in the grid
* @param cumulative_change: number of total changes for the simulation
*
* @return: the output of the current grid
*
*/
void display_grid(char grid[MAX_GRID][MAX_GRID], int size, int c_chance, int d_chance, int b_chance, int n_chance, int cycle, int current_change, int cumulative_change) {

	for (int r = 0; r < size; r++) {

		for (int c = 0; c < size; c++) {

			printf("%c", grid[r][c]); //prints each cell of the grid

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

/**
* Updates the current grid
*
* @param grid: current grid of containing current cells
* @param size: current size of the grid
* @param c_chance: probability of tree catching fire
* @param n_chance: neighor effect percent
* @param current_change: number of current changes in the grid
*
* @return: the new updated grid
*/
void update_grid(char grid[MAX_GRID][MAX_GRID], int size, int c_chance, int n_chance, int *current_change) {

	*current_change = 0;

	for (int row = 0; row < size; row++) {

		for(int col = 0; col < size; col++) {

			copy_grid[row][col] = grid[row][col]; //sets the copy grid with the current grid

		}

	}

	for (int row = 0; row < size; row++) {

		for (int col = 0; col < size; col++) {

			char character = grid[row][col]; //character of the cell

			if (character == EMPTY) { //character that's empty will always be empty

				copy_grid[row][col] = EMPTY;

			} else if (character == TREE) { //character that's a tree will start a spread function

				int total_nbr = 0;

				int burn_nbr = 0;

				if (spread(grid, n_chance, c_chance, row, col, &total_nbr, &burn_nbr)) { //if the tree gets caught on fire, it starts burning process

					copy_grid[row][col] = BURNING;
					counter[row][col] = 0;
					(*current_change)++;

				}

			} else if (character == BURNING) { //if that cell is still burning, we check the counter next

				counter[row][col]++; //increments the seconds to keep track when it will burn out

				if (counter[row][col] < 4) { //if the counter is less than 4 then, it still burns

					copy_grid[row][col] = BURNING;
					counter[row][col]++; //increments seconds for the counter

				} else {

					copy_grid[row][col] = BURNED; //if greater or equal to 4 seconds from counter, the cell burns out
					(*current_change)++;

				}


			} else if (character == BURNED) { //if the cell is burned out, it stays burned out

				copy_grid[row][col] = BURNED;

			}
		}
	}

	for (int row = 0; row < size; row++) {

		for (int col = 0; col < size; col++) {

			grid[row][col] = copy_grid[row][col]; //after changing copy grid, it's put back to the main grid

		}

	}

}

/**
* Displays the current grid in overlay mode
*
* @param grid: current grid of containing current cells
* @param size: current size of the grid
* @param c_chance: probability of tree catching fire
* @param d_chance: percentage of density trees
* @param b_chance: percentage of burning trees
* @param n_chance: neighbor effect percent
* @param cycle: step for each updated grid
* @param current_change: number of current changes in the grid
* @param cumulative_change: number of total changes for the simulation
*
* @return: the output of the current grid in overlay mode
*/
void o_mode(char grid[MAX_GRID][MAX_GRID], int size, int c_chance, int d_chance, int b_chance, int n_chance, int cycle, int current_change, int cumulative_change) {

	set_cur_pos(1, 0);

	for(int row = 0; row < size; row++) {

		for (int col = 0; col < size; col++) {

			set_cur_pos(row, col + 1);
			put(grid[row][col]);

		}

		set_cur_pos(row + 1, 0);

		put('\n');

	}

	float pCatch = (float) c_chance / 100.0;
	float density = (float) d_chance / 100.0;
	float pBurning = (float) b_chance / 100.0;
	float pNeighbor = (float) n_chance / 100.0;

	set_cur_pos(size, 0);
	printf("size %d, pCatch %.2f, density %.2f, pBurning %.2f, pNeighbor %.2f \n", size, pCatch, density, pBurning, pNeighbor);
	printf("cycle %d, current changes %d, cumulative changes %d.\n", cycle, current_change, cumulative_change);

}

/**
* Randomization for the cells
*
* @param cells: the array of cells
* @param n: total number of cells
*
* @return: random placement of the cells
*/
void fy_shuffle(int *cells, int n) {

	for (int i = n - 1; i > 0; --i) {

		int j = rand() % (i + 1);

		int temp = cells[i];
		cells[i] = cells[j];
		cells[j] = temp;
	}
}

/**
* Sets up the start of the grid
*
* @param grid: current grid containing current cells
* @param size: current size of the grid
* @param d_chance: percentage of density trees
* @param b_chance: percentage of burning trees
*
* @return: the initialization of the grid
*/
void initialize_grid(char grid[MAX_GRID][MAX_GRID], int size, int d_chance, int b_chance) {

	float d_prob = (float) d_chance/100; // percentage chance of density

	float b_prob = (float) b_chance/100; // percentage chance of burning

	int total_cell = size * size; //gets total number of cells in a grid

	int temp_treeT_num = (int) (d_prob * total_cell); //temporary number of trees

	int burnT_num = (int) (b_prob * temp_treeT_num); //number of burning cells

	int treeT_num = temp_treeT_num - burnT_num;

	int emptyT_num = total_cell - (treeT_num + burnT_num); //number of empty cells

	int cells[total_cell]; // max cell array

	int i = 0;

	for (int t = 0; t < treeT_num; t++) {

		cells[i++] = TREE; //sets up tree cells

	}

	for (int b = 0; b < burnT_num; b++) {

		cells[i++] = BURNING; //sets up burning cells

	}

	for (int e = 0; e < emptyT_num; e++) {

		cells[i++] = EMPTY; //sets up empty cells

	}

    	fy_shuffle(cells, total_cell); //shuffles the cells to randomize the initialization grid

	int cell = 0;

    	for (int r = 0; r < size; r++) {

		for (int c = 0; c < size; c++) {

			grid[r][c] = cells[cell++]; //sets the grid from the single array of cells

		}

	}

}

/**
* Checks to see if there's still fire inside the grid
*
* @param grid: current grid containing current cells
* @param size: current size of the grid
*
* @return 1 if there is still fire in the grid, 0 if there isn't
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

/**
* Main function of wildfire.c program.
* Takes the user inputs and then decides whether user wants print or overlay mode
*
* @param argc: the number of arguements that's being passed from command line
* @param argv: array of characters that has arguements from command line
*
* @return: 0 for sucess
*/
int main(int argc, char *argv[]) {

	srand(41); //seed of the random number

	int cycle = 0; //steps for each updated grid

	int current_change = 0;
	int cumulative_change = 0;

	command_parse(argc, argv);

	if (p_mode > 0) { //print mode

		if (cycle == 0) { //when cycle is 0 for print mode, we do the header first

			p_header(p_mode);

			initialize_grid(grid, s_size, d_chance, b_chance);

			display_grid(grid, s_size, c_chance, d_chance, b_chance, n_chance, cycle, current_change, cumulative_change);

        		update_grid(grid, s_size, c_chance, n_chance, &current_change);

			cumulative_change += current_change;

			cycle++;

		}

		for (int i = 0; i < p_mode; i++) { //iterates the print mode with the number of cycles user wants

			display_grid(grid, s_size, c_chance, d_chance, b_chance, n_chance, cycle, current_change, cumulative_change);

        		update_grid(grid, s_size, c_chance, n_chance, &current_change);

			cumulative_change += current_change;

	        	if (!fire_checker(grid, s_size)) { //if there's no fire then the fire is out

				cycle++;

				display_grid(grid, s_size, c_chance, d_chance, b_chance, n_chance, cycle, current_change, cumulative_change);

        	    		printf("Fires are out\n");

	            		break;
        		}

			cycle++;

		}

	} else { //overlay mode

		initialize_grid(grid, s_size, d_chance, b_chance);

		clear(); //clears the whole terminal to set up the overlay mode

		while (1) {

			set_cur_pos(1, 0);

			o_mode(grid, s_size, c_chance, d_chance, b_chance, n_chance, cycle, current_change, cumulative_change);

		        usleep(750000); //causes the overlay mode to slow down

		        update_grid(grid, s_size, c_chance, n_chance, &current_change);

			cumulative_change += current_change;

		        if (!fire_checker(grid, s_size)) { //if there's no fire then the fire is out

				cycle++;

				o_mode(grid, s_size, c_chance, d_chance, b_chance, n_chance, cycle, current_change, cumulative_change);

		        	printf("Fires are out\n");

				break;

        		}

			cycle++;

		}
	}

	return 0;

}



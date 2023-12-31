/**************************************************************************
 * This work originally copyright David J. Malan of Harvard University, or-
 * iginally released under:
 * CC BY-NC-SA 3.0  http://creativecommons.org/licenses/by-nc-sa/3.0/
 * licensing.
 *
 * It has been adapted for use in csci 1730.  Per the share-alike
 * clause of this license, this document is also released under the
 * same license.
 **************************************************************************/
/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
int move(int tile);
int won(void);

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (1)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("win!\n");
            break;
        }

        // prompt for move
        printf("Tile to move (0 to exit): ");
        int tile;
		scanf("%d", &tile);
		getchar();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(50000);
        }

        // sleep thread for animation's sake
        usleep(50000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(200000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
	//Tile starts at the square of the dimension - 1
	int ds = (d*d) - 1;

	//Subtract from ds to get the current int for the tile
	int count = 0;

	for (int i = 0; i < d; i++) {

		for (int j = 0; j < d; j++) {
			//Initializes the value in the board
			//Starts at (d*d)-1 and then decreases by one each time with count
			board[i][j] = ds - count;
			count++;
		}
	}

	//If the board has even dimensions, swap tiles 1 & 2
	if ((ds + 1) % 2 == 0) {
		board[d - 1][d - 3] = 1;
		board[d - 1][d - 2] = 2;
	}
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
	for (int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {
			if (board[i][j] > 0) {
				printf(" %d ", board[i][j]);
			}

			if (board[i][j] == 0) {
				printf(" _ ");
			}
		}

		printf("\n");
	}
}

/**
 * If tile borders empty space, moves tile and returns 1 (true), else
 * returns 0 (false).
 */
int move(int tile)
{
	if (tile > d * d - 1 || tile < 1) {
		return 0;   //Checking if the tile is valid
	}

	int row, col; //current tile positions
	row = 0;
	col = 0;

	int blankRow, blankCol; //blanktile positions
	blankRow = 0;
	blankCol = 0;

	for (int i = 0; i < d; i++) {   //Finding the current placement of
		for (int j = 0; j < d; j++) { //the current tile
			if (board[i][j] ==  tile) {
				row = i;
				col = j;
			}

			if (board[i][j] == 0) { //the blank tile
				blankRow = i;
				blankCol = j;
			}
		}
	}

	//Check if the tile is adjacent to the blank space
	if ((abs(row - blankRow) == 1 && col == blankCol) ||
	(row == blankRow && abs(col - blankCol) == 1)) {
		board[blankRow][blankCol] = tile;
		board[row][col] = 0;
		return 1;
	}
	return 0;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
int won(void)
{
	int counter = 0;

	// Check each tile to ensure it's in order
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
			++counter; //Increment counter

			if (counter != (d * d)){ //Check if counter isn't equal to largest tile
				if (board[i][j] != counter) return 0; //Not won yet
			}
        }
    }

    return 1; // Game is won
}

#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define MaxInput 12


// function declarations
void printBoard(int [6][7]);
int userMove();
void makeMove(int [6][7] , int, unsigned int);
int aiMove();
int** copyBoard(int [6][7]);
bool winningMove(int[6][7], unsigned int);
int scoreSet(unsigned int[], unsigned int);
int tabScore(int[6][7], unsigned int);
int* miniMax(int[6][7], unsigned int, int, int, unsigned int);
int heurFunction(unsigned int, unsigned int, unsigned int);

// I'll be real and say this is just to avoid magic numbers
unsigned int NUM_COL = 7; // how wide is the board
unsigned int NUM_ROW = 6; // how tall
const unsigned int PLAYER = 1; // player number
unsigned int COMPUTER = 2; // AI number
unsigned int MAX_DEPTH = 2; // the default "difficulty" of the computer controlled AI
char player_1[MaxInput];

bool gameOver = false; // flag for if game is over
unsigned int turns = 0; // count for # turns
unsigned int currentPlayer = PLAYER; // current player

int board[6][7]; // the game board
int tempArr [2];
int moveSoFar[2];
char selection[MaxInput];


/*
 * Function:  isValidLocation
 * --------------------
 *
 * Checks if selection is a number.
 * Checks if the last(top which is the 6th place (or 5th if we consider the array ideology) place is empty, if it is, then logically it can hold one extra dice!
 * Also checks if column is out of array bounds and does not exists it reasks the user to enter their turn
 *
 * @param : selection  | what did the user select
 *
 * returns: boolean indicating if the selection is valid
 *
 * Testing partitions:
 *
 * - Empty Column
 * - Full Column
 * - Not Full Column
 * - Non-Existent Column
 * - Existing Column
 */
bool isValidLocation(char* selection){
    int length = strlen (selection);
    for (int i=0;i<length; i++){
        if (!isdigit(selection[i]))
        {
            printf("Selection is not a number!\n");
            return false;
        }
    }

    int column = atoi(selection);
    if(board[0][column] == 0 && column >= 0 && column <= 6)
        return true;

    else
    if(column < 0 || column > 6){
        printf("\nColumn %d does not exist !\n\n", column);
        return false;
    }
    if(board[5][column] != 0){
        printf("\nColumn is full please choose another column!\n\n");
        return false;
    }
    return true;
}



/**
 * game playing function
 * loops between players while they take turns
 */
void playGame() {
    printBoard(board); // print initial board
    while (!gameOver) { // while no game over state
        if (currentPlayer == COMPUTER) { // AI move
            makeMove(board, aiMove(), COMPUTER);
        }
        else if (currentPlayer == PLAYER) { // player move
            makeMove(board, userMove(), PLAYER);
        }
        else if (turns == NUM_ROW * NUM_COL) { // if max number of turns reached
            gameOver = true;
        }
        gameOver = winningMove(board, currentPlayer); // check if player won
        currentPlayer = (currentPlayer == 1) ? 2 : 1; // switch player
        turns++; // iterate number of turns
        printf("\n");
        printBoard(board); // print board after successful move
    }
    if (turns == NUM_ROW * NUM_COL) { // if draw condition
        printf("Draw!");
    }
    else { // otherwise, someone won
        if(currentPlayer == PLAYER)
            printf("AI Wins!\n");
        else
            printf("Player Wins!\n");
    }
}

/**
 * function that makes the move for the player
 * @param b - the board to make move on
 * @param c - column to drop piece into
 * @param p - the current player
 */
void makeMove(int b[6][7], int c, unsigned int p) {
// start from bottom of board going up
    for (unsigned int r = 0; r < NUM_ROW; r++) {
        if (b[r][c] == 0) { // first available spot
            b[r][c] = p; // set piece
            break;
        }
    }
}

/**
 * prompts the user for their move
 * and ensures valid user input
 * @return the user chosen column
 */
int userMove() {
    char* move;
    do {
        printf("Make your Selection(0-6): ");
        scanf("%s", &selection);
    } while(!isValidLocation(selection));

    int column = atoi(selection);

    return column;
}

/**
 * AI "think" algorithm
 * uses minimax to find ideal move
 * @return - the column number for best move
 */
int aiMove() {
    printf("AI is thinking about a move...\n");
    return miniMax(board, MAX_DEPTH, 0 - INT_MAX, INT_MAX, COMPUTER)[1];
}

/**
 * Minimax implementation using alpha-beta pruning
 * @param b - the board to perform MM on
 * @param d - the current depth
 * @param alf - alpha
 * @param bet - beta
 * @param p - current player
 */
int* miniMax(int b[6][7], unsigned int d, int alf, int bet, unsigned int p) {
/**
 * if we've reached minimal depth allowed by the program
 * we need to stop, so force it to return current values
 * since a move will never (theoretically) get this deep,
 * the column doesn't matter (-1) but we're more interested
 * in the score
 *
 * as well, we need to take into consideration how many moves
 * ie when the board is full
 */
    if (d == 0 || d >= (NUM_COL * NUM_ROW) - turns) {
// get current score to return
        int tabScoreTemp = tabScore(b, COMPUTER);

        tempArr [0] = tabScoreTemp;
        tempArr [1] = -1;

        return tempArr;
    }
    if (p == COMPUTER) { // if AI player
        moveSoFar[0] = INT_MIN;
        moveSoFar[1] = -1; // since maximizing, we want the lowest possible value
        if (winningMove(b, PLAYER)) { // if player about to win
            return moveSoFar; // force it to say it's worst possible score, so it knows to avoid move
        } // otherwise, business as usual
        for (unsigned int c = 0; c < NUM_COL; c++) { // for each possible move
            if (b[NUM_ROW - 1][c] == 0) { // but only if that column is non-full
                int ** newBoard = copyBoard(b); // make a copy of the board
                makeMove(newBoard, c, p); // try the move
                int score = miniMax(newBoard, d - 1, alf, bet, PLAYER)[0]; // find move based on that new board state
                if (score > moveSoFar[0]) { // if better score, replace it, and consider that best move (for now)
                    moveSoFar[0] = score,
                            moveSoFar[1] = (int)c;
                }
                alf = max(alf, moveSoFar[0]);
                if (alf >= bet) { break; } // for pruning
            }
        }
        return moveSoFar; // return best possible move
    }
    else {
        moveSoFar[0] = INT_MAX;
        moveSoFar[1] = -1; // since PLAYER is minimized, we want moves that diminish this score
        if (winningMove(b, COMPUTER)) {
            return moveSoFar; // if about to win, report that move as best
        }
        for (unsigned int c = 0; c < NUM_COL; c++) {
            if (b[NUM_ROW - 1][c] == 0) {
                int** newBoard = copyBoard(b);
                makeMove(newBoard, c, p);
                int score = miniMax(newBoard, d - 1, alf, bet, COMPUTER)[0];
                if (score < moveSoFar[0]) {
                    moveSoFar[0] = score;
                    moveSoFar[1] = (int)c;
                }
                bet = min(bet, moveSoFar[0]);
                if (alf >= bet) { break; }
            }
        }
        return moveSoFar;
    }
}

/**
 * function to tabulate current board "value"
 * @param b - the board to evaluate
 * @param p - the player to check score of
 * @return - the board score
 */
int tabScore(int b[6][7], unsigned int p) {
    int score = 0;
    unsigned int rs[NUM_COL];
    unsigned int cs[NUM_ROW];
    unsigned int set[4];
    /**
     * horizontal checks, we're looking for sequences of 4
     * containing any combination of AI, PLAYER, and empty pieces
     */
    for (unsigned int r = 0; r < NUM_ROW; r++) {
        for (unsigned int c = 0; c < NUM_COL; c++) {
            rs[c] = b[r][c]; // this is a distinct row alone
        }
        for (unsigned int c = 0; c < NUM_COL - 3; c++) {
            for (int i = 0; i < 4; i++) {
                set[i] = rs[c + i]; // for each possible "set" of 4 spots from that row
            }
            score += scoreSet(set, p); // find score
        }
    }
    // vertical
    for (unsigned int c = 0; c < NUM_COL; c++) {
        for (unsigned int r = 0; r < NUM_ROW; r++) {
            cs[r] = b[r][c];
        }
        for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
            for (int i = 0; i < 4; i++) {
                set[i] = cs[r + i];
            }
            score += scoreSet(set, p);
        }
    }
    // diagonals
    for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
        for (unsigned int c = 0; c < NUM_COL; c++) {
            rs[c] = b[r][c];
        }
        for (unsigned int c = 0; c < NUM_COL - 3; c++) {
            for (int i = 0; i < 4; i++) {
                set[i] = b[r + i][c + i];
            }
            score += scoreSet(set, p);
        }
    }
    for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
        for (unsigned int c = 0; c < NUM_COL; c++) {
            rs[c] = b[r][c];
        }
        for (unsigned int c = 0; c < NUM_COL - 3; c++) {
            for (int i = 0; i < 4; i++) {
                set[i] = b[r + 3 - i][c + i];
            }
            score += scoreSet(set, p);
        }
    }
    return score;
}

/**
 * function to find the score of a set of 4 spots
 * @param v - the row/column to check
 * @param p - the player to check against
 * @return - the score of the row/column
 */
int scoreSet(unsigned int v[4], unsigned int p) {
    unsigned int good = 0; // points in favor of p
    unsigned int bad = 0; // points against p
    unsigned int empty = 0; // neutral spots
    for (unsigned int i = 0; i < 4; i++) { // just enumerate how many of each
        good += (v[i] == p) ? 1 : 0;
        bad += (v[i] == PLAYER || v[i] == COMPUTER) ? 1 : 0;
        empty += (v[i] == 0) ? 1 : 0;
    }
    // bad was calculated as (bad + good), so remove good
    bad -= good;
    return heurFunction(good, bad, empty);
}

/**
 * my """heuristic function""" is pretty bad, but it seems to work
 * it scores 2s in a row and 3s in a row
 * @param g - good points
 * @param b - bad points
 * @param z - empty spots
 * @return - the score as tabulated
 */
int heurFunction(unsigned int g, unsigned int b, unsigned int z) {
    int score = 0;
    if (g == 4) { score += 500001; } // preference to go for winning move vs. block
    else if (g == 3 && z == 1) { score += 5000; }
    else if (g == 2 && z == 2) { score += 500; }
    else if (b == 2 && z == 2) { score -= 501; } // preference to block
    else if (b == 3 && z == 1) { score -= 5001; } // preference to block
    else if (b == 4) { score -= 500000; }
    return score;
}

/**
 * function to determine if a winning move is made
 * @param b - the board to check
 * @param p - the player to check against
 * @return - whether that player can have a winning move
 */
bool winningMove(int b[6][7], unsigned int p) {
    unsigned int winSequence = 0; // to count adjacent pieces
// for horizontal checks
    for (unsigned int c = 0; c < NUM_COL - 3; c++) { // for each column
        for (unsigned int r = 0; r < NUM_ROW; r++) { // each row
            for (int i = 0; i < 4; i++) { // recall you need 4 to win
                if ((unsigned int)b[r][c + i] == p) { // if not all pieces match
                    winSequence++; // add sequence count
                }
                if (winSequence == 4) { return true; } // if 4 in row
            }
            winSequence = 0; // reset counter
        }
    }
// vertical checks
    for (unsigned int c = 0; c < NUM_COL; c++) {
        for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
            for (int i = 0; i < 4; i++) {
                if ((unsigned int)b[r + i][c] == p) {
                    winSequence++;
                }
                if (winSequence == 4) { return true; }
            }
            winSequence = 0;
        }
    }
// the below two are diagonal checks
    for (unsigned int c = 0; c < NUM_COL - 3; c++) {
        for (unsigned int r = 3; r < NUM_ROW; r++) {
            for (int i = 0; i < 4; i++) {
                if ((unsigned int)b[r - i][c + i] == p) {
                    winSequence++;
                }
                if (winSequence == 4) { return true; }
            }
            winSequence = 0;
        }
    }
    for (unsigned int c = 0; c < NUM_COL - 3; c++) {
        for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
            for (int i = 0; i < 4; i++) {
                if ((unsigned int)b[r + i][c + i] == p) {
                    winSequence++;
                }
                if (winSequence == 4) { return true; }
            }
            winSequence = 0;
        }
    }
    return false; // otherwise no winning move
}

/**
 * sets up the board to be filled with empty spaces
 * also used to reset the board to this state
 */
void initBoard() {
    for (unsigned int r = 0; r < NUM_ROW; r++) {
        for (unsigned int c = 0; c < NUM_COL; c++) {
            board[r][c] = 0; // make sure board is empty initially
        }
    }
}

/**
 * function to copy board state to another 2D vector
 * ie. make a duplicate board; used for mutating copies rather
 * than the original
 * @param b - the board to copy
 * @return - said copy
 */
int** copyBoard(int b[6][7]) {
    int newBoard[NUM_ROW][NUM_COL];
    for (unsigned int r = 0; r < NUM_ROW; r++) {
        for (unsigned int c = 0; c < NUM_COL; c++) {
            newBoard[r][c] = b[r][c]; // just straight copy
        }
    }
    return newBoard;
}

/**
 * prints the board to console out
 * @param b - the board to print
 */
void printBoard(int b[6][7]) {
    for (unsigned int i = 0; i < NUM_COL; i++) {
        printf(" %d", i);
    }
    printf("\n---------------\n");
    for (unsigned int r = 0; r < NUM_ROW; r++) {
        for (unsigned int c = 0; c < NUM_COL; c++) {
            printf("|");
            switch (b[NUM_ROW - r - 1][c]) {
                case 0:
                    printf("0"); break; // no piece
                case 1:
                    printf("1"); break; // one player's piece
                case 2:
                    printf("2"); break; // other player's piece
            }
            if (c + 1 == NUM_COL) { printf("|"); }
        }
        printf("\n");
    }
    printf("---------------\n\n");
}



/*
 * Function:  getName
 * --------------------
 * Handles the input of the name not accepting spaces or nulls
 *
 * @params : integer playerNum
 *
 * playerNum : indicates if it is player 1 or 2
 *
 * returns: String (char array) name of player
 *
 * Testing Partitions :
 *
 * - String without any spaces
 * - Empty String
 * - String with space
 * - String of size <= MaxInput
 * - String of size > MaxInput
 */
char *getName(){
    char player[MaxInput];
    printf("Please enter your name: \n");
    scanf("%s", &player);
    while(strlen(player) == 0){
        printf("INVALID!");
        printf("Please enter your name: \n");
        scanf("%s", &player);
    }
    char *player_ptr = player;
    return player_ptr;
}



/**
 * main driver
 */
int main() {

    initBoard(); // initial setup
    playGame(); // begin the game
    return 0; // exit state
}

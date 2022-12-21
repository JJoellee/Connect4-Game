#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>


#define row_size 6
#define column_size 7
#define MaxInput 12 //for both the players' name & the column selection

int Board[6][7];

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
char *getName(int playerNum){
    char player[MaxInput];
    printf("Player %d, please enter your name: \n", playerNum);
    scanf("%s %*s", &player);
    while(strlen(player) == 0){
            printf("INVALID!");
            printf("Player %d, please enter your name: \n"), playerNum;
            scanf("%s %*s", &player);
    }
    char *player_ptr = player;
    return player_ptr;
}

/*
 * Function:  printBoard
 * --------------------
 * Prints the "Connect Four" game board.
 *
 *  Board: 2D-Array representing the board
 *
 *  returns: void (Just prints the board with all the plays up until the current moment)
 */
void printBoard(){

    printf("---------------\n");
    for(int row = 0; row < 6 ; row++){
        printf("|");
        for(int col = 0 ; col <7 ; col++){
            printf("%d|", Board[row][col]);
        }
        printf("\n");
    }
    printf("---------------\n");
}



/*
 * Function:  whoStarts
 * --------------------
 * Flips a coin determining which player starts the game
 * 
 * @params : Character array (String) player_1, player_2
 *
 * player_1 : name of the first player
 * player_2 : name of the second player
 *
 * returns: prints a String indicating which player starts the game
 *
 */
int whoStarts(char* player_1, char* player_2){

    int lower = 1;
    int upper = 2;
    int num = rand()%(upper + 1 - lower) + lower;
    if(num == 1){
        printf("%s's turn\n", player_1);
        return 1;
    }
    else{
        printf("%s's turn\n", player_2);
        return 2;
    }

}


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
        if(Board[0][column] == 0 && column >= 0 && column <= 6)
        return true;

    else
        if(column < 0 || column > 6){
            printf("\nColumn %d does not exist !\n\n", column);
            return false;
        }
        else{
            printf("\nColumn is full please choose another column!\n\n");
            return false;
        }
}



/*
 * Function : scanBoard
 * --------------------
 * Scans board in 4 different directions. Returns true if one of the ways are true
 *
 * @params : int piece
 *
 * returns: boolean indicating if a player wins
 *
 * Testing partitions : 
 * 
 * - Empty Board
 * - Full Board
 * - Four connected pieces horizontally
 * - Less than four connected pieces horizontally
 * - Four connected pieces vertically
 * - Less than four connected pieces vertically
 * - Four connected pieces on a positively sloped line
 * - Less than four connected pieces on a positively sloped line
 * - Four connected pieces on a negatively sloped line
 * - Less than four connected pieces on a negatively sloped line
 * 
 */

//PIECE IS EITHER RED OR GREEN BUT HERE IT IS 1 OR 2
bool scanBoard(int piece){

    //Check horizontal locations for win
    for(int col = 0; col<column_size-3; col++){
        for(int row=0; row < row_size; row++){
            if(Board[row][col] == piece && Board[row][col+1] == piece && Board[row][col+2] == piece && Board[row][col+3] == piece){
                return true;
            }
        }
    }

    //Check vertical locations for win
    for(int col = 0; col<column_size; col++){
        for(int row = 0; row < row_size-3; row++){
            if(Board[row][col] == piece && Board[row + 1][col] == piece && Board[row + 2][col] == piece && Board[row + 3][col] == piece){
                return true;
            }
        }
    }

    //Check for positively sloped diagonals
    for(int col = 0; col < column_size-3; col++){
        for(int row = 0; row < row_size-3; row++){
            if(Board[row][col] == piece && Board[row+1][col+1] == piece && Board[row+2][col+2] == piece && Board[row+3][col+3] == piece){
                return true;
            }
        }
    }

    //Check negatively sloped diagonals
    for(int col = 0; col < column_size-3; col++){
        for(int row = 3; row < row_size; row++){
            if(Board[row][col] == piece && Board[row-1][col+1] == piece && Board[row-2][col+2] == piece && Board[row-3][col+3] == piece){
                return true;
            }
        }
    }

    return false;

}

/*
 * Function:  dropPiece
 * --------------------
 * Drops a piece in the specified column where the player chooses
 * 
 * @params : column, piece
 *
 * column : column that the player chooses
 * piece : piece number based on who's turn it is
 *
 * returns: void places piece in board
 *
 * For testing we already check in the main if the column is valid by "isValidLocation" function
 *  also we specify what the piece is as input based on the player number in the main.
 */

void dropPiece (char* selection, int piece){
    int column = atoi(selection);

    for(int row = row_size - 1 ; row >= 0 ; row--){
        if(Board[row][column] == 0){

            Board[row][column] = piece;
            break;
        }
    }
}


/*
 * Function : fullBoard
 * --------------------
 * Checks if the board is full
 * 
 * @params : null
 *
 * full : boolean sentinel
 *
 * returns: boolean (true if board is full & false otherwise)
 *
 * Testing partitions :
 * 
 * - Board is full
 * - Board is not full
 */
bool fullBoard(){
    bool full = true;
    for(int col = 0; col<column_size; col++){
        if(Board[0][col] == 0)
            full=false;
    }
    return full;
}

/*
 * Function : checkTime
 * --------------------
 * Prints the name of the time-based winner and each player's time taken
 * 
 * @params : players and their time
 *
 * returns: void 
 *
 */
void checkTime(char* player_1, double timePlayer1, char* player_2, double timePlayer2){
    double leastTime = (timePlayer1 < timePlayer2)? timePlayer1: timePlayer2;
    double mostTime = (timePlayer1 > timePlayer2)? timePlayer1: timePlayer2;
    const char *leastTimePlayer = (timePlayer1 < timePlayer2) ? player_1 : player_2;
    const char *mostTimePlayer = (timePlayer1 > timePlayer2) ? player_1 : player_2;
    printf("%s WON! \n Time taken by %s: %f vs %s: %f ", leastTimePlayer, leastTimePlayer, leastTime, mostTimePlayer, mostTime);
}

/*
 * Function : printWinner
 * --------------------
 * Prints the name of the game winner
 * 
 * @params : player name (winner)
 *
 * returns: void 
 *
 */
void printWinner(char* player){
    printf("%s won!", player);
}



int main(){

    bool gameOver = false;
    double timePlayer1 = 0;
    double timePlayer2 = 0;
    clock_t start, end;
    char selection[MaxInput];
    int startingPlayer,  currentPlayer;
    char *currentPlayerName;
    double *currentPlayerTime;

    char player_1[MaxInput], player_2[MaxInput];

    strcpy(player_1, getName(1));
    strcpy(player_2, getName(2));

    srand(time(NULL));

    currentPlayer = whoStarts(player_1, player_2);
    
    while(!gameOver){
        currentPlayerName = (currentPlayer == 1) ? player_1: player_2;
        currentPlayerTime = (currentPlayer == 1)? &timePlayer1 : &timePlayer2;
        
        if(fullBoard()){
            checkTime(player_1, timePlayer1,player_2,timePlayer2);
            break;
        }
        printBoard();
        do {
            printf("%s, Make your Selection(0-6):\n", currentPlayerName );
            start = clock();
            scanf("%s", &selection);
            end = clock();
            *currentPlayerTime += ((double)(end - start)) / CLOCKS_PER_SEC;
        } while(!isValidLocation(selection));

        dropPiece(selection, currentPlayer);

        gameOver = scanBoard(currentPlayer);

        if(gameOver){
            printBoard();
            printWinner(currentPlayerName);
        }
        currentPlayer = (currentPlayer==1)? 2:1;
    }
}

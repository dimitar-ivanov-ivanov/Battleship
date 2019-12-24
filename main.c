#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define ROWS 10
#define COLS 10
#define SHIPDIRECTION 50
#define NUMBEROFSHIPS 5
#define EMPTYSYMBOL '*'
#define SHIPSYMBOL '^'
#define SHOTSYMBOL '#'
#define INVALIDDIRECTION "Invalid ship direction,input up/down/left/right"
#define INVALIDSHIPPARAMETERS "Ship goes out of the board"
#define SHIPALREDYTHERE "There is a ship alredy in that place"
#define ALREDYSHOTTHERE "You have alredy shot in that place"
#define YOURTURN "Your turn."
#define ENEMYTURN "Enemy turn."
#define SEPARATOR "     "

int userPointsRemaining = 0;
int computerPointsRemaining = 0;

void runGame();
void inputUserBoard(char userBoard[ROWS][COLS]);
void putDefaultValuesOnBoard(char board[ROWS][COLS]);
int checkWord(int len,char word[len],int validWordsRow,int validWordsCol,char validWords[validWordsRow][validWordsCol]);
int checkInputParameters(int startRowShip, int startColShip, int endRowShip, int endColShip);
int placeShipHorizontally(int startRowShip, int startColShip, int endRowShip, int endColShip, char board[ROWS][COLS],int direction);
int placeShipVertically(int startRowShip, int startColShip, int endRowShip, int endColShip, char board[ROWS][COLS],int direction);
void inputComputerBoard(char computerBoard[ROWS][COLS]);
int generateRandomNumber(int lower,int upper);
void printBoard(char board[ROWS][COLS]);
void playGame(char userBoard[ROWS][COLS],char computerBoard[ROWS][COLS]);
void playerShoots(char computerBoard[ROWS][COLS]);
void computerShoots(char userBoard[ROWS][COLS]);
void printDuringGame(char userBoard[ROWS][COLS],char computerBoard[ROWS][COLS]);
void finishGame(char computerBoard[ROWS][COLS]);

int main() {
    runGame();
    return 0;
}

void runGame() {
    char userBoard[ROWS][COLS];
    char computerBoard[ROWS][COLS];
    inputUserBoard(userBoard);
    srand(time(NULL));
    inputComputerBoard(computerBoard);
    playGame(userBoard,computerBoard);
    finishGame(computerBoard);
}

void inputUserBoard(char userBoard[ROWS][COLS]) {
    int i, startRowShip, startColShip, endRowShip, endColShip,lenShip;
    char shipDirection;

    char fullShipDirection[SHIPDIRECTION];
    int shipSizes[NUMBEROFSHIPS] = { 4, 3, 2, 2, 1 };
    char validWords[8][10] = { "up","down","left","right","u","d","l","r" };

    putDefaultValuesOnBoard(userBoard);

    printf("Input ships:\n");
    printf("Example Input: Row of ship,Column of ship,Ship direction: UP|DOWN|LEFT|RIGHT\n");

    for (i = 0; i < NUMBEROFSHIPS; i++) {
        printBoard(userBoard);
        printf("Input ship with size %d:", shipSizes[i]);
        scanf("%d %d %s", &startRowShip, &startColShip, fullShipDirection);
        fflush(stdin);

        //invalid input -> skip input
        if (!checkWord(SHIPDIRECTION,fullShipDirection,8,10,validWords)) {
            printf("%s\n",INVALIDDIRECTION);
            i--;
            continue;
        }

        shipDirection = fullShipDirection[0];
        lenShip = shipSizes[i];
        //add/remove one because the startingCell is of length 1
        if (shipDirection == 'u') {
            endRowShip = startRowShip - lenShip + 1;
            endColShip = startColShip;
        } else if (shipDirection == 'd') {
            endRowShip = startRowShip + lenShip - 1;
            endColShip = startColShip;
        } else if (shipDirection == 'l') {
            endRowShip = startRowShip;
            endColShip = startColShip - lenShip + 1;
        } else if (shipDirection == 'r') {
            endRowShip = startRowShip;
            endColShip = startColShip + lenShip - 1;
        }

        if (!checkInputParameters(startRowShip, startColShip, endRowShip, endColShip)) {
            //print the message here because we will reuse this function later on when we don't need the message imbedded
            printf("%s\n", INVALIDSHIPPARAMETERS);
            i--;
            continue;
        }

        if (shipDirection == 'u') {
            //move one row up  row -1
            placeShipVertically(startRowShip,startColShip,endRowShip,endColShip,userBoard,-1);
        } else if (shipDirection == 'd') {
            //move one row donw row + 1
            placeShipVertically(startRowShip,startColShip,endRowShip,endColShip,userBoard,1);
        } else if (shipDirection == 'l') {
            //move one column left column -1
            placeShipHorizontally(startRowShip, startColShip, endRowShip, endColShip, userBoard,-1);
        } else if (shipDirection == 'r') {
            //move one column right column + 1
            placeShipHorizontally(startRowShip, startColShip, endRowShip, endColShip, userBoard,1);
        }

        //successfully placed ship
        userPointsRemaining += lenShip;
    }
}

void inputComputerBoard(char computerBoard[ROWS][COLS]) {
    int i,startRowShip,startColShip,endRowShip,endColShip,indexDirection,lenShip;
    int shipSizes[NUMBEROFSHIPS] = { 4, 3, 2, 2, 1 };
    char directions[4] = {'u','d','l','r'};
    char shipDirection;

    putDefaultValuesOnBoard(computerBoard);

    for(i = 0; i < NUMBEROFSHIPS; i++) {
        startRowShip = generateRandomNumber(0,10);
        startColShip = generateRandomNumber(0,10);
        indexDirection = generateRandomNumber(0,4);
        lenShip = shipSizes[i];

        shipDirection = directions[indexDirection];

        //add/remove one because the startingCell is of length 1
        if (shipDirection == 'u') {
            endRowShip = startRowShip - lenShip + 1;
            endColShip = startColShip;
        } else if (shipDirection == 'd') {
            endRowShip = startRowShip + lenShip - 1;
            endColShip = startColShip;
        } else if (shipDirection == 'l') {
            endRowShip = startRowShip;
            endColShip = startColShip - lenShip + 1;
        } else if (shipDirection == 'r') {
            endRowShip = startRowShip;
            endColShip = startColShip + lenShip - 1;
        }

        if (!checkInputParameters(startRowShip, startColShip, endRowShip, endColShip)) {
            i--;
            continue;
        }

        if (shipDirection == 'u') {
            //move one row up  row -1
            placeShipVertically(startRowShip,startColShip,endRowShip,endColShip,computerBoard,-1);
        } else if (shipDirection == 'd') {
            //move one row donw row + 1
            placeShipVertically(startRowShip,startColShip,endRowShip,endColShip,computerBoard,1);
        } else if (shipDirection == 'l') {
            //move one column left column -1
            placeShipHorizontally(startRowShip, startColShip, endRowShip, endColShip, computerBoard,-1);
        } else if (shipDirection == 'r') {
            //move one column right column + 1
            placeShipHorizontally(startRowShip, startColShip, endRowShip, endColShip, computerBoard,1);
        }

        computerPointsRemaining += lenShip;
    }
}

void playGame(char userBoard[ROWS][COLS],char computerBoard[ROWS][COLS]) {
    printDuringGame(userBoard,computerBoard);

    //1 for player turn
    //-1 for computer turn
    int playerTurn = 1;

    //when one player's points go to zero he loses
    while(userPointsRemaining >0 && computerPointsRemaining > 0) {
        if(playerTurn == 1) {
            //ask player for coordinates he shoots at computerBoard the place where he shoots becomes -1
            printf("%s\n",YOURTURN);
            playerShoots(computerBoard);
            printDuringGame(userBoard,computerBoard);
            printf("Computer remaining points: %d\n",computerPointsRemaining);
        } else if(playerTurn == -1) {
            //computer shoots at player board and shows where he shot and the condition of our board
            printf("%s\n",ENEMYTURN);
            computerShoots(userBoard);
            printDuringGame(userBoard,computerBoard);
            printf("User remaining points: %d\n",userPointsRemaining);
        }

        playerTurn *=-1;
    }

}

void playerShoots(char computerBoard[ROWS][COLS]) {
    int row = ROWS;
    int col = COLS;

    //make sure input is in bounds and also that we haven't shot in that place alredy
    //because if we keep shooting in the same place the game might go on forever
    while(checkInputParameters(row,col,row,col) == 0) {
        printf("Take your shot at:");
        scanf("%d %d",&row,&col);
        if(!checkInputParameters(row,col,row,col)) {
            printf("%s\n",INVALIDSHIPPARAMETERS);
            continue;
        }
        //if we alredy shot there make row and col invalid and continue loop
        if(computerBoard[row][col] == SHOTSYMBOL) {
            printf("%s\n",ALREDYSHOTTHERE);
            row =ROWS;
            col = COLS;
            continue;
        }
    }

    //there is point there make it #
    if(computerBoard[row][col] == SHIPSYMBOL) {
        computerPointsRemaining--;
    }

    computerBoard[row][col] = SHOTSYMBOL;
}

void computerShoots(char userBoard[ROWS][COLS]) {
    int row = generateRandomNumber(0,10);
    int col = generateRandomNumber(0,10);

    //loop until we find a cell that we haven't shot into
    while(userBoard[row][col] == SHOTSYMBOL) {
        row = generateRandomNumber(0,10);
        col = generateRandomNumber(0,10);
    }

    if(userBoard[row][col] == SHIPSYMBOL) {
        userPointsRemaining--;
    }

    userBoard[row][col] = SHOTSYMBOL;
}

void finishGame(char computerBoard[ROWS][COLS]) {
    if(userPointsRemaining == 0) {
        printf("You lost");
        printf("Enemy board");
        printBoard(computerBoard);
    } else if(computerPointsRemaining == 0) {
        printf("You won,congratulations.");
    }
}

int generateRandomNumber(int lower,int upper) {
    return rand() % upper + lower;
}

void putDefaultValuesOnBoard(char board[ROWS][COLS]) {
    int i,j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            board[i][j] = EMPTYSYMBOL;
        }
    }
}

int checkWord(int len,char word[len],int validWordsRow,int validWordsCol,char validWords[validWordsRow][validWordsCol]) {
    int i, validDirection;

    //lower the direction in case the user inputted uppercase
    for (i = 0; i < strlen(word); i++) {
        word[i] = (word[i] <= 'Z' && word[i] >= 'A') ? word[i] + 32 : word[i];
    }

    //if direction word isn't up/down/left/right get new ship
    //if direction word isn't up/down/left/right get new ship
    //if one of them is true -> 0 that means it wont go into the if
    validDirection = 0;
    for (i = 0; i < validWordsRow; i++) {
        if (!strcmp(validWords[i], word)) {
            validDirection = 1;
            break;
        }
    }

    return validDirection;
}

int checkInputParameters(int startRowShip, int startColShip, int endRowShip, int endColShip) {
    if (startColShip < 0 || startColShip >= COLS || startRowShip < 0 || startRowShip >= ROWS ||
            endRowShip < 0 || endRowShip >= ROWS || endColShip < 0 || endColShip >= COLS) {
        return 0;
    }
    return 1;
}

int placeShipHorizontally(int startRowShip, int startColShip, int endRowShip, int endColShip, char board[ROWS][COLS],int direction) {
    //check wheter one column or the other is smaller so we know from where to where to move
    int i,start,end;

    if(startColShip < endColShip) {
        start = startColShip;
        end = endColShip;
    } else {
        start = endColShip;
        end = startColShip;
    }

    //check if ship is alredy placed there
    for (i = start; i <= end; i++) {
        if (board[startRowShip][i] == SHIPSYMBOL) {
            printf("%s\n", SHIPALREDYTHERE);
            return 0;
        }
    }

    //place ship
    for (i = start; i <= end; i++) {
        board[startRowShip][i] = SHIPSYMBOL;
    }
    return 1;
}

int placeShipVertically(int startRowShip, int startColShip, int endRowShip, int endColShip, char board[ROWS][COLS],int direction) {
    //check wheter one column or the other is smaller so we know from where to where to move
    int i,start,end;

    if(startRowShip < endRowShip) {
        start = startRowShip;
        end = endRowShip;
    } else {
        start = endRowShip;
        end = startRowShip;
    }

    for(i = start; i <= end; i++) {
        if(board[i][startColShip] == SHIPSYMBOL) {
            printf("%s\n",SHIPALREDYTHERE);
            return 0;
        }
    }

    for(i = start; i <= end; i++) {
        board[i][startColShip] = SHIPSYMBOL;
    }

    return 0;
}

void printBoard(char board[ROWS][COLS]) {
    int i,j;
    printf("_|");

    for(i = 0; i < ROWS; i++) {
        printf("%d|",i);
    }

    printf("\n");
    for(i = 0; i < ROWS; i++) {
        printf("%d|",i);
        for(j = 0; j < COLS; j++) {
            printf("%c|",board[i][j]);
        }
        printf("\n");
    }
}

void printDuringGame(char userBoard[ROWS][COLS],char computerBoard[ROWS][COLS]) {
    int i,j;
    printf("_|");
    for(i = 0; i < ROWS; i++) {
        printf("%d|",i);
    }

    printf("%s_|",SEPARATOR);
    for(i =0; i< ROWS; i++) {
        printf("%d|",i);
    }

    printf("\n");
    for(i = 0; i < ROWS; i++) {
        printf("%d|",i);
        for(j = 0; j < COLS; j++) {
            printf("%c|",userBoard[i][j]);
        }

        printf("%s",SEPARATOR);
        printf("%d|",i);
        for(j = 0; j < COLS; j++) {
            //in the real game don't show the player where to shoot but in this case to test it show where the enemy ships are
            //printf("%c|",computerBoard[i][j]);
            //if we have a ship on this cell don't show it to the user
            if(computerBoard[i][j] == SHIPSYMBOL) {
                printf("*|");
            } else {
                printf("%c|",computerBoard[i][j]);
            }

        }
        printf("\n");
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXROWS 50
#define MAXCOLS 50
#define SHIPDIRECTION 50
#define MESSAGELEN 50
#define INVALIDBOARDDIMENSIONS "Dimensions are too big"
#define INVALIDSHIPPARAMETERS "Ship goes out of the board"
#define INVALIDDIRECTION "Invalid ship direction,input vertical or horizontal"
#define INVALIDLENGTH "Lenght is not valid"
#define INVALIDSHIPS "Too many ships to build"
#define SHIPALREDYTHERE "There is a ship alredy in that place"
#define ALREDYSHOTTHERE "You have alredy shot in that place"
#define SEPARATOR "---------------------------------------------------\n"

static int rows = MAXROWS + 1;
static int cols = MAXCOLS + 1;
static int numberOfShips = 0;
static int userPointsRemaining = 0;
static int computerPointsRemaining = 0;

void runGame();
void inputRowAndCol();
void inputUserBoard(int userBoard[rows][cols]);
void inputComputerBoard(int computerBoard[rows][cols]);
int generateRandomNumber(int lower,int upper);
void printBoard(int userBoard[rows][cols]);
int getNumberOfShips();
void putDefaultValuesOnBoard(int userBoard[rows][cols]);
int checkInputParameters(int startRowShip,int startColShip);
int checkShipDirection(char fullShipDirection[SHIPDIRECTION]);
int placeShipHorizontally(int startRowShip,int startColShip,int lenShip,int board[rows][cols],int currentIsUser);
int placeShipVertically(int startRowShip,int startColShip,int lenShip,int board[rows][cols],int currentIsUser);
void playGame(int userBoard[rows][cols],int computerBoard[rows][cols]);
void playerShoots(int computerBoard[rows][cols]);
void computerShoots(int userBoard[rows][cols]);
void printBoardDuringGame(int board[rows][cols],char message[MESSAGELEN]);

int main() {
    runGame();
    return 0;
}

void runGame() {
    inputRowAndCol();
    int userBoard[rows][cols];
    int computerBoard[rows][cols];

    inputUserBoard(userBoard);
    inputComputerBoard(computerBoard);
    playGame(userBoard,computerBoard);

}

void inputRowAndCol() {
    while(rows > MAXROWS || cols > MAXCOLS) {
        printf("Input board dimensions: ");
        scanf("%d %d",&rows,&cols);
        if(rows > MAXROWS || cols > MAXCOLS) {
            printf("%s","Dimensions are too big\n");
        }
    }
}

void inputUserBoard(int userBoard[rows][cols]) {
    int i,startRowShip,startColShip,lenShip;
    char shipDirection;
    char fullShipDirection[SHIPDIRECTION];

    numberOfShips = getNumberOfShips();
    putDefaultValuesOnBoard(userBoard);

    printf("Input ships:\n");
    printf("Example Input: Row of ship,Column of ship,Length of ship,Ship direction: Horizontal or vertical\n");

    //ROW COL LEN V/H
    for(i = 0; i < numberOfShips; i++) {
        //print board everytime so that player can keep track of ships placement
        //printBoard(userBoard);
        fflush(stdin);
        scanf("%d %d %d %s",&startRowShip,&startColShip,&lenShip,fullShipDirection);
        fflush(stdin);

        //invalid input -> skip input
        if(!checkInputParameters(startRowShip,startColShip)) {
            //print the message here because we will reuse this function later on when we don't need the message imbedded
            printf("%s\n",INVALIDSHIPPARAMETERS);
            i--;
            continue;
        }

        if(!checkShipDirection(fullShipDirection)) {
            i--;
            continue;
        }

        shipDirection = fullShipDirection[0];
        if(shipDirection == 'h') {
            if(!placeShipHorizontally(startRowShip,startColShip,lenShip,userBoard,1)) {
                i--;
                continue;
            }
        } else if(shipDirection == 'v') {
            if(!placeShipVertically(startRowShip,startColShip,lenShip,userBoard,1)) {
                i--;
                continue;
            }
        }

        //successfully placed ship
        userPointsRemaining += lenShip;
    }

    printBoard(userBoard);
}

void inputComputerBoard(int computerBoard[rows][cols]) {
    int i,startRowShip,startColShip,lenShip;
    char shipDirection = 'h';
    putDefaultValuesOnBoard(computerBoard);

    for(i = 0; i < numberOfShips; i++) {
        startRowShip = generateRandomNumber(0,rows / 2);
        startColShip = generateRandomNumber(0,cols / 2);
        lenShip = generateRandomNumber(0,rows / 2);

        if(shipDirection == 'h') {
            shipDirection = 'v';
            if(!placeShipHorizontally(startRowShip,startColShip,lenShip,computerBoard,0)) {
                i--;
                continue;
            }
        } else if(shipDirection == 'v') {
            shipDirection = 'h';
            if(!placeShipVertically(startRowShip,startColShip,lenShip,computerBoard,0)) {
                i--;
                continue;
            }
        }
        computerPointsRemaining += lenShip;
    }
    printBoard(computerBoard);
}

void playGame(int userBoard[rows][cols],int computerBoard[rows][cols]) {
    //1 for player turn
    //-1 for computer turn
    int playerTurn = 1;
    char playerMessage[MESSAGELEN] = "Enemy";
    char computerMessage[MESSAGELEN] = "Your";

    //when one player's points go to zero he loses
    while(userPointsRemaining >0 && computerPointsRemaining > 0) {
        if(playerTurn == 1) {
            //ask player for coordinates he shoots at computerBoard the place where he shoots becomes -1
            printBoardDuringGame(computerBoard,playerMessage);
            playerShoots(computerBoard);
        } else if(playerTurn == -1) {
            //computer shoots at player board and shows where he shot and the condition of our board
            computerShoots(userBoard);
            printBoardDuringGame(userBoard,computerMessage);
            printf("User remaining points: %d\n",userPointsRemaining);
        }

        playerTurn *=-1;
    }
}

void playerShoots(int computerBoard[rows][cols]) {
    int row = rows + 1;
    int col = cols + 1;

    //make sure input is in bounds and also that we haven't shot in that place alredy
    //because if we keep shooting in the same place the game might go on forever
    while(!checkInputParameters(row,col) && computerBoard[row][col] != -1) {
        printf("Take your shot at:");
        scanf("%d %d",&row,&col);
        if(!checkInputParameters(row,col)){
            printf("%s\n",INVALIDSHIPPARAMETERS);
        }
        if(computerBoard[row][col] == -1){
            printf("%s\n",ALREDYSHOTTHERE);
        }
    }

    //there is point there make it -1
    if(computerBoard[row][col] == 1) {
        computerBoard[row][col] = -1;
        computerPointsRemaining--;
    }

    //do nothing because the cell is empty
    if(computerBoard[row][col] == 0) {
    }
}

void computerShoots(int userBoard[rows][cols]) {
    int row = generateRandomNumber(0,rows-2);
    int col = generateRandomNumber(0,cols-2);

    //make sure that the computer shoots at a cell that he hasn't shot before
    while(userBoard[row][col] != -1) {
        row = generateRandomNumber(0,rows-2);
        col = generateRandomNumber(0,cols-2);
    }

    if(userBoard[row][col] == 1) {
        userBoard[row][col] = -1;
        userPointsRemaining--;
    }

    //do nothing because the cell is empty
    if(userBoard[row][col] == 0) {
    }
}

void printBoardDuringGame(int board[rows][cols],char message[MESSAGELEN]) {
    //separating the rows with ---- and cols with | to be able to read the boards better
    //2*cols because on every iteration we print one char with |
    char *rowDelimeter = malloc(2*cols+1);
    memset(rowDelimeter,'-',2*cols);
    rowDelimeter[2*cols] = '\0';

    printf("%s Board:\n",message);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(board[i][j] != -1) {
                printf("*|");
            } else {
                printf("%d|",board[i][j]); // -1
            }
        }
        printf("\n%s\n",rowDelimeter);
    }
    printf("%s",SEPARATOR);
}

int generateRandomNumber(int lower, int upper) {
    //add 1 if rows and cols 1 -> they will become zero
    return (rand() % (upper - lower)) + lower + 1;
}

void printBoard(int userBoard[rows][cols]) {
    //0 - no ship
    //1 - ship
    //-1 - hit cell
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("%d ",userBoard[i][j]);
        }
        printf("\n");
    }
    printf("%s",SEPARATOR);
}

int getNumberOfShips() {
    //if one ships is exactly of length 1 the max ships we can build is rows * cols
    int maxShips = rows * cols;
    int numberOfShips = maxShips + 1;

    while(numberOfShips > maxShips) {
        printf("How many ships on your board: ");
        scanf("%d",&numberOfShips);
        if(numberOfShips > maxShips) {
            printf("%s\n",INVALIDSHIPS);
        }
    }
    return numberOfShips;
}

void putDefaultValuesOnBoard(int userBoard[rows][cols]) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            userBoard[i][j] = 0;
        }
    }
}

int checkInputParameters(int startRowShip,int startColShip) {
    if(startColShip < 0 || startColShip >= cols || startRowShip < 0 || startRowShip >= rows) {
        return 0;
    }
    return 1;
}

int checkShipDirection(char fullShipDirection[SHIPDIRECTION]) {
    //lower the direction in case the user inputted uppercase
    for(int j= 0; j < strlen(fullShipDirection); j++) {
        fullShipDirection[j] = (fullShipDirection[j] <= 'Z' && fullShipDirection[j] >= 'A') ?
                               fullShipDirection[j] + 32 : fullShipDirection[j];
    }

    //if direction word isn't horizontal or vertical get new ship
    if(strcmp(fullShipDirection,"horizontal") && strcmp(fullShipDirection,"vertical") &&
            strcmp(fullShipDirection,"h") && strcmp(fullShipDirection,"v")) {
        printf("%s\n",INVALIDDIRECTION);
        return 0;
    }
    return 1;
}

int placeShipHorizontally(int startRowShip,int startColShip,int lenShip,int board[rows][cols],int currentIsUser) {
    //remove one because the startingCell is of length 1
    //if the function is called from inputing the table for the user then print messages if it is called
    //for the computer don't print

    int i;
    int endColShip = startColShip + lenShip - 1;
    if(endColShip >= cols) {
        if(currentIsUser) {
            printf("%s\n",INVALIDSHIPPARAMETERS);
        }
        return 0;
    }

    //ship alredy placed there
    for(i = startColShip; i <= endColShip; i++) {
        if(board[startRowShip][i] == 1) {
            if(currentIsUser) {
                printf("%s\n",SHIPALREDYTHERE);
            }
            return 0;
        }
    }

    //place ship
    for(i = startColShip; i <= endColShip; i++) {
        board[startRowShip][i] = 1;
    }
    return 1;
}

int placeShipVertically(int startRowShip,int startColShip,int lenShip,int board[rows][cols],int currentIsUser) {
    //remove one because the startingCell is of length 1
    int i;
    int endRowShip = startRowShip + lenShip - 1;
    if(endRowShip >= rows) {
        if(currentIsUser) {
            printf("%s\n",INVALIDSHIPPARAMETERS);
        }
        return 0;
    }
    //ship alredy placed there
    for(i = startRowShip; i <= endRowShip; i++) {
        if(board[i][startColShip] == 1) {
            if(currentIsUser) {
                printf("%s\n",SHIPALREDYTHERE);
            }
            return 0;
        }
    }

    for(i = startRowShip; i <= endRowShip; i++) {
        board[i][startColShip] = 1;
    }
    return 1;
}

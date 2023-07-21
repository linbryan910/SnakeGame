#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <windows.h>
#include <pthread.h>

// Snake Linked List struct
struct snakePart{
    int position[2]; // 2d coordinate
    char direction; // up: u, down: d, left: l, right: r
    struct snakePart* next;
};

// Board
int board[6][6];
int boardLength = 6;

// Snake
struct snakePart *head;
int snakeLen;

// Apple
int apple[2];

// Data about game speed
int snakeSpeed = 500; // Speed of snake (in ms)
int moveSpeed = 100; // Speed of move input (in s)

// Updates position of snake and apple on board
void updateBoard(){
    // Resets board
    for(int i = 0; i < boardLength; i ++){
        for(int j = 0; j < boardLength; j ++){
            board[i][j] = 0;
        }
    }

    // Adds apple to board
    board[apple[0]][apple[1]] = -1;

    // Adds snake to board
    struct snakePart *iter = head;
    while(iter != NULL){
        switch(iter->direction){
            case 'u':
                board[iter->position[0]][iter->position[1]] = 1;
                break;
            case 'd':
                board[iter->position[0]][iter->position[1]] = 2;
                break;
            case 'l':
                board[iter->position[0]][iter->position[1]] = 3;
                break;
            case 'r':
                board[iter->position[0]][iter->position[1]] = 4;
                break;
        }

        iter = iter->next;
    }
}

// Displays the board as output
void displayBoard(){
    // Clears output
    system("cls");

    // Displays top border of board
    for(int i = 0; i < boardLength * 2.5; i ++){
        printf("_");
    }
    printf("\n");

    // Displays body of board
    for(int i = 0; i < boardLength; i ++){
        // Left border of board
        printf("|");

        // Displays contents of board
        for(int j = 0; j < boardLength; j ++){
            switch(board[i][j]){
                case 0:
                    printf("  ");
                    break;
                case -1:
                    printf("A ");
                    break;
                case 1:
                    printf("^ ");
                    break;
                case 2:
                    printf("v ");
                    break;
                case 3:
                    printf("< ");
                    break;
                case 4:
                    printf("> ");
                    break;
            }
        }

        // Right border of board
        printf("|");

        printf("\n");
    }

    // Displays right border of board
    for(int i = 0; i < boardLength * 2.5; i ++){
        printf("_");
    }
    printf("\n");
}

// Generates an apple
void generateApple(){
    // Checks if snake covers entire board (player wins)
    if(snakeLen >= pow(boardLength, 2)){
        updateBoard();
        displayBoard();
        printf("Congratulations! You win");
        exit(0);
    }

    // Fields for generating new apple
    int i, j;
    struct snakePart *iter;

    while(1){
        // Generates random position
        i = rand() % boardLength;
        j = rand() % boardLength;

        // Checks that apple isn't generated on snake
        iter = head;
        while(iter != NULL){
            // If it is, restart
            if(i == iter->position[0] && j == iter->position[1]){
                break;
            }
            else{
                iter = iter->next;
            }
        }

        // If apple isn't generated on snake, set its new position
        if(iter == NULL){
            apple[0] = i;
            apple[1] = j;
            return;
        }
    }
}


// Checks if snake dies
void checkDeath(){
    // Checks if snake slithers into itself
    struct snakePart *iter = head->next;
    while(iter != NULL){
        if(head->position[0] == iter->position[0] && head->position[1] == iter->position[1]){
            printf("Snake dies\n");
            exit(0);
        }
        iter = iter->next;
    }

    // Checks if snake slithers into wall
    if(head->position[0] < 0 || head->position[0] > boardLength - 1 || head->position[1] < 0 || head->position[1] > boardLength - 1){
        printf("Snake dies\n");
        exit(0);
    }
}

// Moves the snake forwards
void moveSnake(int ifGrow){
    // Initializes fields for snake position update
    int pos0, pos1, temp0, temp1;
    char dir, tempDir;
    struct snakePart *iter = head;

    // Begins update for head (specifically on head)
    pos0 = iter->position[0];
    pos1 = iter->position[1];
    dir = iter->direction;
    switch(head->direction){
        case 'u': // Up
            iter->position[0] = iter->position[0] - 1;
            iter->direction = 'u';
            break;
        case 'd': // Down
            iter->position[0] = iter->position[0] + 1;
            iter->direction = 'd';
            break;
        case 'l': // Left
            iter->position[1] = iter->position[1] - 1;
            iter->direction = 'l';
            break;
        case 'r': // Right
            iter->position[1] = iter->position[1] + 1;
            iter->direction = 'r';
            break;
    }
    iter = iter->next;

    // Updates position of rest of snake
    while(iter != NULL){
        temp0 = pos0;
        temp1 = pos1;
        tempDir = dir;

        pos0 = iter->position[0];
        pos1 = iter->position[1];
        dir = iter->direction;

        iter->position[0] = temp0;
        iter->position[1] = temp1;
        iter->direction = tempDir;

        iter = iter->next;
    }

    // If snake ate apple next move, add part to end of snake
    if(ifGrow == 1){
        struct snakePart *newPart = (struct snakePart *) malloc(sizeof(struct snakePart));
        newPart->position[0] = pos0;
        newPart->position[1] = pos1;
        newPart->direction = dir;
        newPart->next = NULL;
        struct snakePart *iter1 = head;
        while(iter1->next != NULL){
            iter1 = iter1->next;
        }
        iter1->next = newPart;

        snakeLen ++;
    }

    // Checks if snake dies
    checkDeath();

    // If snake eats apple:
    //   New apple generated
    //   Snakes grows with user move
    if(head->position[0] == apple[0] && head->position[1] == apple[1]){
        updateBoard();
        displayBoard();
        Sleep(snakeSpeed);
        generateApple();
        updateBoard();
        displayBoard();
        moveSnake(1);
    }
}

// Initializes game data
void initializeGame(){
    // Initializes board
    for(int i = 0; i < boardLength; i ++) {
        for (int j = 0; j < boardLength; j++) {
            board[i][j] = 0;
        }
    }

    // Initializes snake
    struct snakePart * newPart = (struct snakePart *)malloc(sizeof(struct snakePart));
    newPart->position[0] = 0;
    newPart->position[1] = 0;
    newPart->direction = 'r';
    newPart->next = NULL;
    struct snakePart * newPart1 = (struct snakePart *)malloc(sizeof(struct snakePart));
    newPart1->position[0] = 0;
    newPart1->position[1] = 1;
    newPart1->direction = 'r';
    newPart1->next = newPart;
    struct snakePart * newPart2 = (struct snakePart *)malloc(sizeof(struct snakePart));
    newPart2->position[0] = 0;
    newPart2->position[1] = 2;
    newPart2->direction = 'r';
    newPart2->next = newPart1;
    snakeLen = 3;

    // Sets head pointer
    head = newPart2;

    // Initializes apple
    generateApple();
}

// Checks for player's moves
void *getInput(){
    while(1){
        if(_kbhit() != 0){
            _getch();
            int move = _getch();

            switch(move){
                case 72: // Up
                    if(head->direction != 'd'){
                        head->direction = 'u';
                    }
                    break;
                case 80: // Down
                    if(head->direction != 'u'){
                        head->direction = 'd';
                    }
                    break;
                case 75: // Left
                    if(head->direction != 'r'){
                        head->direction = 'l';
                    }
                    break;
                case 77: // Right
                    if(head->direction != 'l'){
                        head->direction = 'r';
                    }
                    break;
            }

            Sleep(snakeSpeed);
        }
        else{
            Sleep(moveSpeed);
        }
    }
}

// Progresses the game state
void *progressGame(){
    while(1){
        updateBoard();

        displayBoard();

        Sleep(snakeSpeed);

        moveSnake(0);
    }
}

int main() {
    initializeGame();

    // Threads
    pthread_t thread1, thread2;

    // Creates and runs threads
    pthread_create(&thread1, NULL, progressGame, NULL);
    pthread_create(&thread2, NULL, getInput, NULL);
    pthread_join(thread1, NULL);

    return 0;
}
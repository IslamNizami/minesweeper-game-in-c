#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>


/* Enum for cell states */
typedef enum{
    HIDDEN = 0, // cell is not revealed
    REVEALED, // cell is revealed
    FLAGGED // cell is flagged
}CellState;


/* Struct for each cell */
typedef struct{
    int isMine; // 1 if mine present
    int adjacentMines; // number of mines nearby
    CellState state;
}Cell;

/* ------------Function protoypes---------- */
Cell** createField(int rows, int cols);
void placeMines(Cell** field, int rows, int cols, int mineCount);
void calculateNumbers(Cell** field, int rows, int cols);
void printField(Cell** field, int rows, int cols, int revealAll);
int isValid(int r, int c, int rows, int cols);
void revealCell(Cell** field, int rows, int cols, int r, int c);
void floodFill(Cell** field, int rows, int cols, int r, int c);
int checkWin(Cell** field, int rows, int cols, int mineCount);
void saveScore(int score, double timeElapsed);
void freeField(Cell** field, int rows);
void printOpeningText();

/* ------------Implementations---------- */
int main(){
    int rows;
    int cols;
    int gameOver = 0;
    int mineCount;
    int r;
    int c;
    char action;
    time_t startTime, endTime;

    printOpeningText();

    printf("Enter number of rows: ");
    scanf("%d",&rows);
    printf("Enter number of columns: ");
    scanf("%d",&cols);
    printf("Enter number of mines: ");
    scanf("%d",&mineCount);

    Cell** field = createField(rows,cols);
    placeMines(field,rows,cols,mineCount);
    calculateNumbers(field,rows,cols);

    time(&startTime);

    while(!gameOver){
        printField(field,rows,cols,0);
        printf("Enter action (r=Reveal, f=Flag) and coordinates (row col): ");
        scanf(" %c %d %d",&action,&r,&c);

        if(!isValid(r,c,rows,cols)){
            printf("Invalid coordinates!\n");
            continue;
        }

        if(action == 'f'){
            if(field[r][c].state == HIDDEN){
                field[r][c].state = FLAGGED;
            }else if(field[r][c].state == FLAGGED){
                field[r][c].state = HIDDEN;
            }
        }else if(action == 'r'){
            if(field[r][c].isMine){
                printf("BOOM! You hit a mine!\n");
                gameOver = 1;
                printField(field,rows,cols,1);
                time(&endTime);
                double elapsedTime = difftime(endTime,startTime);
                saveScore(0,elapsedTime);
            }else{
                revealCell(field,rows,cols,r,c);
                if(checkWin(field,rows,cols,mineCount)){
                    time(&endTime);
                    double elapsedTime = difftime(endTime,startTime);
                    printf("Congratulations! You cleared the field!\n");
                    printf("Time taken: %.2f seconds.\n",elapsedTime);
                    saveScore(1,elapsedTime);
                    printField(field,rows,cols,1);
                    gameOver = 1;
                }
            }
        }
    }
    freeField(field,rows);
    return 0;
}

Cell** createField(int rows, int cols){
    Cell** field = (Cell**)malloc(rows * sizeof(Cell*));
    for(int i = 0; i < rows; i++){
        field[i] = (Cell*)malloc(sizeof(Cell) * cols);

        for(int j = 0; j < cols; j++){
            field[i][j].isMine = 0;
            field[i][j].adjacentMines = 0;
            field[i][j].state = HIDDEN;
        }
    }
    return field;
}

void placeMines(Cell** field, int rows, int cols, int mineCount){
    srand(time(NULL));
    int placed = 0;

    while(placed < mineCount){
        int r = rand() % rows;
        int c = rand() % cols;
        if(!field[r][c].isMine){
            field[r][c].isMine = 1;
            placed++;
        }
    }
}

/* Calculate numbers for each cell */
void calculateNumbers(Cell** field, int rows, int cols){
    int dr[] = {-1,-1,-1,0,0,1,1,1}; //delta row
    int dc[] = {-1,0,1,-1,1,-1,0,1}; // delta column
    for(int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            if(field[r][c].isMine){
                continue;
            }
            int count = 0;
            for(int i = 0; i < 8; i++){
                int nr = r + dr[i];
                int nc = c + dc[i];
                if(isValid(nr,nc,rows,cols) && field[nr][nc].isMine){
                    count++;
                }
            }
            field[r][c].adjacentMines = count;
        }
    }
}


//Print the Grid
void printField(Cell** field, int rows, int cols, int revealAll) {
    // Print column headers
    printf("    ");
    for (int c = 0; c < cols; c++) {
        printf(" %2d ", c);
    }
    printf("\n");

    for (int r = 0; r < rows; r++) {
        // Print top border of the row
        printf("   ");
        for (int c = 0; c < cols; c++) {
            printf("+---");
        }
        printf("+\n");

        // Print row number and contents
        printf("%2d ", r);
        for (int c = 0; c < cols; c++) {
            char ch;
            if (revealAll) {
                if (field[r][c].isMine) {
                    ch = '*';
                } else if (field[r][c].adjacentMines == 0) {
                    ch = ' ';
                } else {
                    ch = '0' + field[r][c].adjacentMines;
                }
            } else {
                if (field[r][c].state == HIDDEN) {
                    ch = ' ';
                } else if (field[r][c].state == FLAGGED) {
                    ch = 'F';
                } else if (field[r][c].adjacentMines == 0) {
                    ch = ' ';
                } else {
                    ch = '0' + field[r][c].adjacentMines;
                }
            }
            printf("| %c ", ch);
        }
        printf("|\n");
    }

    // Bottom border after last row
    printf("   ");
    for (int c = 0; c < cols; c++) {
        printf("+---");
    }
    printf("+\n");
}


/* Check if position is valid */
int isValid(int r, int c, int rows, int cols){
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

/* Reveal cell, and auto flood fill if empty */
void revealCell(Cell** field, int rows, int cols, int r, int c){
    if(!isValid(r,c,rows,cols)) return;
    if(field[r][c].state != HIDDEN) return;
    if(field[r][c].isMine) return;

    floodFill(field, rows, cols, r, c);
}


/* Flood fill to reveal mine-free regions */
void floodFill(Cell** field, int rows, int cols, int r, int c){
   if(!isValid(r,c,rows,cols)) return;
   if(field[r][c].state != HIDDEN) return;
   if(field[r][c].isMine) return;

   field[r][c].state = REVEALED; 
   
   if(field[r][c].adjacentMines == 0){
        int dr[] = {-1,-1,-1,0,0,1,1,1};
        int dc[] = {-1,0,1,-1,1,-1,0,1};
        for(int i = 0; i < 8; i++){
            int nr = r + dr[i];
            int nc = c + dc[i];
            floodFill(field,rows,cols,nr,nc);
        }
   }

}

/* Check win condition */
int checkWin(Cell** field, int rows, int cols, int mineCount){
    int revealedCount = 0;
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            if(field[r][c].state == REVEALED){
                revealedCount++;
            }
        }
    }

    return revealedCount == (rows * cols - mineCount);
}

/* Save score to file */
void saveScore(int score, double timeElapsed){
    FILE* file = fopen("scores.txt","a");
    char name[100];
    printf("What is your name: ");
    scanf("%s",name);

    if(file){
        fprintf(file,"Name: %s Score: %d, Time: %.2f seconds\n",name,score,timeElapsed);
        fclose(file);
    }
}

/* Free allocated memory */
void freeField(Cell** field, int rows){
    for(int i = 0; i < rows; i++){
        free(field[i]);
    }
    free(field);
}


void printOpeningText() {
    printf("========================================\n");
    printf("        M I N E S W E E P E R\n");
    printf("========================================\n");
    printf("Welcome to the classic game of Minesweeper!\n");
    printf("\n");
    printf("Rules:\n");
    printf(" 1. The board is filled with hidden mines.\n");
    printf(" 2. Your goal is to reveal all safe cells without hitting a mine.\n");
    printf(" 3. Each revealed cell shows the number of mines in the 8\n");
    printf("    surrounding cells. A '0' means no adjacent mines.\n");
    printf(" 4. You can flag suspected mine cells to avoid clicking them.\n");
    printf("\n");
    printf("Controls:\n");
    printf("  r <row> <col>  -> Reveal cell at (row, col)\n");
    printf("  f <row> <col>  -> Flag/unflag cell at (row, col)\n");
    printf("\n");
    printf("Good luck, and watch your step!\n");
    printf("========================================\n\n");
}
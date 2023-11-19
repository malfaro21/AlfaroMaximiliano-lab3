#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;
void* validate_row(void* param);
void* validate_column(void* param);
void* validate_subgrid(void* param);

int** read_board_from_file(char* filename){
    FILE *fp = fopen(filename, "r");

    if(fp == NULL){
        return NULL;
    }
    int** board = (int**)malloc(sizeof(int*)*ROW_SIZE);
    
    if(board == NULL){
        fclose(fp);
        return NULL;
    }
    for(int row = 0; row<ROW_SIZE; row++){
        board[row] = (int*)malloc(sizeof(int)* COL_SIZE);
        for(int col = 0; col<COL_SIZE; col++){
            fscanf(fp, "%d", &board[row][col]);
        }
    }
    fclose(fp);
    return board;
}

void* validate_row(void* param){
    param_struct* params = (param_struct*) param;
    int start_row = params->starting_row;
    int end_row = params->ending_row;
    int result = 1;
    for(int col = params-> starting_col; col <= params->ending_col; col++){
        for(int other_col = col + 1; other_col<=params->ending_col;other_col++){
            if(sudoku_board[start_row][col] == sudoku_board[start_row][other_col]){
                result = 0;
                break;
            }
        }
        if(result == 0){
            break;
        }
    }
    worker_validation[params->id] = result;
    pthread_exit(NULL);
}


int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;
    
    // replace this comment with your code
    
}

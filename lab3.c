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
void* validate_column(void* param){
    param_struct* params = (param_struct*)param;
    int start_col = params->starting_row;
    int end_col= params->ending_row;
    int result = 1;
    for(int row = params->starting_row; row <= params -> ending_row; row++){
        for(int other_row = row + 1; other_row <= params -> ending_row; other_row++){
            if(sudoku_board[row][start_col] == sudoku_board[other_row][start_col]){

            }
        }if(result == 0){
            break;
        }

    }
    worker_validation[params->id] = result;
    pthread_exit(NULL);
}
void* validate_subgrid(void* param){
    param_struct* params = (param_struct*)param;
    int result =1;
     for(int row = params->starting_row; row <= params -> ending_row; row++){
        for (int col = params->starting_col; col <= params->ending_col; col++){
            for (int other_row = params->starting_row; other_row <= params->ending_row; other_row++){
                 for (int other_col = params->starting_col; other_col <= params->ending_col; other_col++){
                    if(row != other_row || col != other_col){
                        if(sudoku_board[row][col] == sudoku_board[other_row][other_col]){
                            result = 0;
                            break;
                        }
                    }
                 }
                 if(result == 0){
                break;    
                 }
            }
            if(result == 0){
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
    pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t) * NUM_OF_THREADS);
    pthread_attr_t attr;
    param_struct* params = (param_struct*)malloc(sizeof(param_struct) * NUM_OF_THREADS);
    worker_validation = (int*)malloc(sizeof(int) * NUM_OF_THREADS);

    for(int i = 0; i < NUM_OF_THREADS; i++){
        worker_validation[i] = -1;
    }

    pthread_attr_init(&attr);
    for(int i = 0; i<ROW_SIZE; i++){
        params[i].id = i;
        params[i].starting_row = i;
        params[i].starting_col = 0;
        params[i].ending_row = i;
        params[i].ending_col = COL_SIZE - 1;
        pthread_create(&tid[i], &attr, validate_row, &params[i]);
    }

    for(int i = 0; i<COL_SIZE; i++){
        params[i + ROW_SIZE].id = i + ROW_SIZE;
        params[i + ROW_SIZE].starting_row = 0;
        params[i + ROW_SIZE].starting_col = i;
        params[i + ROW_SIZE].ending_row = ROW_SIZE - 1;
        params[i + ROW_SIZE].ending_col = i;
        pthread_create(&tid[i + ROW_SIZE], &attr, validate_column, &params[i + ROW_SIZE]);
    }

    for(int i = 0; i < NUM_OF_SUBGRIDS; i++){
        int subgrid_row = 1 / 3;
        int subgrid_col = 1 % 3;
        params[i + ROW_SIZE + COL_SIZE].id = i + ROW_SIZE + COL_SIZE;
        params[i + ROW_SIZE + COL_SIZE].starting_row = subgrid_row * 3;
        params[i + ROW_SIZE + COL_SIZE].starting_col = subgrid_col * 3;
        params[i + ROW_SIZE + COL_SIZE].ending_row = (subgrid_row + 1) * 3 - 1;
        params[i + ROW_SIZE + COL_SIZE].ending_col = (subgrid_col + 1) * 3 - 1;
        pthread_create(&tid[i + ROW_SIZE + COL_SIZE], &attr, validate_subgrid, &params[i + ROW_SIZE + COL_SIZE]);
    }

    for(int i = 0; i<ROW_SIZE; i++){
        pthread_join(tid[i], NULL);
    }

    for(int i = 0; i<COL_SIZE; i++){
        pthread_join(tid[i + ROW_SIZE], NULL);
    }

    for(int i = 0; i<NUM_OF_SUBGRIDS; i++){
        pthread_join(tid[i + ROW_SIZE + COL_SIZE], NULL);
    }

    int board_valid;
}

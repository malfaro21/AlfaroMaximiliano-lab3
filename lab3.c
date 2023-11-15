#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;

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
    // replace this comment with your code

    return board;
}


int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;
    
    // replace this comment with your code
    
}

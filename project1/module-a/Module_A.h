#ifndef PROJECT1_H
#define PROJECT1_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <time.h>

#define ROWS_COUNT 16
#define COLUMN_COUNT 16


void print_matrix_rows(int *matrix, int start_row, int end_row){

    for (int i = start_row; i <= end_row; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            printf("%d\t",*(matrix+i*COLUMN_COUNT+j));
        }
        printf("\n");
    }
}


#endif // PROJECT1_H

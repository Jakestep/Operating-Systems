#include "Project1_Module_B.h"

int main(int argc, char *argv[]) {

    if (argc != 6) {
        printf("Usage: %s <starting_row_number> <number_of_rows_to_sum> shmid_matrix shmid_partial_sum partial_sum_index\n", argv[0]);
        return 1;
    }

    int start_row_id = atoi(argv[1]);
    int end_row_id = start_row_id + atoi(argv[2])-1;
    int shmid_matrix = atoi(argv[3]);
    int shmid_partial_sum = atoi(argv[4]);
    int partial_sum_index = atoi(argv[5]);

    if ((start_row_id < 0) ||(start_row_id>=ROWS_COUNT)) {
        printf("Invalid starting row id: %d\n", start_row_id);
        return 1;
    }

    int *matrix = (int *)shmat(shmid_matrix, NULL, 0); // attach the shared memory segment for the matrix to the address space
    long *partial_sums = (long *)shmat(shmid_partial_sum, NULL, 0); // attach the shared memory segment for the partial sums to the address space

    long partial_sum = sum_matrix(start_row_id, end_row_id, matrix); //use sum_matrix() function to calculate the sum of all elements from start_row to end_row
    partial_sums[partial_sum_index] = partial_sum; // store the calculated partial sum in the shared memory segment for the partial sums

    shmdt(matrix); // detach the shared memory segment for the matrix
    shmdt(partial_sums); // detach the shared memory segment for the partial sums

    exit(0);
}

/**
 * Jacob Estep -- 800818397
 * Austin Voight-Malone -- 800878160
 */

#include "Project1_Module_B.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        printf("Invalid number of processes: %d\n", num_processes);
        return 1;
    }

    struct timespec start, end;
    long long seconds, nano_seconds;

    int shmatrix = shmget(IPC_PRIVATE, ROWS_COUNT * COLUMN_COUNT * sizeof(int), IPC_CREAT | 0666);  // create a shared memory segment for the matrix
    int (*matrix)[COLUMN_COUNT] = (int (*)[COLUMN_COUNT])shmat(shmatrix, NULL, 0);  // attach the shared memory segment to the address space

    int partial_sums_shm = shmget(IPC_PRIVATE, num_processes * sizeof(long), IPC_CREAT | 0666);  // create a shared memory segment for the partial sums
    long *partial_sums = (long *)shmat(partial_sums_shm, NULL, 0);  // attach the shared memory segment to the address space

    // initialize each element of the matrix to the sum of its row and column indices
    for (int i = 0; i < ROWS_COUNT; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            matrix[i][j] = i + j;
        }
    }

    clock_gettime(CLOCK_REALTIME, &start);

    int childRows = ROWS_COUNT / num_processes; // num of rows each child process will handle
    int remainingRows = ROWS_COUNT % num_processes; // calculate any remaining rows that need to be distributed
    int startRow = 0; // initialize the starting row; used to keep track of what row each child process should start with

    for (int i = 0; i < num_processes; i++) {

        int numRows = childRows; // the number of rows this child process will handle

        if (i == num_processes - 1) { // if this is the last child process assign the remaining rows to it
            numRows += remainingRows;
        }

        int currStart = startRow; // store the current starting row for this child process
        int currEnd = currStart + numRows - 1; // calculate the ending row for this child process
        startRow = currEnd + 1; // update the starting row for the next child process

        pid_t pid = fork();

        if (pid == 0) { // child process

            char matrix_id[32], start_row[32], num_rows[32], partial_sum_index[32], partialsum_location[32]; // char arrays to hold matrix id, start row, # of rows, partial sum index, and memory location of the shared memory segment

            // convert the integer values to strings so we can pass them as arguments using execlp
            sprintf(matrix_id, "%d", shmatrix); // convert the shared memory id for the matrix to a string
            sprintf(start_row, "%d", currStart); // convert the starting row for this child process to a string
            sprintf(num_rows, "%d", numRows); // convert the number of rows for this child process to a string
            sprintf(partial_sum_index, "%d", i); // convert the index for the partial sum in shared memory to a string
            sprintf(partialsum_location, "%d", partial_sums_shm); // convert the shared memory id for the partial sums to a string

            execlp("./child", "child", start_row, num_rows, matrix_id, partialsum_location, partial_sum_index, (char *)NULL); // execute child_proc with the appropriate arguments
            perror("execlp failed"); // if execlp fails, print an error message
            exit(1); // exit with an error code

        } else if (pid < 0) { // fork failed
            perror("Fork failed");
            exit(1);
        }
    }

    // Wait for all child processes to complete
    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    long totalSum = 0; // variable to hold the total sum of the matrix

    for (int i = 0; i < num_processes; i++) {
        totalSum += partial_sums[i]; // add the partial sum from each child process to the total sum
    }

    printf("Total sum of the matrix: %ld\n", totalSum); // print the total sum of the matrix

    clock_gettime(CLOCK_REALTIME, &end);
    // Calculate execution time
    seconds=end.tv_sec - start.tv_sec;
    nano_seconds=end.tv_nsec - start.tv_nsec;
    long double executio_time=(long double)seconds+((long double)nano_seconds/1000000000);
    printf("Execution Time with %d process(es): %Lf\n",num_processes,executio_time);

    shmdt(matrix); // detach the shared memory segment for the matrix
    shmdt(partial_sums); // detach the shared memory segment for the partial sums

    shmctl(shmatrix, IPC_RMID, NULL); // destroy the shared memory segment for the matrix
    shmctl(partial_sums_shm, IPC_RMID, NULL); // destroy the shared memory segment for the partial sums

    return 0;
}



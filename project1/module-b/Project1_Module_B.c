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

    /* ToDo1
     * Create and attach shared memory region 1 to store the matrix
     * 
     * Hint: Google shmget() and shmat()
     */
    int shmatrix = shmget(IPC_PRIVATE, ROWS_COUNT * COLUMN_COUNT * sizeof(int), IPC_CREAT | 0666);  // create a shared memory segment for the matrix
    int (*matrix)[COLUMN_COUNT] = (int (*)[COLUMN_COUNT])shmat(shmatrix, NULL, 0);  // attach the shared memory segment to the address space




    /* ToDo2
     * create and attach shared memory region 2 to store the partial sums that
     * are returned by children processes.
     */
    int partial_sums_shm = shmget(IPC_PRIVATE, num_processes * sizeof(int), IPC_CREAT | 0666);  // create a shared memory segment for the partial sums
    int *partial_sums = (int *)shmat(partial_sums_shm, NULL, 0);  // attach the shared memory segment to the address space


    /* ToDo3
     * Initialize the shared memory region 1 to hold
     * the matrix of size ROW_COUNT x COLUMN_COUNT.
     * Value of each element is set to i+j, where i is index of row
     * and j is index of column.
     * 
     * Hint: Treat starting address of shared memory region as first
     * elemnt of a 2D matrix i.e.,  matrix[0][0]
    */

    // initialize each element of the matrix to the sum of its row and column indices
    for (int i = 0; i < ROWS_COUNT; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            matrix[i][j] = i + j;
        }
    }

    clock_gettime(CLOCK_REALTIME, &start);


    /* ToDo4:
     * 1) Create num_processes count of children processes. (using fork). Note that each child process runs a new binary 
     * (i.e., binary of child_proc.c) using execlp()
     * 2) Specify for each process the start_row, end_row, and
     * index in shared memory region 2 where a child process is supposed to store its
     * calculated partial sum.
     *
     *
     * Hints:
     *
     * Google "man fork" and "man execlp"
     *
    */

    // for (i = 0; i < num_processes; i++) {

    //     pid_t pid = fork();

    //     if (pid == 0) { // child process
    //     }

    // }



    /* ToDo5
     * Wait for all children processes to complete
    */
    for (int i = 0; i < num_processes; i++) {
        wait(NULL); // wait for each child process to complete
    }


    /* ToDo6:
     * Sum all the partial-sums stored in sharred memory region 2 (by children processes)
     * and print the sum.
    */


    clock_gettime(CLOCK_REALTIME, &end);
    // Calculate execution time
    seconds=end.tv_sec - start.tv_sec;
    nano_seconds=end.tv_nsec - start.tv_nsec;
    long double executio_time=(long double)seconds+((long double)nano_seconds/1000000000);
    printf("Execution Time with %d process(es): %Lf\n",num_processes,executio_time);


    /* ToDo7:
     * Detach and destroy both shared memory regions.
     * Hint:
     * Google "man shmdt" and "man shmctl"
    */




    return 0;
}



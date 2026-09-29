#include "Module_A.h"





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


    /* ToDo 1:
     * Dynamically allocate memory of size ROWS_COUNT x COLUMN_COUNT x size_of(int).
     * run "man malloc" on ubuntu/linux terminal or see online help about malloc function
     */
    int *matrix = (int *)malloc(ROWS_COUNT * COLUMN_COUNT * sizeof(int));
    if (matrix == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    

    /* ToDo 2:
     * Initialize the allocated memory region 1 to hold
     * the matrix of size ROW_COUNT x COLUMN_COUNT.
     * Value of each element is set to i+j, where i is index of row
     * and j is index of column.
    */
    for (int i = 0; i < ROWS_COUNT; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            matrix[i * COLUMN_COUNT + j] = i + j;
        }
    }

    
    int row_per_process=ROWS_COUNT/num_processes;
    int start_row=0;
    int end_row=start_row+row_per_process-1;

    clock_gettime(CLOCK_REALTIME, &start);

  
    
    for (int p = 0; p < num_processes; p++) {
        if (p == num_processes - 1) end_row = ROWS_COUNT - 1; // Adjust the end_row for the last process to include any remaining rows
        pid_t pid = fork(); // ToDo 3: Create num_processes count of children processes. (using fork)  
        if (pid < 0) {
            perror("Fork failed");
            free(matrix);
            return 1;
        }

        if (pid == 0) { // Child process
            print_matrix_rows(matrix, start_row, end_row); // ToDo 4: Ask each child process to print "row_per_process" number of rows by calling function print_matrix_rows(). See header file.
            free(matrix); // Free dynamically allocated memory in child process TODO 5
            exit(0); // Exit child process after printing and freeing memory TODO 6
        } else { // Parent process
            start_row += row_per_process;
            end_row += row_per_process;
        }
    }

    // ToDO 7: Parent process must wait for all children processes to complete
    for (int p = 0; p < num_processes; p++) {
        wait(NULL);
    }

    printf("all child processes completed their execution\n");


    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate execution time
    seconds=end.tv_sec - start.tv_sec;
    nano_seconds=end.tv_nsec - start.tv_nsec;
    long double executio_time=(long double)seconds+((long double)nano_seconds/1000000000);
    printf("Execution Time with %d process(es): %Lf\n",num_processes,executio_time);


    return 0;
}



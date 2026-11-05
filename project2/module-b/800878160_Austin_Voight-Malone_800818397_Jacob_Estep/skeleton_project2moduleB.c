/**
 * Jacob Estep -- 800818397
 * Austin Voight-Malone -- 800878160
 */


#include "project2ModB.h"


int main(int argc, char *argv[]) {
    
    
    if (argc != 5) {
        printf("Usage: %s input.pgm output.pgm threshold worker_thread_cnt\n", argv[0]);
        return 1;
    }

    char *input_file=argv[1];
    char *output_file=argv[2];
    threshold=atoi(argv[3]);
    int worker_thread_cnt=atoi(argv[4]);

    struct timespec start, end;    
	long long seconds, nano_seconds;    


    clock_gettime(CLOCK_REALTIME, &start); 

    /* 
    Open input file in read-only binary mode i.e. "rb"
    Hint: google "man fopen()"
    */

    FILE *Fin=fopen(input_file, "rb");

    /*
    Do error checking to ensure file was opened sucessfully before proceeding
    */
    if(Fin==NULL)
    {
        printf("Failed to open the input file\n");
        exit(0);
    }

    //read the first line just to advance the cursor in file but dont use the line.
    char Line[256];

    fgets(Line,256,Fin);
    // printf("Line: %s",Line);

    //read the width, and height of the file 
    
    int width, height, max_value;
    int return_val;
    
    fgets(Line, sizeof(Line), Fin); // P5
    sscanf(Line, "%d %d", &width, &height);

    //read the maximum pixel value;
    sscanf(Line, "%d", &max_value);
    fgetc(Fin); // consume the newline before pixel data

    if (height%worker_thread_cnt!=0){
        printf("Number of rows (%d) is not a multiple of worker_thread_cnt (%d)\n",height,worker_thread_cnt);
        fclose(Fin);
        exit(0);
    }

    /*
    Allocate memory to store width x height number of pixels for black and white image. 
    Note that each pixel has max value of 255, so an 8-bit char is sufficent to store the pixel value.
    Hint: google "man malloc";
    */

    char *image=malloc(width*height*1);
    

    /*
    TDo Error checking to make sure memory was successfully allocated before proceeding.
    In case of failure, exit the program.
    */

    if(image==NULL)
    {
        printf("Failure in dynamically allocating the required memory size\n");
        exit(0);
    }


    /*
    Read all the content of input image file into allocated memory.
    Hint: Google "man fread(3p)"
    */
    return_val=fread(
                    image,
                    1, 
                    width*height,
                    Fin);

    /*Do error checking to make sure that fread was sucessfull. If not, exit the program.
    Hoever, free the dynamically allocated memory before exiting.
    */
    if(return_val!=(width*height))
    {
        printf("Failure in reading input file into dymaically allocated memory\n");
        free(image);
        exit(0);
    }

    //close the input file
    fclose(Fin);

    /*
    ====================================ToDo 2:============================= 
    Create worker_thrd_cnt number of threads. 
    Each thread is responsible for doing the threshold operation on pixles of height/worker_thread_cnt number of rows. 
    */
   
    pthread_t threads[worker_thread_cnt];
    struct arguments thread_args[worker_thread_cnt];

    for (int i = 0; i < worker_thread_cnt; i++) {
        thread_args[i].image = image;
        thread_args[i].start = i * (width * (height / worker_thread_cnt));
        thread_args[i].end = (i + 1) * (width * (height / worker_thread_cnt));
        pthread_create(&threads[i], NULL, (void *(*)(void *))threshold_operation, &thread_args[i]);
    }


    /*
    ========================================ToDo 3:==============================
    Make the parent thread (the one that is generating new threads) wait for all threads to complete before proceeding.
    */
   for (int i = 0; i < worker_thread_cnt; i++) {
        pthread_join(threads[i], NULL);
    }
    


    /*
    open the ouput file
    Hint: google "man fopen"
    */

    FILE *Fout=fopen(output_file, "w+");

    /*
    Do error checking to make sure that output file is opened sucessfully. If not, exit the program.
    However, free the dynamically allocated memory before exiting.
    */
    if(Fout==NULL)
    {
        printf("Failure in opening the output file\n");
        free(image);
        exit(0);
    }


    fprintf(Fout, "P5\n%d %d\n255\n", width, height);
    /*
    Write output file with the content of dynamically allocated memory
    Hint: google "man fwrite(3p)"
    */
    return_val=fwrite(image,1,width*height,Fout);

    /*
    Do error checking to make sure that all bytes from allocated memory are written successfully to the output image file.
    If not, free the dynmaically allocated memory and exit the program.
    */
    if(return_val!=width*height)
    {
        printf("Failure in writing black and white image to output file\n");
        free(image);
        exit(0);
    }

    /*
    Close output file.
    */
    fclose(Fout);
    
    /*
    Free the memory that was allocated to store black and white image.
    Hint: Google "man free". 
    */
    free(image);

    clock_gettime(CLOCK_REALTIME, &end);    // Calculate execution time   	 	
    seconds=end.tv_sec - start.tv_sec;    
	nano_seconds=end.tv_nsec - start.tv_nsec;    
	long double executio_time=(long double)seconds+((long double)nano_seconds/1000000000);    
	// printf("Execution Time: %Lf\n",executio_time);
     printf("%Lf\n",executio_time);



    return 0;
}

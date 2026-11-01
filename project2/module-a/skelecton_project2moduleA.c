#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
    
    if (argc != 4) {
        printf("Usage: %s input.pgm output.pgm threshold\n", argv[0]);
        return 1;
    }

    char *input_file=argv[1];
    char *output_file=argv[2];
    int threshold=atoi(argv[3]);

    /* 
    ToDO 1: Open input file in read-only binary mode i.e. "rb"
    Hint: google "man fopen()"
    */

    /*
    ToDo 2: Do error checking to ensure file was opened sucessfully before proceeding
    */

    //======================Reading File Header=====================================
    //read the first line just to advance the cursor in file but dont use the line.
    char Line[256];

    fgets(Line,256,Fin);

    //read the width, and height of the file 
    
    int width, height, max_value;
    int return_val;
    
    fgets(Line, sizeof(Line), Fin); // P5
    sscanf(Line, "%d %d", &width, &height);

    //read the maximum pixel value;
    sscanf(Line, "%d", &max_value);
    fgetc(Fin); // consume the newline before pixel data

    //====================Done Reading File Header===================================
    /*
    ToDo 3: Allocate memory to store width x height number of pixels for black and white image. 
    Note that each pixel has max value of 255, so an 8-bit char is sufficent to store the pixel value.
    Hint: google "man malloc";
    */
    

    /*
    ToDo 4: Do Error checking to make sure memory was successfully allocated before proceeding.
    In case of failure, exit the program.
    */


    /*
    ToDo 5: read all the content of input image file into allocated memory.
    Hint: Google "man fread(3p)"
    */


    /*ToDo 6: Do error checking to make sure that fread was sucessfull. If not, exit the program.
    Hoever, free the dynamically allocated memory before exiting.
    */


    //ToDo 7: close the input file
    

    /*
    ToDo 8: Iterate over all bytes of the dynmaically allocated memory (where each byte stores one pixel of the input file) 
    and update its avlue by doing following operation.

    if (value of the byte is greater than or equal to threshold)
        {
        set value of the byte to 255.
        }
    else
        {
        set value of the byte to 0.
        }
    */


    /*
    ToDo 9: open the ouput file with "w+" permission
    Hint: google "man fopen"
    */


    /*
    ToDo 10: Do error checking to make sure that output file is opened sucessfully. If not, exit the program.
    However, free the dynamically allocated memory before exiting.
    */
    

    //======================Write header of the output file====================
    fprintf(Fout, "P5\n%d %d\n255\n", width, height);
    //===============Done writing header of the output file====================

    /*
    ToDo 11: Write output file with the content of dynamically allocated memory
    Hint: google "man fwrite(3p)"
    */
    

    /*
    ToDo 12: Do error checking to make sure that all bytes from allocated memory are written successfully to the output image file.
    If not, free the dynmaically allocated memory and exit the program.
    */
    
    /*
    ToDo 13: Close output file.
    */
   
    
    /*
    ToDo 14: Free the memory that was allocated to store black and white image.
    Hint: Google "man free". 
    */


    return 0;
}

/*
*******************************************************************************************************************************************
 *Purpose/Description:		The objective of this program is to modify a photo depending on the commands entered by the user. Options 
			include; Flip vertically, flip horizontally, scale, and rotate in both directions. The input can come from a file 
			or from the standard input, and the output can be written to a file or a standard output. Depending on the options, 
			the program takes the user commands and performs the corresponding operations. After identifying the commands entered 
			by the user, the program creates a temporary file (which can be removed with "make clean") in which it copies the 
			input regardless of whether it come from a file or from standard input. In this temporary (which can be removed with 
			"make clean") file all modifications are made, which are then written to a file or to a standard output depending on 
			the user's selection. Modifications are made by pointer operations which are manipulated to point to new memory addresses 
			to form a modified image. For a better understanding of the code, please browse through the code and read the comments on 
			the lines.	  

 * Author’s Name: Jose Aparicio
 * Author’s Panther ID: 6186111 
 * Certification: I hereby certify that this work is my own and none of it is the work of any other person.
*********************************************************************************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bmplib.h"

const int stringSize = 50;		//constant size for the strings

void errorMessage(int error);												//function to display error messages to the user
int flip (PIXEL *original, PIXEL **new, int rows, int cols);								//function to flip horizontally
int verticalFlip (PIXEL *original, PIXEL **new, int rows, int cols);                                                    //function to perform vertical flip
int rotate(PIXEL *original, PIXEL **new, int rows, int cols, int rotation, int *newrows, int *newcols);			//function to rotate the image
int enlarge (PIXEL *original, PIXEL **new, int rows, int cols, int scale, int *newrows, int *newcols);			//function to enlarge the image
//24
int  main(int argc, char ** argv)                                     							//main function
{
	extern char *optarg;                                          							//external variable (getopt)
	extern int optind;				              							//external variable (getopt)
	int c;                                                        							//variables to be used in the while loop
	char inputFileName[stringSize];			              							//char array used to store the input file name
	char outputFileName[stringSize];                              							//char array used to store the output file name
	char temporaryFile[]={"tmp.bmp"};										//char array used to store the name of a temporary file (can be deleted by "make clean")
	int flag[]={0,0,0,0,0};												//flags for the options entered by the user
	////////////s,r,f,v,o
	int counter =1;													//variable to count the number of operation to be performed
	int scale;													//variable to store the scale factor
	int rotation;													//variable to store the degrees of rotation
	int inputFromConsole=1;												//integer to be used as boolean to know if the input is from console or from a file
	memset(inputFileName,0,stringSize);										//setting the string to null
	memset(outputFileName,0,stringSize);	                                                      			//setting the string to null
	while ((c = getopt(argc, argv, "s: r: fls : v : ols")) != -1){                        				//while loop to iterate through all the arguments
		switch (c) {						
			case 's':											//case s
				flag[0]=1;										//setting flag to 1
				atoi(optarg)>0?scale=atoi(optarg):errorMessage(2);					//getting the scale factor
				counter=optind;										//setting the counter to the position after the current index
				break;											
			case 'r':                                                                                       //case r
				atoi(optarg)!=0?rotation=atoi(optarg):errorMessage(3);					//getting the degrees of rotation
				if(rotation % 90 != 0)									//validating the input
					errorMessage(1);								//displaying an error message if invalid input
				flag[1]=1;                                                                              //setting flag to 1
                                counter=optind;                                                                         //setting the counter to the position after the current index
                                break;
			case 'f':                                                                                       //case f
                                flag[2]=1;                                                                              //setting flag to 1
                                counter=optind;                                                                         //setting the counter to the position after the current index
                                break;
			case 'v':                                                                                       //case v
                                flag[3]=1;                                                                              //setting flag to 1
                                counter=optind;                                                                         //setting the counter to the position after the current index
                                break;
			case 'o':                                                                                       //case o
				flag[4]=1;                                                                              //setting flag to 1
				if(argv[optind]!= NULL)									//checking for a valid input
					strcpy(outputFileName,argv[optind]);            				//getting the name of the output file
				else
					errorMessage(4);								//displaying an error message if invalid input
                                counter=optind+1;                                                                       //setting the counter to the position after the current index
				break;						
			case '?':
				errorMessage(5);									//displaying an error message if invalid input
				break;
		}//75
	}
	
	if(argc>counter){												//determining if the input comes from a file or from console
		inputFromConsole=0;											//setting flag to 0
	}

	if(flag[0]+flag[1]+flag[2]+flag[3]==0)										//determining if any option was entered
		errorMessage(6);											//displaying an error message if no option was entered

	int rows, cols, newrows, newcols;										//variables to be use on the image manipulation
	PIXEL *b, *nb;													//pointers to PIXEL to modify the image
	if(inputFromConsole==1){											//check if the imput is from std input
		readFile(NULL,&rows,&cols,&b);										//reading the data from std input
		writeFile(temporaryFile,rows,cols,b);									//copying the data to a temporary file (it can be deleted by "make clean")
	}else {														//else the imput is from a file
		strcpy(inputFileName, argv[argc-1]);									//getting the name of the input file
		readFile(inputFileName,&rows,&cols,&b);									//reading the file
                writeFile(temporaryFile,rows,cols,b);									//copying the data to a temporary file (it can be deleted by "make clean")
	}

	if(flag[0]){													//checking if the enlarge flag is set
		readFile(temporaryFile,&rows,&cols,&b);									//reading the file
                enlarge(b,&nb, rows, cols, scale, &newrows, &newcols);							//calling the enlarge function
                writeFile(temporaryFile,newrows,newcols,nb);								//writing to the file
	}
	if(flag[1]){													//checking if the rotate flag is set
		if(rotation<0){												//checking if the angle is less than 0
			int i,times=(rotation/90)*-1;									//integer i to be use in the for loop, int times to know how many 90 degrees the image has to be rotated
			readFile(temporaryFile,&rows,&cols,&b);								//reading the file
			rotate(b,&nb, rows, cols, rotation, &newrows, &newcols);					//calling the function to rotate the image
			writeFile(temporaryFile,newrows,newcols,nb);							//writing to the file
			if(times>1){											//if times is grether than 1
				for(i=0;i<times-1;i++){									//for loop to repeat the process
					readFile(temporaryFile,&rows,&cols,&b);						//reading the file
					rotate(b,&nb, rows, cols, rotation, &newrows, &newcols);			//calling the function to rotate the image
					writeFile(temporaryFile,newrows,newcols,nb);					//writing to the file
				}
			}
		}else{													// else if the angle is bigger or equal to 0
			int i;												//integer i to be use in the for loop
			int times=(rotation/90);									//int times to know how many 90 degrees the image has to be rotated
			times=times<=4?4-times:4-times%4;								//calculating times
       	                readFile(temporaryFile,&rows,&cols,&b);								//reading the file
               	        rotate(b,&nb, rows, cols, rotation, &newrows, &newcols);					//calling the function to rotate the image
                       	writeFile(temporaryFile,newrows,newcols,nb);							//writing to the file
         		if(times>1){											//if times is grether than 1
                               	for(i=0;i<times-1;i++){									//for loop to repeat the process
                                       	readFile(temporaryFile,&rows,&cols,&b);						//reading the file
                                       	rotate(b,&nb, rows, cols, rotation, &newrows, &newcols);			//calling the function to rotate the image
                               		writeFile(temporaryFile,newrows,newcols,nb);					//writing to the file
                               	}
                       	}
		}
	}
	if(flag[3]){													//checking if the vertical flag is set
		readFile(temporaryFile,&rows,&cols,&b);									//reading the file
		verticalFlip(b, &nb, rows, cols);									//calling the function to vertically flip the image
		writeFile(temporaryFile,rows,cols,nb);									//writing to the file
	}
	if(flag[2]){													//checking if the horizontal flag is set
		readFile(temporaryFile,&rows,&cols,&b);									//reading the file
       	        flip(b, &nb, rows, cols);										//calling the function to horizontally flip the image
       	        writeFile(temporaryFile,rows,cols,nb);									//writing to the file
	}
	
	if(flag[4]==1){													//checking if the output-to-file flag is set
		readFile(temporaryFile,&rows,&cols,&b);									//reading from temporary file
            	writeFile(outputFileName,rows,cols,b);									//writing to output file
	}else{														//else the output is to std output
		readFile(temporaryFile,&rows,&cols,&b);									//reading from temporary file
                writeFile(NULL,rows,cols,b);										//writing to std output
	}
	free(b);													//reclaiming memory
	free(nb);													//reclaiming memory

	return 0;													//returning 0
}


int flip (PIXEL *original, PIXEL **new, int rows, int cols){								//function to flip the image horizontally
	int row, col;													//integer variables to be used in the for loops

	if ((rows <= 0) || (cols <= 0)) 										//checking if the image has a valid size
		return -1;												//returning -1

	*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));									//allocating the space in memory for the new image

	for (row=0; row < rows; row++)											//for loop to iterate through the array 
		for (col=0; col < cols; col++) {									//for loop to iterate through the array
			PIXEL* o = original + row*cols + col;								//setting *o to poit to the original pixel
			PIXEL* n = (*new) + row*cols + (cols-1-col);							//setting *n to point to the position of the new pixel
			*n = *o;											//setting *n equal to *o
		}
	return 0;													//returning 0
}

int verticalFlip (PIXEL *original, PIXEL **new, int rows, int cols){							//function to flip the image vertically
        int row, col;													//integer variables to be used in the for loops
	
        if ((rows <= 0) || (cols <= 0))											//checking if the image has a valid size
                return -1;												//returning -1

        *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));									//allocating the space in memory for the new image

        for (row=0; row < rows; row++)	{										//for loop to iterate through the array
                for (col=0; col < cols; col++) {									//for loop to iterate through the array
			PIXEL* o = original + row*cols + col;								//setting *o to poit to the original pixel
			PIXEL* n = (*new) + (((rows-1)-row)*cols)+((cols-1)-((cols-1)-col)); 				//setting *n to point to the position of the new pixel
			*n = *o;											//setting *n equal to *o
		}
	}
	return 0;													//returning 0
}


int rotate (PIXEL *original, PIXEL **new, int rows, int cols, int rotation, int *newrows, int *newcols){		//function to rotate the image 
        int row, col;													//integer variables to be used in the for loops
	*newrows=cols;													//setting the new size of newcols
	*newcols=rows;													//setting the new size of newrows
        if ((rows <= 0) || (cols <= 0))											//checking if the image has a valid size
                return -1;												//returning -1

        *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));									//allocating the space in memory for the new image
		
        for (row=0; row < rows; row++)  {										//for loop to iterate through the array
                for (col=0; col < cols; col++) {									//for loop to iterate through the array
			PIXEL* o = original + row*cols + col;								//setting *o to poit to the original pixel
			PIXEL* n = (*new) + col*cols + (rows-1-row);							//setting *n to point to the position of the new pixel
      			*n = *o;											//setting *n equal to *o
        	}
	}
        return 0;													//returning 0
}


int enlarge (PIXEL *original, PIXEL **new, int rows, int cols, int scale, int *newrows, int *newcols){			//function to enlarge the image
        int row, col;													//integer variables to be used in the for loops
        *newrows=scale*rows;												//setting the new size of newrows
        *newcols=scale*cols;												//setting the new size of newcols
        if ((rows <= 0) || (cols <= 0))											//checking if the image has a valid size
                return -1;												//returning -1
		
        *new = (PIXEL*)malloc((scale*rows)*(scale*cols)*sizeof(PIXEL));							//allocating the space in memory for the new image

        for (row=0; row < rows; row++)  {										//for loop to iterate through the array
                for (col=0; col < cols; col++) {									//for loop to iterate through the array
                        PIXEL* o = original + row*cols + col;								//setting *o to poit to the original pixel
			int i,x;											//setting *n to point to the position of the new pixel
			for(x=0;x<scale;x++){										//for loop to help to calculate  the new position
				for(i=0;i<scale;i++){									//for loop to help to calculate  the new position
                        		PIXEL* n = (*new) + scale*row*(*newcols)+(*newcols*x) + (scale * col + i);	//setting *n to point to the position of the new pixel
                        		*n = *o;									//setting *n equal to *o
				}
			}
                }
        }
        return 0;													//returning 0
}



void errorMessage(int error){																			//function to display error messages
	switch(error){
		case 1:
			printf("\n****ERROR**** The argument for \"rotate [-r]\" has to be a multiple of 90 degrees!\n\n");							//eror message
			break;
		case 2:
                        printf("\n****ERROR**** The argument for \"scale [-s]\" has to be an integer greather than 0!\n\n");							//eror message
                        break;
		case 3:
			printf("\n****ERROR**** The argument for \"rotate [-r]\" must be an integer different from 0!\n\n");							//eror message
                        break;
		case 4:
                        printf("\n****ERROR**** The argument for \"output [-o]\" must be a valid file name!\n\n");								//eror message
                        break;
		case 5:
                        printf("\n****ERROR**** An invalid option was entered. Valid option are:  [-s scale | -r degree | -f | -v ] [-o output_file] [input_file] \n\n");	//eror message
                        break;
		case 6:
                        printf("\n****ERROR**** No option was entered. Valid option are:  [-s scale | -r degree | -f | -v ]\n\n");						//eror message
                        break;

		default:
			printf("\n*****UNHANDLED ERROR*****\n\n");														//eror message
			break;
	}


	exit(0);																				//terminating the program after any error
}

//Natalie Carlson
//Linda Lee
////OS A2-findlocationfast.c
//Due Oct 6
/*
	Test Cases:
	201200Jersey City NJ	201201 Invalid
	201202Hackensack NJ     662906 Invalid                
	662907Rolling For MS    664725 Invalid              
	732355Monmouth Ju NJ 	989996Saginaw MI 	
*/


#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define bool int
#define false 0
#define true 1

//function prototypes
void checkArgs(int argc, char *pf);
int lengthCheck(char *s);
int binaryLineSearch(size_t fd, size_t cursor, int p, int q, char* prefix);


//main
int main(int argc, char *argv[]) {
	
	//Search info
	char* fileName = argv[1];
	char* prefix = argv[2]; 
	checkArgs(argc, prefix);
	
	//File info
	size_t fd, cursor, fileLength;
	fd = open(fileName, O_RDONLY, 0); //open file with read permissions
	if(fd == -1){
		perror("File could not be opened");
		close(fd); //close file
		exit(1);
	}
	cursor = lseek(fd, 0, 2); //move cursor to end of file
	if(cursor == -1){
		perror("File not seekable");
		close(fd); //close file
		exit(1);
	}
	else{
		fileLength = cursor+1; //get final bit number
		if (fileLength > 2147483648){
			fileLength = 2147483648;
		}
	}
	//Search
	int foundAt = binaryLineSearch(fd, cursor, 0, fileLength, prefix);	
	if (foundAt >= 0){ //prefix found
		void* line[2];
		cursor = lseek(fd, foundAt+30, 0); //move cursor to end of line
		while((read(fd, line, 1)) && (*line == ' ')){ //ignore trailing white space
			cursor = lseek(fd, -2, 1);	
		}
		int stop = cursor+1;
		int start = foundAt + 6; //ignore prefix
		cursor = lseek(fd, start, 0); //move cursor to start of line
		for(int i = start; i < stop; i++){
			read(fd, line, 1);
			write(1,line, 1); //output location char by char
		}
		write(1, "\n", 1);
		close(fd); //close file
		exit(0);
		
	}
	else{ //Prefix not in file
		close(fd); //close file
		exit(1);
	}	
}

//This method verifies the argument conditions are met 	
void checkArgs(int argc, char *pf){
	
	if (argc != 3){
		perror("Incorrect number of arguments");
		exit(1);
	}
	if (lengthCheck(pf) != 6){
		perror("Incorrect prefix length");
		exit(1);
	}
	for (int i = 0; i < 6 ; i++){
		if (!isdigit(pf[i])){
			perror("Incorrect prefix type");
			exit(1);
		}
	}
}

//This method returns the length of a c string
int lengthCheck(char *s){

	int i = 0;
	char c = s[i];
	while (c != '\0'){
		i++;
		c = s[i];
	}
	return i;
}

//This method recursively searches a file by line, halving the search space each cycle
int binaryLineSearch(size_t fd, size_t cursor, int start, int end, char* prefix){
		
	void* middleBuffer[6];
	int numLines = (end - start)/32;
	int middleLine = start + (numLines/2)*32;
	
	if (start > (end-32)){ //no match found
		return -1;
	}
	else{ //compare to middleLine
		cursor = lseek(fd, middleLine, 0); //reset cursor
		int i = 0;
		bool match = true;
		while(match == true && i <= 5){ //check first six char of line
			read(fd, middleBuffer, 1);
			if (*middleBuffer == prefix[i]){ //compare with prefix
				i++;
			}
			else{
				match = false;
			}	
		}
		if(match == true){ //prefix found!!!
			return middleLine;
		}			
		else if(*middleBuffer > prefix[i]){ //prefix is smaller
			printf("");//I don't know why but the program fails without this.
			return binaryLineSearch(fd, cursor, start, middleLine, prefix);		
		}
		else{ //prefix is bigger
			printf("");//I don't know why but the program fails without this.
			return binaryLineSearch(fd, cursor, middleLine+32, end, prefix);	
		}			
	}	
}
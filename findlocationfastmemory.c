//Natalie Carlson
//Linda Lee
//OS A2- Mapping files to memory
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
#include <sys/mman.h>

#define bool int
#define false 0
#define true 1

//function prototypes
void checkArgs(int argc, char *pf);
int lengthCheck(char *s);
int binaryLineSearch(char* memFile, int p, int q, char* prefix);


int main(int argc, char *argv[]) {
		
		//Search info
		char* prefix = argv[2]; 
		char* fileName = argv[1];
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
		
		//map file to memory
		char *memFile = mmap(NULL, fileLength, PROT_READ, MAP_PRIVATE, fd, 0);
		
		//search
		int foundAt = binaryLineSearch(memFile, 0, fileLength, prefix);		
		if (foundAt >= 0){
			munmap(memFile, fileLength);
			close(fd); //close file
			exit(0);
		}
		else{ //Prefix not in file
			munmap(memFile, fileLength);
			close(fd); //close file
			exit(1);
		}	
		
}

//This method verifies the argument conditions are met 	
void checkArgs(int argc, char *pf){
		
	if (argc !=3){
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
int binaryLineSearch(char* memFile, int start, int end, char* prefix){
		
	int numLines = (end - start)/32;
	int middleLine = start + (numLines/2)*32;
	
	if (start > (end-32)){ //no match found
		return -1;
	}
	else{ //compare to middleLine
		int cursor = middleLine;//reset cursor
		int i = 0;
		bool match = true;
		while(match == true && i <= 5){ //check first six
			if (memFile[cursor] == prefix[i]){//compare with prefix
				i++;
				cursor++;
			}
			else{
				match = false;
			}	
		}
		if(match == true){ //prefix found!!!
			return middleLine;
		}			
		else if(memFile[cursor] > prefix[i]){//prefix is smaller
			printf("");//I don't know why but the program fails without this.
			return binaryLineSearch(memFile, start, middleLine, prefix);		
		}
		else{//prefix is bigger
			printf("");//I don't know why but the program fails without this.
			return binaryLineSearch(memFile, middleLine+32, end, prefix);		
		}			
	}	
}

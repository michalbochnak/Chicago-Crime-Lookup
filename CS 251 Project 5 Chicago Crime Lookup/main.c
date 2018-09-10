//
// main.c
//
// Chicago Crime Lookup via hashing. 
//
// << Michal Bochnak >>
// << Windows 10, Microsoft Visual Studio 2017 >>
// U. of Illinois, Chicago
// CS251, Spring 2017
// Project #05
//


#include "header.h"


int main() {
	printf("** Chicago Crime Lookup **\n");

	// stats about the records, initialize to 0
	Stats stats;
	stats.numCrimes = 0;
	stats.numCollisions = 0;

	// get file names
	char *crimesFileName = getFileName();
	char *areasFileName = getFileName();

	// open files
	FILE *crimesFile = fopen(crimesFileName, "r");
	FILE *areasFile = fopen(areasFileName, "r");
		
	// estimate the size of HashTable, verify allocation, malloc the memory, and initialize
	long sizeHT = estimateSize(crimesFileName);
	Crime **crimesHashTable = (Crime**)malloc(sizeof(Crime*) * sizeHT);
	if (crimesHashTable == NULL) {
		printf("** Error: malloc failed to allocate hash table (%ld elements).\n", sizeHT);
		exit(-1);
	}
	initializeCrimesHT(crimesHashTable, sizeHT);

	char areasHashTable[78][25];	// space for areasHashTable

	// built the areas hash table
	buildAreasHashTable(areasFile, areasHashTable);

	// build the crimes hash table
	buildCrimesHashTable(crimesHashTable, crimesFile, sizeHT, areasHashTable, &stats);
	
	displayStats(crimesFileName, sizeHT, stats);	// display stats
	
	fclose(crimesFile);		// close the crimes file
	fclose(areasFile);		// close the areas file

	// search for requested case number and display info if found
	searchCrimeAndDisplayResult(crimesHashTable, sizeHT);
	
	printf("** Done **\n");
	return 0;
} // end of main


//-----------------------------------------------------------------------------
// Functions definitions
//-----------------------------------------------------------------------------

// getFileName: inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be opened, an
// error message is output and the program is exited.
char *getFileName() {
	
	char filename[512];
	int  fnsize = sizeof(filename) / sizeof(filename[0]);

	// input filename from the keyboard:
	fgets(filename, fnsize, stdin);
	filename[strcspn(filename, "\r\n")] = '\0';  // strip EOL char(s):

	// make sure filename exists and can be opened:
	FILE *infile = fopen(filename, "r");
	if (infile == NULL)
	{
		printf("**Error: unable to open '%s'\n\n", filename);
		exit(-1);
	}

	fclose(infile);

	// duplicate and return filename:
	char *s = (char *)malloc((strlen(filename) + 1) * sizeof(char));
	strcpy(s, filename);

	return s;
}


// returns the file size in bytes
long getFileSizeInBytes(char *filename) { 
	
	FILE *file = fopen(filename, "r"); 
	if (file == NULL)
		return -1; 
	
	fseek(file, 0, SEEK_END); 
	long size = ftell(file); 
	
	fclose(file); 
	
	return size; 
}


// estimates and returns the size of hash table
long long estimateSize(char *crimesFileName) {

	long long fileSize = (getFileSizeInBytes(crimesFileName));

	if (fileSize < 80000000)
		return ( fileSize / 50 ) * 15;
	else
		return  ( fileSize / 50 ) * 12;
}


// displays statistics
void displayStats(char* crimesFileName, long sizeHT, Stats stats) {

	printf(">> Crimes file: %ld\n", getFileSizeInBytes(crimesFileName));
	printf(">> # crimes:    %ld\n", stats.numCrimes);
	printf(">> HT Size:     %ld\n", sizeHT);
	printf(">> Collisions:  %ld\n\n", stats.numCollisions);
}


// verifies input correctnes
int inputCorrect(char* caseNum) {

	// if number of characters is different than 8,
	if (strlen(caseNum) != 8) {
		printf("** invalid case #, try again...\n\n");
		return 0;		// invalid
	}
	// if letter part of the case number is not valid letter
	else if (caseNum[3] > 90 || caseNum[3] < 65) {
		printf("* invalid case #, try again...\n\n");
		return 0;		// invalid
	}

	return 1;	// valid
}


// displays info about crime
void displayCrimeInfo(Crime* crime) {
	
	printf("%s:\n", crime->caseNum);
	printf("  date/time: %s\n", crime->dateAndTime);
	printf("  city area: %d => %s\n", crime->areaCode, crime->area);
	printf("  IUCR:      %s\n", crime->IUCR);
	printf("  arrested:  %s\n\n", (crime->arrested == 'T') ? "true" : "false");
}
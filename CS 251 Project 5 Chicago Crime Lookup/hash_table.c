//
// hash_table.c
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


// build the areasHashTable 
void buildAreasHashTable(FILE *areasFile, char areasHashTable[][25]) {

	char line[81];			// line
	char *token;			// token from line
	int index;				// index in hash table

	// skip header line in input file
	fgets(line, 81, areasFile);
	line[strcspn(line, "\r\n")] = '\0';

	// build the table
	while (fgets(line, 81, areasFile) != NULL) {
		line[strcspn(line, "\r\n")] = '\0';
		// get index
		token = strtok(line, ",");
		index = atoi(token);
		// get the name of area
		token = strtok(NULL, ",");
		// insert into table
		strcpy(areasHashTable[index], token);
	}
}


// initialize the hash table to '\0'
void initializeCrimesHT(Crime** crimesHashTable, long sizeHT) {

	int i = 0;

	// initialize every element to '\0'
	for (; i < sizeHT; i++)
		crimesHashTable[i] = '\0';

}


// builds the crimesHashTable
void buildCrimesHashTable(Crime **crimesHashTable, FILE *crimesFile,
	long size, char areasHashTable[][25], Stats *stats) {

	char line[81];		// line
	char *token;		// token from line
	Crime temp;			// temp to store info from file

	fgets(line, 81, crimesFile);			// skip header line in input file
	line[strcspn(line, "\r\n")] = '\0';		// avoid problem on different platforms

	// build the table
	while ((fgets(line, 81, crimesFile)) != NULL) {
		stats->numCrimes++;		// increment number of crimes
		// get case number and insert into temp
		token = strtok(line, ",");
		strcpy(temp.caseNum, token);
		// get the IUCR and insert into temp
		token = strtok(NULL, ",");
		strcpy(temp.IUCR, token);
		// get Date and Time and insert into temp
		token = strtok(NULL, ",");
		strcpy(temp.dateAndTime, token);
		// skip beat, district, and ward
		token = strtok(NULL, ",");
		token = strtok(NULL, ",");
		token = strtok(NULL, ",");
		// get status for "arrested"
		token = strtok(NULL, ",");
		temp.arrested = token[0];
		// insert the area name and area code
		strcpy(temp.area, areasHashTable[getAreaCode(temp.caseNum, areasHashTable)]);
		temp.areaCode = getAreaCode(temp.caseNum, areasHashTable);
		
		// insert into crimesHashTable
		insertIntoHT((findFreeSpot(temp.caseNum, size, crimesHashTable, *&stats)), temp, crimesHashTable);
	}
}


// extracts the area part from the case number and returns the code
// of the area which corresponds to the extracted value
int getAreaCode(char *caseNum, char areasHashTable[][25]) {

	char areaCodeString[3];		// string code
	int areaCodeInt;			// integer code

	// copy the 2 integers corresponding the the area part
	// and insert NULL at the end
	areaCodeString[0] = caseNum[1];
	areaCodeString[1] = caseNum[2];
	areaCodeString[2] = '\0';

	// convert to int 
	areaCodeInt = atoi(areaCodeString);

	// return the code
	return areaCodeInt;
}


// calculate the hash index
long long hash(char *caseNum, int size) {

	long long hashIndex = 0;
	char *temp = caseNum;
	int first;
	
	// discard "R" at the beggining
	temp++;

	// combines the ASCII values corresponding to the input string
	// to creates the hash ID
	hashIndex = 10 * temp[0];
	hashIndex = hashIndex + temp[1];
	hashIndex = hashIndex * 100;
	hashIndex = hashIndex + temp[2];
	hashIndex = hashIndex * 10;
	hashIndex = hashIndex + temp[3];
	hashIndex = hashIndex * 10;
	hashIndex = hashIndex + temp[4];
	hashIndex = hashIndex * 10;
	hashIndex = hashIndex + temp[5];
	hashIndex = hashIndex * 10;
	hashIndex = hashIndex + temp[6];
	
	// return hashID moduled by hash table size
	return (hashIndex) % size;
}


// finds and returns free spot in hash table
long findFreeSpot(char *caseNum, long size, Crime** crimesHashTable, Stats *stats) {

	// get the index
	long freeSpot = hash(caseNum, size);

	// if first spot is null, spot found, return freeSpot
	if (crimesHashTable[freeSpot] == '\0')
		return freeSpot;
	// check for free spot by linear probing
	else {
		while (crimesHashTable[freeSpot] != '\0') {
			stats->numCollisions++;		// count the collision
			++freeSpot;					// increment freeSpot
			freeSpot = freeSpot % size;	// modulo to wrap around to index 0 if needed
		}
	}

	return freeSpot;		// return free spot
}


// insert at given spot
void insertIntoHT(long spotNum, Crime temp, Crime** crimesHashTable) {

	// malloc the memory
	crimesHashTable[spotNum] = (Crime*)malloc(sizeof(Crime));

	// verify if malloc was sussessful
	if (crimesHashTable[spotNum] == NULL) {
		printf("** Error: malloc failed to allocate space for Crime struct.\n");
			exit(-1);
	}

	// copy the data over to hash table
	strcpy(crimesHashTable[spotNum]->caseNum, temp.caseNum);
	strcpy(crimesHashTable[spotNum]->IUCR, temp.IUCR);
	strcpy(crimesHashTable[spotNum]->dateAndTime, temp.dateAndTime);
	crimesHashTable[spotNum]->arrested = temp.arrested;
	strcpy(crimesHashTable[spotNum]->area, temp.area);
	crimesHashTable[spotNum]->areaCode = temp.areaCode;
}


// prompts for the input and look up the case number
// displays the results if found
void searchCrimeAndDisplayResult(Crime **crimesHashTable, long sizeHT) {

	char caseNum[10];		// case number
	long hashID;			// hash ID
	long hashIndex;			// hash index

	// prompt for input
	printf("Enter a case number> ");
	fgets(caseNum, 81, stdin);
	caseNum[strcspn(caseNum, "\r\n")] = '\0';	// handle platform differences

	// process case numbers 
	while (caseNum[0] != '\0') {

		// verify input correctness, continue to while if incorrect
		if (!(inputCorrect(caseNum))) {
			fgets(caseNum, 81, stdin);
			caseNum[strcspn(caseNum, "\r\n")] = '\0';	// handle platform differences
			continue;
		}
		else {
			// find hash ID
			hashID = hash(caseNum, sizeHT);
			// retrieve case information case number
			hashIndex = retrieveCaseInfo(hashID, crimesHashTable, caseNum);

			printf(">> hash index: %ld <<\n", hashID);	// print hash ID
			if (hashIndex == -1)						// not found
				printf("** Case not found...\n\n");
			else										// found, display
				displayCrimeInfo(crimesHashTable[hashIndex]);
		}
		printf("Enter a case number> ");	// read in next case number from user
		fgets(caseNum, 81, stdin);
		caseNum[strcspn(caseNum, "\r\n")] = '\0';	// handle platform differences
	}
}


// receives hashID, based on that searches for the case number,
// returns new index ( might be the same - linear probing was used 
// to handle collisions ) or -1 if not found
long retrieveCaseInfo(long hashID, Crime** crimesHashTable, char* caseNum) {


	if (crimesHashTable[hashID] == NULL)
		return -1;				// not found
	else if (strcmp(crimesHashTable[hashID]->caseNum, caseNum) == 0)	// found
		return hashID;			// found
	else {
		hashID++;				// forward hashID
		// lookup the fallowing indeces
		while (crimesHashTable[hashID]->caseNum[0] != '\0') {
			if (strcmp(crimesHashTable[hashID]->caseNum, caseNum) == 0) {
				return hashID;	// found 
			}
			hashID++;			// forward hashID
		}
	}

	return -1;					// not found
}
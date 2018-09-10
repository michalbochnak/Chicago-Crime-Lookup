//
// header.h
//
// Chicago Crime Lookup via hashing. 
//
// << Michal Bochnak >>
// << Windows 10, Microsoft Visual Studio 2017 >>
// U. of Illinois, Chicago
// CS251, Spring 2017
// Project #05
//

#pragma once

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// typedef for Hash Table Key
typedef char HTKey;

// struct with inforamtions about crime
typedef struct Crime {
	HTKey	caseNum[10];
	char	IUCR[5];
	char	dateAndTime[25];
	int		areaCode;
	char	arrested;
	char	area[25];
} Crime;

// struct with stats about collisions and crimes,
// to make passing parameters easier
typedef struct Stats {
	long	numCrimes;
	long	numCollisions;
} Stats;


//-----------------------------------------------------------------------------
// function declarations
//-----------------------------------------------------------------------------

// main.c
char *getFileName();
long getFileSizeInBytes(char *filename);
void displayStats(char* crimesFileName, long sizeHT, Stats stats);
void displayCrimeInfo(Crime* crime);
long long estimateSize(char *crimesFileName);
int inputCorrect(char* caseNum);

// hash_table.c
void buildAreasHashTable(FILE *areasFileName, char areasHashTable[][25]);
void buildCrimesHashTable(Crime **crimesHashTable, FILE *crimesFile,
					long size, char areasHashTable[][25], Stats *stats);
int getAreaCode(char *caseNum, char areasHashTable[][25]);
long long hash(char *caseNum, int size);
long findFreeSpot(char *caseNum, long size, Crime** crimesHashTable, Stats *stats);
void initializeCrimesHT(Crime** crimesHashTable, long size);
void insertIntoHT(long spotNum, Crime temp, Crime** crimesHashTable);
void searchCrimeAndDisplayResult(Crime **crimesHashTable, long sizeHT);
long retrieveCaseInfo(long hashID, Crime** crimesHashTable, char* caseNum);


/*
Sample input:

Crimes.csv
Areas.csv
R08U3503
R51Q0712
abcd
1234
R03T1234
R78A9218

Crimes-2.csv
Areas.csv
R08U3503
R51Q0712
abcd
1234
R71M8636
R02D7706
R49H4781
*/



#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include <string.h>
#include "readfile.h"



void zipCodeInsertion(zipTowns arrs, int length) {          // add data into array
	int j = length - 1;
  int pZip = arrs.cities[length].zip;
	while (j >= 0 && arrs.cities[arrs.zips[j]].zip > pZip) {
		arrs.zips[j + 1] = arrs.zips[j];
		j--;
	}
	arrs.zips[j + 1] = length;
}


void townInsertion(city * pCity, city * towns[], int length) { // add data into array
	int j = length - 1;
	while (j >= 0 && strcmp(towns[j]->town, pCity->town) > 0) {
		towns[j + 1] = towns[j];
		j--;
	}
	towns[j + 1] = pCity;
}
/*
Used the following to help produce the insertion code: Source: Professor Hardin

void insert(char x, char A[], int length) {
	int j = length - 1;
	while (j >= 0 && A[j] > x) {
		A[j + 1] = A[j];
		j--;
	}
	A[j + 1] = x;
}
*/
void readFile(zipTowns arrs, FILE * infile, int * length){
  char zipCode[6]; //Zip codes are 6 char long
  char cityName[20]; //Allow up to 20 char for city name
  int i = 0; //this will keep track of the length of the file

  while(fscanf(infile, "%s  %s", zipCode, cityName) > 1){ //cont. to loop while we find values using fscanf()

    city * nTown = &arrs.cities[i];
    int zip = atoi(zipCode);
    nTown->zip = 0;                                  // zip code initialized
    nTown->town = malloc((strlen(cityName)+1)); // town initialized and allocate space

    //int sizeofp = strlen(cityName);           // Used this to help see what was going on with sizeOf() and strLen()
    //printf("size of town = %d\n", sizeofp);

    nTown->zip = zip;
    strcpy(nTown->town, cityName);                  //takes the current cityName and performs stringCopy
    arrs.cities[i] = *nTown;                         // Unsorted array;
    townInsertion(nTown, arrs.towns, i);            // insert town into array
    zipCodeInsertion(arrs, i);                              // insert zip code into array
    i++; // increase index

  }

  *length=i;

}

/*
Used the following to help produce the readFile code:
Source: https://stackoverflow.com/questions/54302412/how-to-correctly-use-fscanf-in-a-while-loop

*/

/*
Previous implementation of readFile():


void readFile(zipTowns arrs, FILE* infile, int* length)
{
    city* mainArray = (city*) malloc(sizeof(city*) * 100);
    int zip;
    char *town = malloc(100*(sizeof(char*)));

    int i = 0;
    while(fscanf(infile, "%d %s", &zip, town) != EOF)
    {
        mainArray[i].town = malloc(sizeof(char*) * 100);    // allocate space for town name

        strcpy(mainArray[i].town, town);
        mainArray[i].zip = zip;

        // copy data read from file into arrs (cities)
        arrs.cities[i].town = malloc(sizeof(char*) * 100);    // allocate space for town name
        strcpy(arrs.cities[i].town, mainArray[i].town);
        arrs.cities[i].zip = mainArray[i].zip;

        // add data to arrs using insertion sort for town names
        int j = i - 1;
        city* c = (city*)malloc(sizeof(city));
        c = &mainArray[i];

        while (j >= 0 && strcmp(arrs.towns[j]->town, c->town) > 0)
        {
            arrs.towns[j+1] = (city*)malloc(sizeof(city*) * 100);
            arrs.towns[j + 1] = arrs.towns[j];
            j--;
        }
        arrs.towns[j + 1] = c;

        // add data (index of mainArray) into arrs using insertion sort for zip codes
        j = i - 1;
        c = &mainArray[i];

        while (j >= 0 && mainArray[arrs.zips[j]].zip > c->zip)
        {
            arrs.zips[j + 1] = arrs.zips[j];
            j--;
        }
        arrs.zips[j + 1] = i;

        // increase index
        i++;
    }

    *length = i;
}



*/

#ifndef INTERACTIVE_H
#define INTERACTIVE_H
#include "structs.h"

extern void doInteractive(zipTowns arrs, int length);

extern int searchZip(zipTowns arrs, int size, unsigned int zip);

extern int searchTown(zipTowns arrs, int size, char* town);

#endif // last line of file

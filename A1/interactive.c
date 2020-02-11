#include "interactive.h"

void doInteractive(zipTowns arrs, int length)
{
    int choice;
    do
    {
        printf("1. Quit\n");
        printf("2. Search for zip\n");
        printf("3. Search for town\n");
        printf("Choice > ");
        scanf("%d", &choice);

        if(choice == 1)
        {
            printf("Bye!!\n");
        }
        else if(choice == 2)
        {
            unsigned int zip;
            printf("Enter zip you want to search? ");
            scanf("%u", &zip);

            int index = searchZip(arrs, length, zip);

            if(index == -1)
            {
                printf("Requested zip not found!\n");
            }
            else
            {
                printf("zip = %u\ntown = %s\n", arrs.cities[arrs.zips[index]].zip, arrs.cities[arrs.zips[index]].town);
            }
        }
        else if(choice == 3)
        {
            char* town = malloc(sizeof(char) * 100);
            printf("Enter town you want to search? ");
            scanf("%s", town);

            int index = searchTown(arrs, length, town);

            if(index == -1)
            {
                printf("Requested town not found!\n");
            }
            else
            {
                printf("zip = %u\ntown = %s\n", arrs.towns[index]->zip, arrs.towns[index]->town);
            }
        }
        else
        {
            printf("Invalid Choice! Try again!\n");
        }

        printf("\n");
    }while(choice != 1);
}

int searchZip(zipTowns arrs, int size, unsigned int zip)
{
    int index = -1;

    int start = 0, end = size-1;
    while (start <= end)
    {
        int mid = start + (end - start) / 2;

        // check existance of zip in arrs
        if (arrs.cities[arrs.zips[mid]].zip == zip)
        {
            index = mid;
        }

        // If zip at mid is greater ignore left half
        if (arrs.cities[arrs.zips[mid]].zip < zip)
            start = mid + 1;

        // If zip at mid is smaller, ignore right half
        else
            end = mid - 1;
    }

    // return index, if found then index of zip
    // else -1
    return index;
}

int searchTown(zipTowns arrs, int size, char* town)
{
    int index = -1;

    int start = 0, end = size-1;
    while (start <= end)
    {
        int mid = start + (end - start) / 2;

        // check existance of town in arrs
        if (strcmp(arrs.towns[mid]->town, town) == 0)
        {
            index = mid;
        }

        // If town at mid is alphabetically greater ignore left half
        if (strcmp(arrs.towns[mid]->town, town) < 0)
            start = mid + 1;

        // If town at mid is alphabetically smaller, ignore right half
        else
            end = mid - 1;
    }

    // return index, if found then index of town
    // else -1
    return index;
}

#include <stdio.h>
#include <stdlib.h>

void processArr();
extern void computeArr(int *forceArr, int *distanceArr, int * calcArr, int arrSize);
void createFile(int *calcArr, int arrSize);

int main()
{
    int arrSize = 0;

    //FLUSH INPUT AND READ FILE NAME
    fflush(stdin);
    printf("Enter the size of the force array: ");
    scanf("%d", &arrSize);

    //BEGIN ARRAY CREATION
    processArr(arrSize);

    return 0;
}

void processArr(int arrSize)
{
    int *forceArr = NULL;
    int *distanceArr = NULL;
    int *calcArr = NULL;
    int i = 0;
    int j = 0;

    forceArr = malloc(sizeof(int) * arrSize);
    distanceArr = malloc(sizeof(int) * arrSize);
    calcArr = malloc(sizeof(int) * arrSize);

    for (i = 0; i < arrSize; i ++)
    {
        distanceArr[i] = i % 10 + 1;

        if (i < (arrSize / 2))
        {
            forceArr[i] = i + 1;
            j = i + 1;
        } else if (i == (arrSize / 2) && 0 < arrSize % 2)
        {
            forceArr[i] = i + 1;
        } else
        {
            forceArr[i] = j--;
        }
    }

    //SEND FUNCTION TO GPU
    computeArr(forceArr, distanceArr, calcArr, arrSize);

    free(forceArr);
    free(distanceArr);

    printf("Printing the inner resulting array: \n");

    //OUTPUT THE MATCH SCORE ARRAY
    createFile(calcArr, arrSize);

    free(calcArr);
}

void createFile(int *calcArr, int arrSize)
{
    //DECLARE VARS
    FILE *filep;
    int i = 0;

    //OPEN FILE
    filep = fopen("force.txt", "w+");

    //WRITE RESULTS TO FILE
    for (i = 0; i < arrSize; i ++)
    {
        fprintf(filep, "%d,", calcArr[i]);
    }

    //CLOSE FILE
    fclose(filep);

    //LET USER KNOW PROGRAM IS DONE
    printf("file created \n");
}


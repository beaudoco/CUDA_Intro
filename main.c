#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void processArr();
extern void computeArr(int * calcArr, int arrSize);
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
    int *calcCPUArr = NULL;
    int i = 0, j = 0;
    unsigned long long int scalar = 0;
    unsigned long long int scalarCPU = 0;

    forceArr = malloc(sizeof(int) * arrSize);
    distanceArr = malloc(sizeof(int) * arrSize);
    calcArr = malloc(sizeof(int) * arrSize);
    calcCPUArr = malloc(sizeof(int) * arrSize);

    //SETUP TIMER FOR FILE
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

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

        calcCPUArr[i] = distanceArr[i] * forceArr[i];

        scalarCPU += calcCPUArr[i];
    }

    //END CLOCK AND GET TIME
    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    free(forceArr);
    free(distanceArr);

    //SETUP TIMER FOR FILE
    struct timespec begin2, end2;
    clock_gettime(CLOCK_REALTIME, &begin2);

    //SEND FUNCTION TO GPU
    computeArr(calcArr, arrSize);

    for (i = 0; i < arrSize; i ++)
    {
        scalar += calcArr[i];
    }

    //END CLOCK AND GET TIME
    clock_gettime(CLOCK_REALTIME, &end2);
    long seconds2 = end2.tv_sec - begin2.tv_sec;
    long nanoseconds2 = end2.tv_nsec - begin2.tv_nsec;
    double elapsed2 = seconds2 + nanoseconds2*1e-9;

    printf("Printing the inner resulting array: \n");

    //OUTPUT THE MATCH SCORE ARRAY
    createFile(calcArr, arrSize);

    free(calcArr);
    free(calcCPUArr);

    printf("The scalar value of the system is: %lld \n", scalar);
    printf("time taken for GPU: %f\n",elapsed2);

    printf("The scalar value of the system is: %lld \n", scalarCPU);
    printf("time taken for CPU: %f\n",elapsed);
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


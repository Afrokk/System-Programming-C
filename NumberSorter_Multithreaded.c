#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

//Function Prototypes
void* sorter(void* params);
void* merger(void* params);

//Index Struct
typedef struct {
    int from_index;
    int to_index;
} parameters;

//Global Arrays
int* origArray; 
int* sortedArray;
int threadCount = 1;

int main(int argc, char* argv[]) {
    int n;
    bool isOdd = false;

    //Parameter object for sorter and merger threads.
    parameters T1params;
    parameters T2params;

    //POSIX struct variables for the three threads.
    pthread_t sortThread1; 
    pthread_t sortThread2;
    pthread_t mergeThread;

    //Using dynamic arrays to get a dynamic size of the array from the user.
    printf("Enter the number of integers in the list: ");
    scanf("%d", &n);

    //Allocating memory for the arrays.
    origArray = malloc(n * sizeof(int));
    sortedArray = malloc(n * sizeof(int));

    //Error handling.
    if (origArray == NULL || sortedArray == NULL) {
        perror("Unable to allocate memory.\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        printf("Enter the integer #%d: ", i + 1);
        scanf("%d", &origArray[i]);
    }
    printf("\n");
    
    //If the size is odd, set the flag to true.
    //This will be used to accomodate the odd number of integers.
    //Setting up the parameters for Sorter Thread #1 as per the conditions.
    if ((n % 2) != 0) {
        isOdd = true;
        T1params.from_index = 0; 
        T1params.to_index = (n/2) + 1;
    }
    else {
        isOdd = false;
        T1params.from_index = 0; 
        T1params.to_index = n/2;
    }

    //Setting parameters for Sorter Thread #2 as per the conditions.
    T2params.from_index = T1params.to_index;
    T2params.to_index = n;

    //Printing the raw, unsorted array as received from input.
    printf("Unsorted List of Integers:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", origArray[i]);
    }
    printf("\n");

    //Creating Sorter Thread #1, and passing the parameters.
    if ((pthread_create(&sortThread1, NULL, sorter, &T1params)) != 0) {
        perror("Error creating thread 1");
        exit(1);
    }

    //Creating Sorter Thread #2, and passing the parameters.
    if ((pthread_create(&sortThread2, NULL, sorter, &T2params)) != 0) {
        perror("Error creating thread 2");
        exit(1);
    }
    //Waiting for the Sorter Threads to finish.
    pthread_join(sortThread2, NULL);

    //Creating the Merger Thread, and passing the parameters.
    if ((pthread_create(&mergeThread, NULL, merger, &T2params)) != 0) {
        perror("Error creating merger thread");
        exit(1);
    }
    //Waiting for merger thread to finish.
    pthread_join(mergeThread, NULL);

    //Printing the final sorted array.
    printf("----------------------------------------\n");
    printf("Final Sorted List of Integers:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", sortedArray[i]);
    }
    printf("\n");

    //Freeing the allocating space to ensure no resource leaks.
    free(origArray);
    free(sortedArray);
    return 0;
}

//Sorter Function:
void* sorter(void* params) {
    printf("----------------------------------------\n");
    printf("SORTING THREAD %d:\n", threadCount);

    //Getting the parameters from the struct.
    int from_index = ((parameters*)params)->from_index;
    int to_index = ((parameters*)params)->to_index;

    printf("Making sublist from index #%d to index #%d and sorting...\n", from_index, to_index);

    //Printing the unsorted sublist as per the parameters.
    printf("Unsorted Sublist:\n");
    for (int i = from_index; i < to_index; i++) {
        printf("%d ", origArray[i]);
    }
    
    //Sorting the sublist using bubble sort.
    for (int i = from_index; i < to_index; i++) {
        for (int j = i + 1; j < to_index; j++) {
            if (origArray[i] > origArray[j]) {
                int temp = origArray[i];
                origArray[i] = origArray[j];
                origArray[j] = temp;
            }
        }
    }

    //Printing the sorted sublist as per the parameters.
    printf("\nSorted Sublist:\n");
    for (int i = from_index; i < to_index; i++) {
        printf("%d ", origArray[i]);
    }
    
    printf("\nThread %d done, exiting..\n", threadCount);

    //Incrementing thread count.
    threadCount++; 

    //Returning NULL for successful execution.
    return NULL;
}

//Merger Function:
void* merger(void* params) {
    printf("----------------------------------------\n");
    printf("MERGER THREAD:\n", threadCount);

    //Getting the parameters from the struct.
    int from_index = ((parameters*)params)->from_index;
    int to_index = ((parameters*)params)->to_index;

    //Printing Sublist 1 from Thread 1.
    printf("Sublist 1: ");
    for (int i = 0; i < from_index; i++) {
        printf("%d ", origArray[i]);
    }

    //Printing Sublist 2 from Thread 2.
    printf("\nSublist 2: ");
    for (int i = from_index; i < to_index; i++) {
        printf("%d ", origArray[i]);
    }

    printf("\nMerging sublists...\n");
    
    //Copying the two sublists to the sorted array.
    for (int i = 0; i < to_index; i++) {
        sortedArray[i] = origArray[i];
    }

    //Sorting the final array.
    for (int i = 0; i < to_index; i++) {
        for (int j = i + 1; j < to_index; j++) {
            if (sortedArray[i] > sortedArray[j]) {
                int temp = sortedArray[i];
                sortedArray[i] = sortedArray[j];
                sortedArray[j] = temp;
            }
        }
    }

    printf("Merging done. Merger Thread exiting...\n");

    return NULL;
}
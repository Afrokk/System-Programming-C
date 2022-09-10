// Student Name: Afrasiyab Khan
// Student ID: 110034991

#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(int argc, char *argv) {
    //Declaring vars
    int n;
    int sum = 0, time = 0, temp;
    float avgWaitingTime;
    float avgTurnaroundTime;

    //Creating dynamic arrays for the process data:
    int *processNumber, *Ppriority, *waitingTime, *burstTime, *completionTime, *turnaroundTime;

    printf("Enter the Number of Processes: ");
    scanf("%d", &n);

    //Allocating Memory:
    processNumber = (int*)malloc(n * sizeof(int));
    Ppriority = (int*)malloc(n * sizeof(int));
    completionTime = (int*)malloc(n * sizeof(int));
    waitingTime = (int*)malloc(n * sizeof(int));
    burstTime = (int*)malloc(n * sizeof(int));
    turnaroundTime = (int*)malloc(n * sizeof(int));

    //Getting burst times and priority from the user:
    for (int i = 0; i < n; i++) {
        printf("Enter the Priority for Process #%d: ", i + 1);
        scanf("%d", &Ppriority[i]);
        printf("Enter the Burst Time for Process #%d: ", i + 1);
        scanf("%d", &burstTime[i]);
        processNumber[i]=i+1;
    }

    //Assuming all processes arrive at the same time.
    //Sorting the process numbers, burst times and priority ascending order:
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (Ppriority[i] > Ppriority[j]) {
                temp = Ppriority[i];
                Ppriority[i] = Ppriority[j];
                Ppriority[j] = temp;
                temp = burstTime[i];
                burstTime[i] = burstTime[j];
                burstTime[j] = temp;
                temp = processNumber[i];
                processNumber[i] = processNumber[j];
                processNumber[j] = temp;
            }
        }
    }
    //As waiting time for the first process is 0:
    waitingTime[0] = 0;

    //Calculating Waiting Time and Completion Time:
    for (int i = 0; i < n; i++) {
        completionTime[i] = waitingTime[i] + burstTime[i];
        waitingTime[i + 1] = completionTime[i];
    }

    //Calculating Average Turnaround Time:
    for (int i = 0; i < n; i++) {
        temp += completionTime[i];
    }
    avgTurnaroundTime = (float)temp / n;

    //Calculating Average Waiting Time:
    for (int i = 0; i < n; i++) {
        sum += waitingTime[i];
    }
    avgWaitingTime = (float)sum / n;

    //Printing the results in a tabular form:
    printf("\n*Execution order from bottom to top, priority-wise.(non-preemptive approach)\n");
    printf("\nProcess\t\tPriority\tBurst Time\tCompletion Time\t\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("P#%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n", processNumber[i], Ppriority[i], burstTime[i], completionTime[i], waitingTime[i]);
    }
    printf("\nAverage Waiting Time: %.2f time units.\n", avgWaitingTime);
    printf("Average Turnaround Time: %.2f time units.\n", avgTurnaroundTime);

    //Freeing the memory:
    free(processNumber);
    free(Ppriority);
    free(waitingTime);
    free(burstTime);
    free(completionTime);
    free(turnaroundTime);

    return 0;
}
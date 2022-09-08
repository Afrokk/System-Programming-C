#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define LIMIT 1024

int childFunction (char *inputExpression) {
    int N1, N2;
    float DivN;
    char operator;
    char result[LIMIT];

    write(1, "I am a child working for my parent\n", sizeof("I am a child working for my parent\n"));

    sscanf(inputExpression, "%d %c %d", &N1, &operator, &N2);

    if ((sscanf(inputExpression, "%d %c %d", &N1, &operator, &N2)) != 3) {
        exit(50);
    }
    if (operator == '/' && N2 == 0) {
        exit(100);
    }

    switch (operator) {
    case '+':
        sprintf(result, "%d %c %d = %d\n\n", N1, operator, N2, N1 + N2);
        break;

    case '/':
        DivN = N1 / N2;
        sprintf(result, "\n%d %c %d = %.3f\n\n", N1, operator, N2, DivN);
        break;

    case '*':
        sprintf(result, "\n%d %c %d = %d\n\n", N1, operator, N2, N1 * N2);
        break;

    case '-':
        sprintf(result, "%d %c %d = %d\n\n", N1, operator, N2, N1 - N2);
        break;

    default:
        exit(200);
        break;
    }
    printf("%s", result);
}

int main(int argc, char *argv[]) {
    char inputExpression[LIMIT];
    int pID, childStatus;

    write(1, "This program makes simple arithmetics.\n", sizeof("This program makes simple arithmetics.\n"));

    while (1) {
        write(1, "Enter an arithmetic statement, e.g., 34 + 132 > ", sizeof("Enter an arithmetic statement, e.g., 34 + 132 > "));
        scanf("%1024s", inputExpression);

        //Exit Condition
        if ((strcmp ("exit", inputExpression)) == 0) {
            exit(0);
        }

        pID = fork();

        if (pID == -1) {
            write (1, "Forking not possible.\n", sizeof("Forking not possible\n"));
            exit(0);
        }

        else if (pID == 0) {
            childFunction(inputExpression);
        }

        else {
            write (1, "\n\nCreated a child to make your operation, waiting...\n\n", sizeof("\n\nCreated a child to make your operation, waiting...\n\n"));
            wait(&childStatus);
            if (WEXITSTATUS(childStatus) == 50)
                write(1, "Wrong Statement.\n", sizeof("Wrong Statement.\n"));
            else if (WEXITSTATUS(childStatus) == 100)
                write(1, "Division by Zero.\n", sizeof("Division by Zero.\n"));
            else if (WEXITSTATUS(childStatus) == 200)
                write(1, "Wrong Operator.\n", sizeof("Wrong Operator.\n"));
        }
    }
    exit(0);
}
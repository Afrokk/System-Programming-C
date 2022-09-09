/* Write a C Program to do the following.

1 Ask for two integer values from the user.
2 The user supplies the two integer values.
3 Calculate the sum of the two integer values and display the result on the console.
4 Use standard i/o functions to write these two integer values and their sum to a file.
5 Use system i/o functions to read the two integer values and their sum from the file in step 4.
6 Display the two integer values and their sum read from step 5 on the console. */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int num1, num2, sum; 

    //Taking input from the user.
    printf("Enter first integer: ");
    scanf("%d", &num1);
    printf("Enter second integer: ");
    scanf("%d", &num2);

    sum = num1 + num2; 
    printf("The sum of the two integers is: %d\n", sum);

    //Using standard io to write to a file:
    FILE* fp = fopen ("standardIOOutput.txt", "w+");

    //Error handling.
    if (fp < 0) {
        printf("Error: Can't write the file.\n");
        return 1;
    }

    fprintf(fp, "%d\n%d\n%d\n", num1, num2, sum);

     //Closing FILE* fp as it is no longer needed. 
    fclose(fp);    

    //Reading above file via System io: 
    //Opening the file in read only mode.
    int fd = open ("standardIOOutput.txt", O_RDONLY);
    char ch;

     //Error handling.
    if (fp < 0) {
        write(1, "Error: Can't write the file.\n", sizeof("Error: Can't write the file.\n"));
        exit(1);
    }

    //Output formatting:
    write(1, "\nTwo integers and their sum generated in step 4 file are: \n", sizeof("\nTwo integers and their sum generated in step 4 file are: \n"));

    //Reading the file byte by byte and outputting to the console.
    while(read(fd, &ch, 1)) {
        write(1, &ch, 1);
    }

    close(fd);
    return 0;
}

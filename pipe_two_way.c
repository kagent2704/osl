//Two way communication using pipe
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#define MAX 20

int main()
{
    int pipe1[2], pipe2[2], n;
    char string1[MAX], string2[MAX];
    pid_t pid;

    if (pipe(pipe1) < 0 || pipe(pipe2) < 0)
    {
        printf("Pipe creation error\n");
        exit(0);
    }


    if ((pid=fork())<0)
    {
        printf("Fork error");
        exit(0);
    }

    if(pid>0)
    {
        close(pipe1[0]);
        close(pipe2[1]);

        printf("Parent: Enter a string for child: ");
        fgets(string1, MAX, stdin);

        write(pipe1[1], string1, MAX);

        n = read(pipe2[0], string2, MAX);
        string2[n] = '\0';
        printf("Parent received from child: %s\n", string2);
    }
    if (pid==0)
    {
        close(pipe1[1]);
        close(pipe2[0]);

        n = read(pipe1[0], string2, MAX);
        string2[n] = '\0';
        printf("Child received from parent: %s\n", string2);

        printf("Child: Enter a string for parent: ");
        fgets(string1, MAX, stdin);

        write(pipe2[1], string1, MAX);
    }

    exit(0);
}
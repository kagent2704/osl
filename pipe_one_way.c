//One way communication using pipe
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#define MAX 20

int main()
{
    int filedes[2],n;
    char string[MAX];
    char line[MAX];
    pid_t pid;

    printf("Enter a string to be given to the parent: ");
    fflush(stdin);
    fgets(string,MAX,stdin);



    if(pipe(filedes)<0)
    {
        printf("Pipe creation error");
        exit(0);
    }

    if ((pid=fork())<0)
    {
        printf("Fork error");
        exit(0);
    }

    if(pid>0)
    {
        close(filedes[0]);
        write(filedes[1],string,MAX);
    }
    if (pid==0)
    {
        close(filedes[1]);
        n=read(filedes[0],line,MAX);
        line[n]='\0';
        printf("\n\nData read by child is: %s\n",line);
    }

    exit(0);
}
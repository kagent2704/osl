#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
    int pid=fork();
    if(pid>0)
    {
        printf("Parent Process id %d\n", getpid());
        printf("Child Process id %d\n", pid);
    }
    else if(pid ==0)
    {
        sleep(10);
        printf("Child process id %d\n", getpid());
        printf("parent process if %d\n", getpid());
    }
    return 0;
}

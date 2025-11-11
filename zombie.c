/* zombie.c
 * Demonstrates a zombie (defunct) child process.
 * Compile: gcc -Wall -o zombie zombie.c
 * Run:     ./zombie
 *
 * While the parent sleeps, run `ps -l` or `ps aux | grep defunct` in another terminal
 * to observe the child in state Z (zombie).
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    pid_t pid;

    printf("Parent PID: %d\n", (int)getpid());

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        /* Child */
        printf("Child: PID %d — exiting immediately to become a zombie.\n", (int)getpid());
        /* _exit to avoid flushing parent's stdio buffers twice */
        _exit(0);
    } else {
        /* Parent: do not call wait() — sleep so zombie persists for observation */
        printf("Parent: created child PID %d. Now sleeping for 30 seconds.\n", (int)pid);
        printf("During this time, run `ps -l` or `ps aux | grep %d` in another terminal.\n", (int)pid);
        sleep(30);  /* zombie exists while parent sleeps */
        printf("Parent: woke up. Now exiting (child will be reaped by init if parent exits).\n");
    }

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <unistd.h>

int arr[100], n;

void forkexample()
{
    if (fork() == 0) {
        // Child process – Descending sort
        printf("\nHello from child process. Sorting in descending order:\n");
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] < arr[j + 1]) {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }

        printf("Sorted array (Descending): ");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");

    } else {
        sleep(1);  // Let child finish first (optional)
        // Parent process – Ascending sort
        printf("\nHello from parent process. Sorting in ascending order:\n");
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }

        printf("Sorted array (Ascending): ");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
}

int main()
{
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter %d elements to push: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    forkexample();
    return 0;
}


#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3

int available[MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];

bool isSafe();
bool isLessThanOrEqual(int a[], int b[]);
void requestResources(int process, int request[]);

int main() {
    
    printf("Enter available resources (separated by spaces): ");
    for (int i = 0; i < MAX_RESOURCES; i++) {
        scanf("%d", &available[i]);
    }
    printf("Enter maximum needs and initial allocation for each process:\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        printf("Process %d:\n", i);
        for (int j = 0; j < MAX_RESOURCES; j++) {
            printf("  Max: ");
            scanf("%d", &max[i][j]);
            printf("  Allocation: ");
            scanf("%d", &allocation[i][j]);
        }
    }

    for (int i = 0; i < MAX_PROCESSES; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    if (isSafe()) {
        printf("System is in a safe state initially.\n");
    } else {
        printf("System is in an unsafe state initially.\n");
    }
    int process, request[MAX_RESOURCES];
    while (1) {
        printf("Enter process number (0-%d) or -1 to exit: ", MAX_PROCESSES - 1);
        scanf("%d", &process);
        if (process == -1) {
            break;
        }

        printf("Enter resource requests (separated by spaces): ");
        for (int i = 0; i < MAX_RESOURCES; i++) {
            scanf("%d", &request[i]);
        }

        requestResources(process, request);
    }

    return 0;
}

bool isSafe() {
    bool finish[MAX_PROCESSES] = {false};
    int work[MAX_RESOURCES];

    for (int i = 0; i < MAX_RESOURCES; i++) {
        work[i] = available[i];
    }

    int count = 0;
    while (count < MAX_PROCESSES) {
        int i;
        for (i = 0; i < MAX_PROCESSES; i++) {
            if (!finish[i] && isLessThanOrEqual(need[i], work)) {
                break;
            }
        }

        if (i == MAX_PROCESSES) {
            return false;
        }

        for (int j = 0; j < MAX_RESOURCES; j++) {
            work[j] += allocation[i][j];
        }

        finish[i] = true;
        count++;
    }

    return true;
}

bool isLessThanOrEqual(int a[], int b[]) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (a[i] > b[i]) {
            return false;
        }
    }
    return true;
}

void requestResources(int process, int request[]) {
    if (!isLessThanOrEqual(request, need[process])) {
        printf("Error: Process %d requested more than its maximum need.\n", process);
        return;
    }

    if (!isLessThanOrEqual(request, available)) {
        printf("Error: Process %d requested more resources than available.\n", process);
        return;
    }

    for (int i = 0; i < MAX_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }

    if (isSafe()) {
        printf("Request granted. System is in a safe state.\n");
    } else {
        printf("Request denied. System would be in an unsafe state.\n");
        for (int i = 0; i < MAX_RESOURCES; i++) {
            available[i] += request[i];
            allocation[process][i] -= request[i];
            need[process][i] += request[i];
        }
    }
}

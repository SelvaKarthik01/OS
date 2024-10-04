#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_ROBOTS 4
#define NUM_ROUNDS 10
#define TIME_SLICE 2 // Time slice for round robin scheduling in seconds
#define RESOURCE_TIMEOUT 3 // Timeout for acquiring resources in seconds

// Mutex for each resource
pthread_mutex_t screwdriver;
pthread_mutex_t hammer;
pthread_mutex_t workstation;

// Maximum resources for each type
#define MAX_SCREWDRIVER 2
#define MAX_HAMMER 2
#define MAX_WORKSTATION 2

// Current resources
int available_screwdriver = MAX_SCREWDRIVER;
int available_hammer = MAX_HAMMER;
int available_workstation = MAX_WORKSTATION;

// Enum to represent robot types
typedef enum {
    ROBOT_TYPE_A, // Requires Screwdriver and Hammer
    ROBOT_TYPE_B, // Requires Hammer and Workstation
    ROBOT_TYPE_C, // Requires Screwdriver and Workstation
    ROBOT_TYPE_D  // Requires all resources
} RobotType;

// Struct to store metrics for each robot
typedef struct {
    int robot_id;
    int tasks_completed;
    float total_execution_time;
    int resources_used;  // Count of resources acquired
    int max_screwdriver;
    int max_hammer;
    int max_workstation;
    int allocated_screwdriver;
    int allocated_hammer;
    int allocated_workstation;
} RobotMetrics;

// Global metrics array
RobotMetrics metrics[NUM_ROBOTS];

// Robot task
void *robot(void *arg) {
    int robot_id = *(int *)arg;
    RobotType robot_type = robot_id % NUM_ROBOTS; // Assign type based on ID
    int round_time; // Variable to track time spent in each round

    for (int round = 0; round < NUM_ROUNDS; round++) {
        printf("Robot %d (Type %d) is waiting for resources in round %d...\n", robot_id, robot_type, round + 1);
        time_t start_time = time(NULL); // Start time for this round

        // Introduce randomness in resource requirements
        int require_screwdriver = rand() % 2;
        int require_hammer = rand() % 2;
        int require_workstation = rand() % 2;

        // Set maximum resources required based on robot type
        if (robot_type == ROBOT_TYPE_A) {
            metrics[robot_id - 1].max_screwdriver = 1;
            metrics[robot_id - 1].max_hammer = 1;
            metrics[robot_id - 1].max_workstation = 0;
        } else if (robot_type == ROBOT_TYPE_B) {
            metrics[robot_id - 1].max_screwdriver = 0;
            metrics[robot_id - 1].max_hammer = 1;
            metrics[robot_id - 1].max_workstation = 1;
        } else if (robot_type == ROBOT_TYPE_C) {
            metrics[robot_id - 1].max_screwdriver = 1;
            metrics[robot_id - 1].max_hammer = 0;
            metrics[robot_id - 1].max_workstation = 1;
        } else if (robot_type == ROBOT_TYPE_D) {
            metrics[robot_id - 1].max_screwdriver = 1;
            metrics[robot_id - 1].max_hammer = 1;
            metrics[robot_id - 1].max_workstation = 1;
        }

        // Resource acquisition with timeout
        time_t resource_start_time = time(NULL);
        while ((time(NULL) - resource_start_time) < RESOURCE_TIMEOUT) {
            // Try to acquire screwdriver
            if (require_screwdriver && metrics[robot_id - 1].allocated_screwdriver < metrics[robot_id - 1].max_screwdriver) {
                pthread_mutex_lock(&screwdriver);
                if (available_screwdriver > 0) {
                    metrics[robot_id - 1].allocated_screwdriver++;
                    available_screwdriver--;
                    printf("Robot %d acquired Screwdriver\n", robot_id);
                }
                pthread_mutex_unlock(&screwdriver);
            }

            // Try to acquire hammer
            if (require_hammer && metrics[robot_id - 1].allocated_hammer < metrics[robot_id - 1].max_hammer) {
                pthread_mutex_lock(&hammer);
                if (available_hammer > 0) {
                    metrics[robot_id - 1].allocated_hammer++;
                    available_hammer--;
                    printf("Robot %d acquired Hammer\n", robot_id);
                }
                pthread_mutex_unlock(&hammer);
            }

            // Try to acquire workstation
            if (require_workstation && metrics[robot_id - 1].allocated_workstation < metrics[robot_id - 1].max_workstation) {
                pthread_mutex_lock(&workstation);
                if (available_workstation > 0) {
                    metrics[robot_id - 1].allocated_workstation++;
                    available_workstation--;
                    printf("Robot %d acquired Workstation\n", robot_id);
                }
                pthread_mutex_unlock(&workstation);
            }

            // Break if all required resources are acquired
            if ((metrics[robot_id - 1].allocated_screwdriver == metrics[robot_id - 1].max_screwdriver) &&
                (metrics[robot_id - 1].allocated_hammer == metrics[robot_id - 1].max_hammer) &&
                (metrics[robot_id - 1].allocated_workstation == metrics[robot_id - 1].max_workstation)) {
                break;
            }

            // Short sleep to prevent tight looping
            usleep(100000); // Sleep for 0.1 seconds
        }

        // Simulate robot task if all required resources are acquired
        if (metrics[robot_id - 1].allocated_screwdriver > 0 ||
            metrics[robot_id - 1].allocated_hammer > 0 ||
            metrics[robot_id - 1].allocated_workstation > 0) {
            printf("Robot %d is performing its task.\n", robot_id);
            sleep(rand() % 2 + 1);  // Simulate some work being done

            // Release resources after task completion
            if (metrics[robot_id - 1].allocated_workstation > 0) {
                metrics[robot_id - 1].allocated_workstation--;
                available_workstation++;
                printf("Robot %d released Workstation\n", robot_id);
            }

            if (metrics[robot_id - 1].allocated_hammer > 0) {
                metrics[robot_id - 1].allocated_hammer--;
                available_hammer++;
                printf("Robot %d released Hammer\n", robot_id);
            }

            if (metrics[robot_id - 1].allocated_screwdriver > 0) {
                metrics[robot_id - 1].allocated_screwdriver--;
                available_screwdriver++;
                printf("Robot %d released Screwdriver\n", robot_id);
            }

            // Increment tasks completed
            metrics[robot_id - 1].tasks_completed++;
        } else {
            printf("Robot %d failed to acquire required resources in time.\n", robot_id);
        }

        // Calculate execution time for this round
        round_time = time(NULL) - start_time;
        metrics[robot_id - 1].total_execution_time += round_time;

        // Simulate a break before the next round
        sleep(1);
    }

    printf("Robot %d completed all rounds.\n", robot_id);
    return NULL;
}

int main() {
    pthread_t robot_threads[NUM_ROBOTS];
    int robot_ids[NUM_ROBOTS];

    // Initialize mutexes for resources
    pthread_mutex_init(&screwdriver, NULL);
    pthread_mutex_init(&hammer, NULL);
    pthread_mutex_init(&workstation, NULL);

    // Seed random number generator
    srand(time(NULL));

    // Create robot threads
    for (int i = 0; i < NUM_ROBOTS; i++) {
        robot_ids[i] = i + 1;
        metrics[i].robot_id = robot_ids[i]; // Initialize metrics
        pthread_create(&robot_threads[i], NULL, robot, &robot_ids[i]);
    }

    // Implement round robin scheduling
    for (int i = 0; i < NUM_ROUNDS; i++) {
        for (int j = 0; j < NUM_ROBOTS; j++) {
            // Wait for the time slice
            sleep(TIME_SLICE);
        }
    }

    // Wait for all robots to complete their tasks
    for (int i = 0; i < NUM_ROBOTS; i++) {
        pthread_join(robot_threads[i], NULL);
    }

    // Destroy the mutexes after use
    pthread_mutex_destroy(&screwdriver);
    pthread_mutex_destroy(&hammer);
    pthread_mutex_destroy(&workstation);

    // Display metrics
    printf("\n=== Metrics ===\n");
    for (int i = 0; i < NUM_ROBOTS; i++) {
        printf("Robot %d:\n", metrics[i].robot_id);
        printf("  Tasks Completed: %d\n", metrics[i].tasks_completed);
        printf("  Total Execution Time: %.2f seconds\n", metrics[i].total_execution_time);
        printf("  Resources Used: %d\n", metrics[i].tasks_completed); // Count of resources acquired
    }

    return 0;
}

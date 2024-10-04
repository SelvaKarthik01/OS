#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_ROBOTS 4
#define RESOURCE_TIMEOUT 5
#define MAX_RETRIES 3 // Maximum number of retries to acquire resources

// Resource availability
int available_screwdriver = 2;
int available_hammer = 2;
int available_workstation = 2;

// Mutexes for resources
pthread_mutex_t screwdriver = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t hammer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t workstation = PTHREAD_MUTEX_INITIALIZER;

// Metrics structure for each robot
typedef struct {
    int tasks_completed;
    float total_execution_time;
    int resources_used;
    int allocated_screwdriver;
    int allocated_hammer;
    int allocated_workstation;
    int max_screwdriver;
    int max_hammer;
    int max_workstation;
} RobotMetrics;

RobotMetrics metrics[NUM_ROBOTS];

// Function for robot tasks
void *robot_task(void *arg) {
    int robot_id = *((int *)arg);
    
    // Randomly determine if screwdriver, hammer, or workstation is needed
    int require_screwdriver = rand() % 2; // Randomly determine if screwdriver is needed
    int require_hammer = rand() % 2;       // Randomly determine if hammer is needed
    int require_workstation = rand() % 2;  // Randomly determine if workstation is needed
    
    // Ensure that at least one resource is required
    if (require_screwdriver + require_hammer + require_workstation == 0) {
        require_screwdriver = 1; // Ensure at least one resource is needed
    }

    // Initialize metrics
    metrics[robot_id].max_screwdriver = require_screwdriver ? 1 : 0;
    metrics[robot_id].max_hammer = require_hammer ? 1 : 0;
    metrics[robot_id].max_workstation = require_workstation ? 1 : 0;

    // Start resource acquisition with timeout
    time_t resource_start_time = time(NULL);
    int retries = 0;

    while ((time(NULL) - resource_start_time) < RESOURCE_TIMEOUT && retries < MAX_RETRIES) {
        // Try to acquire screwdriver
        if (require_screwdriver && metrics[robot_id].allocated_screwdriver < metrics[robot_id].max_screwdriver) {
            pthread_mutex_lock(&screwdriver);
            if (available_screwdriver > 0) {
                metrics[robot_id].allocated_screwdriver++;
                available_screwdriver--;
                metrics[robot_id].resources_used++; // Increment resources used
                printf("Robot %d acquired Screwdriver\n", robot_id);
            }
            pthread_mutex_unlock(&screwdriver);
        }

        // Try to acquire hammer
        if (require_hammer && metrics[robot_id].allocated_hammer < metrics[robot_id].max_hammer) {
            pthread_mutex_lock(&hammer);
            if (available_hammer > 0) {
                metrics[robot_id].allocated_hammer++;
                available_hammer--;
                metrics[robot_id].resources_used++; // Increment resources used
                printf("Robot %d acquired Hammer\n", robot_id);
            }
            pthread_mutex_unlock(&hammer);
        }

        // Try to acquire workstation
        if (require_workstation && metrics[robot_id].allocated_workstation < metrics[robot_id].max_workstation) {
            pthread_mutex_lock(&workstation);
            if (available_workstation > 0) {
                metrics[robot_id].allocated_workstation++;
                available_workstation--;
                metrics[robot_id].resources_used++; // Increment resources used
                printf("Robot %d acquired Workstation\n", robot_id);
            }
            pthread_mutex_unlock(&workstation);
        }

        // Break if all required resources are acquired
        if ((metrics[robot_id].allocated_screwdriver == metrics[robot_id].max_screwdriver) &&
            (metrics[robot_id].allocated_hammer == metrics[robot_id].max_hammer) &&
            (metrics[robot_id].allocated_workstation == metrics[robot_id].max_workstation)) {
            break;
        }

        // Increment retries if resources are not acquired
        retries++;

        // Short sleep to prevent tight looping
        usleep(100000); // Sleep for 0.1 seconds
    }

    // Simulate task execution if resources were successfully acquired
    if (metrics[robot_id].allocated_screwdriver > 0 || metrics[robot_id].allocated_hammer > 0 || metrics[robot_id].allocated_workstation > 0) {
        metrics[robot_id].tasks_completed++;
        metrics[robot_id].total_execution_time += 10.0; // Simulate some execution time
        printf("Robot %d completed a task.\n", robot_id);
    }

    return NULL;
}

int main() {
    pthread_t robots[NUM_ROBOTS];
    int robot_ids[NUM_ROBOTS];

    // Initialize random seed
    srand(time(NULL));

    // Create robot threads
    for (int i = 0; i < NUM_ROBOTS; i++) {
        robot_ids[i] = i;
        pthread_create(&robots[i], NULL, robot_task, &robot_ids[i]);
    }

    // Wait for all robots to finish
    for (int i = 0; i < NUM_ROBOTS; i++) {
        pthread_join(robots[i], NULL);
    }

    // Print summary
    for (int i = 0; i < NUM_ROBOTS; i++) {
        printf("Robot %d:\n", i);
        printf("  Tasks Completed: %d\n", metrics[i].tasks_completed);
        printf("  Total Execution Time: %.2f seconds\n", metrics[i].total_execution_time);
        printf("  Resources Used: %d\n", metrics[i].resources_used);
    }

    return 0;
}

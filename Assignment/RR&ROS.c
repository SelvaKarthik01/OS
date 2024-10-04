#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_ROBOTS 4
#define NUM_ROUNDS 10

// Mutex for each resource
pthread_mutex_t screwdriver;
pthread_mutex_t hammer;
pthread_mutex_t workstation;

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
} RobotMetrics;

// Global metrics array
RobotMetrics metrics[NUM_ROBOTS];

// Global variable to track the current robot in the round-robin schedule
int current_robot = 0;

// Robot task
void *robot(void *arg) {
    int robot_id = *(int *)arg;
    RobotType robot_type = robot_id % NUM_ROBOTS; // Assign type based on ID
    int round_time; // Variable to track time spent in each round

    for (int round = 0; round < NUM_ROUNDS; round++) {
        // Wait for the turn in round robin
        while (current_robot != robot_id - 1) {
            usleep(100); // Sleep for a short duration to prevent busy waiting
        }

        printf("Robot %d (Type %d) is waiting for resources in round %d...\n", robot_id, robot_type, round + 1);
        time_t start_time = time(NULL); // Start time for this round

        // Introduce randomness in resource requirements
        int require_screwdriver = rand() % 2;
        int require_hammer = rand() % 2;
        int require_workstation = rand() % 2;

        // Acquire resources based on robot type and random requirements
        if (require_screwdriver && robot_type != ROBOT_TYPE_B) {
            pthread_mutex_lock(&screwdriver);
            printf("Robot %d acquired Screwdriver\n", robot_id);
            metrics[robot_id - 1].resources_used++; // Increment resources used
        }

        if (require_hammer && robot_type != ROBOT_TYPE_C) {
            pthread_mutex_lock(&hammer);
            printf("Robot %d acquired Hammer\n", robot_id);
            metrics[robot_id - 1].resources_used++; // Increment resources used
        }

        if (require_workstation) {
            pthread_mutex_lock(&workstation);
            printf("Robot %d acquired Workstation\n", robot_id);
            metrics[robot_id - 1].resources_used++; // Increment resources used
        }

        // Simulate robot task
        printf("Robot %d is performing its task.\n", robot_id);
        sleep(rand() % 2 + 1);  // Simulate some work being done

        // Release resources after task completion
        if (require_workstation) {
            pthread_mutex_unlock(&workstation);
            printf("Robot %d released Workstation\n", robot_id);
        }

        if (require_hammer && robot_type != ROBOT_TYPE_C) {
            pthread_mutex_unlock(&hammer);
            printf("Robot %d released Hammer\n", robot_id);
        }

        if (require_screwdriver && robot_type != ROBOT_TYPE_B) {
            pthread_mutex_unlock(&screwdriver);
            printf("Robot %d released Screwdriver\n", robot_id);
        }

        // Calculate execution time for this round
        round_time = time(NULL) - start_time;
        metrics[robot_id - 1].total_execution_time += round_time;

        // Increment tasks completed
        metrics[robot_id - 1].tasks_completed++;

        // Move to the next robot in the round robin
        current_robot = (current_robot + 1) % NUM_ROBOTS;

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
        printf("  Resources Used: %d\n", metrics[i].resources_used);
    }

    printf("All robots have finished their tasks.\n");

    return 0;
}

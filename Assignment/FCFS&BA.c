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

// Check if the current allocation is safe
int is_safe_state() {
    int work_screwdriver = available_screwdriver;
    int work_hammer = available_hammer;
    int work_workstation = available_workstation;
    
    int finish[NUM_ROBOTS] = {0};
    int can_finish;

    // Try to find a safe sequence
    for (int i = 0; i < NUM_ROBOTS; i++) {
        can_finish = 0;
        for (int j = 0; j < NUM_ROBOTS; j++) {
            if (!finish[j] && metrics[j].allocated_screwdriver <= work_screwdriver &&
                metrics[j].allocated_hammer <= work_hammer &&
                metrics[j].allocated_workstation <= work_workstation) {
                work_screwdriver += metrics[j].allocated_screwdriver;
                work_hammer += metrics[j].allocated_hammer;
                work_workstation += metrics[j].allocated_workstation;
                finish[j] = 1;
                can_finish = 1;
            }
        }
        if (!can_finish) break; // No safe allocation possible
    }

    // Check if all robots can finish
    for (int i = 0; i < NUM_ROBOTS; i++) {
        if (!finish[i]) return 0; // Not safe
    }
    return 1; // Safe
}

// Robot task
void *robot(void *arg) {
    int robot_id = *(int *)arg;
    RobotType robot_type = robot_id % NUM_ROBOTS; // Assign type based on ID
    int round_time; // Variable to track time spent in each round

    for (int round = 0; round < NUM_ROUNDS; round++) {
        printf("Robot %d (Type %d) is waiting for resources in round %d...\n", robot_id, robot_type, round + 1);
        time_t start_time = time(NULL); // Start time for this round

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

        // Attempt to acquire resources
        pthread_mutex_lock(&screwdriver);
        if (metrics[robot_id - 1].allocated_screwdriver < metrics[robot_id - 1].max_screwdriver && available_screwdriver > 0) {
            metrics[robot_id - 1].allocated_screwdriver++;
            available_screwdriver--;
            printf("Robot %d acquired Screwdriver\n", robot_id);
        }
        pthread_mutex_unlock(&screwdriver);

        pthread_mutex_lock(&hammer);
        if (metrics[robot_id - 1].allocated_hammer < metrics[robot_id - 1].max_hammer && available_hammer > 0) {
            metrics[robot_id - 1].allocated_hammer++;
            available_hammer--;
            printf("Robot %d acquired Hammer\n", robot_id);
        }
        pthread_mutex_unlock(&hammer);

        pthread_mutex_lock(&workstation);
        if (metrics[robot_id - 1].allocated_workstation < metrics[robot_id - 1].max_workstation && available_workstation > 0) {
            metrics[robot_id - 1].allocated_workstation++;
            available_workstation--;
            printf("Robot %d acquired Workstation\n", robot_id);
        }
        pthread_mutex_unlock(&workstation);

        // Simulate robot task
        if (metrics[robot_id - 1].allocated_screwdriver > 0 || 
            metrics[robot_id - 1].allocated_hammer > 0 || 
            metrics[robot_id - 1].allocated_workstation > 0) {
            printf("Robot %d is performing its task.\n", robot_id);
            sleep(rand() % 2 + 1);  // Simulate some work being done
            // Increment resources used after task
            metrics[robot_id - 1].resources_used++;
        }

        // Release resources after task completion
        if (metrics[robot_id - 1].allocated_workstation > 0) {
            pthread_mutex_lock(&workstation);
            metrics[robot_id - 1].allocated_workstation--;
            available_workstation++;
            printf("Robot %d released Workstation\n", robot_id);
            pthread_mutex_unlock(&workstation);
        }

        if (metrics[robot_id - 1].allocated_hammer > 0) {
            pthread_mutex_lock(&hammer);
            metrics[robot_id - 1].allocated_hammer--;
            available_hammer++;
            printf("Robot %d released Hammer\n", robot_id);
            pthread_mutex_unlock(&hammer);
        }

        if (metrics[robot_id - 1].allocated_screwdriver > 0) {
            pthread_mutex_lock(&screwdriver);
            metrics[robot_id - 1].allocated_screwdriver--;
            available_screwdriver++;
            printf("Robot %d released Screwdriver\n", robot_id);
            pthread_mutex_unlock(&screwdriver);
        }

        // Calculate execution time for this round
        round_time = time(NULL) - start_time;
        metrics[robot_id - 1].total_execution_time += round_time;

        // Increment tasks completed
        metrics[robot_id - 1].tasks_completed++;

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

    // Wait for all robot threads to finish
    for (int i = 0; i < NUM_ROBOTS; i++) {
        pthread_join(robot_threads[i], NULL);
    }

    // Print metrics after all robots have finished
    printf("\n--- Robot Metrics ---\n");
    for (int i = 0; i < NUM_ROBOTS; i++) {
        printf("Robot ID: %d, Tasks Completed: %d, Total Execution Time: %.2f seconds, Resources Used: %d\n",
            metrics[i].robot_id, metrics[i].tasks_completed, metrics[i].total_execution_time, metrics[i].resources_used);
    }

    // Destroy mutexes
    pthread_mutex_destroy(&screwdriver);
    pthread_mutex_destroy(&hammer);
    pthread_mutex_destroy(&workstation);

    return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

// Define message structure
struct msgbuf {
    long mtype;
    char mtext[100];
};

int main(void) {
    int mqID;
    key_t key;
    struct msgbuf buf;
    int roll_number;

    key = 333; // Unique key for the message queue
    
    // Create the message queue
    if ((mqID = msgget(key, 0666 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Message Queue Created...\n");

    while (1) {
        printf("\nEnter last two digits of roll number (01-60): ");
        scanf("%d", &roll_number);
        
        if (roll_number < 1 || roll_number > 60) {
            printf("Invalid roll number. Please enter a valid roll number (01-60).\n");
            continue;
        }

        buf.mtype = roll_number; // Set message type to roll number

        printf("Enter amount to be paid: ");
        scanf("%s", buf.mtext); // Read the amount as a string

        // Send the message to the message queue
        if (msgsnd(mqID, &buf, sizeof(buf.mtext), 0) == -1) {
            perror("msgsnd");
        }

        // Optionally, exit if a specific roll number is chosen (e.g., 00)
        if (roll_number == 0) {
            break;
        }
    }

    // Cleanup: remove the message queue
    msgctl(mqID, IPC_RMID, NULL);
    printf("\nExited...\n");
    return 0;
}


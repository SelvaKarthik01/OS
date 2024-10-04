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

    key = 333; // Same key used by the server
    
    // Connect to the message queue
    if ((mqID = msgget(key, 0666)) == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Message Queue Connected...\n");

    printf("Enter your last two digits of roll number (01-60) to receive the amount: ");
    scanf("%d", &roll_number);

    if (roll_number < 1 || roll_number > 60) {
        printf("Invalid roll number. Exiting...\n");
        exit(1);
    }

    while (1) {
        // Receive the message from the queue where mtype is the roll number
        if (msgrcv(mqID, &buf, sizeof(buf.mtext), roll_number, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        printf("Message Received for Roll Number %02d: Amount to be paid is %s\n", roll_number, buf.mtext);

        // Optionally, break out of the loop if a specific condition is met (e.g., receiving a "bye" message)
        if (strcmp(buf.mtext, "bye") == 0) {
            break;
        }
    }

    printf("\nExited...\n");
    return 0;
}


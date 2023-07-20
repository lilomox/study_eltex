#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define ROWS 100
#define COLS 50
#define NUM_NAME 3
#define NUM_MSG 1

struct group {
    long mtype; 
    char marray[ROWS][COLS];  
    int count;
};

struct message {
    long mtype;
    char mtext[ROWS];
};

void *prints() {
    key_t key_grp;

    int msgid_grp;
    struct group grp;

    key_grp = ftok("server.c", 'B');
    msgid_grp = msgget(key_grp, 0666);
    while (1) {
        msgrcv(msgid_grp, &grp, sizeof(grp), 1, 0);

        system("clear");
        for (int i = 0; i < grp.count; i++) {
            printf("%s", grp.marray[i]);
        }
        
    }
}

void *writs() {
    char name[ROWS];

    key_t key_msg;
    int msgid_msg;
    
    struct message msg;

    key_msg = ftok("client.c", 'A');
    msgid_msg = msgget(key_msg, 0666);

    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);

    strcpy(msg.mtext, "New user - ");
    strcat(msg.mtext, name);
    msg.mtype = 1;
    name[strlen(name)-1] = ':';
    strcat(name, " ");

    msgsnd(msgid_msg, &msg, sizeof(msg), 0);

    strcpy(msg.mtext, "\n");
    msg.mtype = 1;
    msgsnd(msgid_msg, &msg, sizeof(msg), 0);

    char buf[ROWS];
    while (1) {
        strcpy(buf, " ");
        fgets(buf, sizeof(buf), stdin);

        strcpy(msg.mtext, name);
        strcat(msg.mtext, buf);
        msg.mtype = 1;
        msgsnd(msgid_msg, &msg, sizeof(msg), 0);

        strcpy(msg.mtext, "\n");
        msg.mtype = 1;
        msgsnd(msgid_msg, &msg, sizeof(msg), 0);
    }
    return NULL;
}

int main() {
    pthread_t potok, potok2;

    pthread_create(&potok, NULL, prints, NULL);
    pthread_create(&potok2, NULL, writs, NULL);

    pthread_join(potok, NULL);
    pthread_join(potok2, NULL);
    
    return 0;
}

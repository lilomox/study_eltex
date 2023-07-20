#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>

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

int main() {
    key_t key_msg, key_grp;
    int msgid_msg, msgid_grp;
    struct group grp;
    struct message msg;

    key_msg = ftok("client.c", 'A');
    key_grp = ftok("server.c", 'B');

    msgid_msg = msgget(key_msg, 0666 | IPC_CREAT);
    msgid_grp = msgget(key_grp, 0666 | IPC_CREAT);

    if (msgid_msg == -1 || msgid_grp == -1) {
        printf("Ошибка создания очереди сообщений\n");
        return 1;
    }

    grp.mtype = 1;
    grp.count = 0;

    while (1) {
        msgrcv(msgid_msg, &msg, sizeof(msg), 1, 0);
        
        strcpy(grp.marray[grp.count], msg.mtext);
        grp.count++;
        grp.mtype = NUM_MSG;

        msgsnd(msgid_grp, &grp, sizeof(grp), 0);
    }

    msgctl(msgid_msg, IPC_RMID, NULL);
    msgctl(msgid_grp, IPC_RMID, NULL);

    return 0;
}

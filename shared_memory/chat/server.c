#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sem.h>

#define ROWS 100
#define COLS 50
#define SIZE 9000

#define WOR "12plkopl21"


int main() {
    key_t key_msg, key_grp, key_count;
    int shmid_msg, shmid_grp, shmid_count;

    key_msg = ftok("server.c", 'A');
    key_grp = ftok("server.c", 'B');
    key_count = ftok("server.c", 'C');

    shmid_msg = shmget(key_msg, SIZE, 0666 | IPC_CREAT);
    shmid_grp = shmget(key_grp, SIZE, 0666 | IPC_CREAT);
    shmid_count = shmget(key_count, SIZE, 0666 | IPC_CREAT);

    if (shmid_msg == -1 || shmid_grp == -1 || shmid_count == -1) {
        perror("shmget");
        return 1;
    }
    char *text = (char *)shmat(shmid_msg, NULL, 0);
    strcpy(text, WOR);

    char (*arr)[ROWS];
    arr = shmat(shmid_grp, NULL, 0);
    int *count = (int *)shmat(shmid_count, NULL, 0);

    *count = 0;
    int co = 0;
    while (1) {
        while (strcmp(text, WOR) == 0) {
             usleep(10000);
        }
        printf("%s", text);
        strcpy(arr[co], text);
        
        co++;
        *count = co;
        strcpy(text, WOR);
    }

    shmdt(text);
    shmdt(arr);
    shmdt(count);

    return 0;
}

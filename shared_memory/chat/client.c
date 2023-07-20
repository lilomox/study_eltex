#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sem.h>
#include <pthread.h>

#define ROWS 100
#define COLS 50
#define SIZE 9000

#define WORD "12plkopl21"

void *prints() {
    key_t key_grp, key_count;

    int shmid_grp, shmid_count;

    key_grp = ftok("server.c", 'B');
    key_count = ftok("server.c", 'C');

    shmid_grp = shmget(key_grp, SIZE, 0666 | IPC_CREAT);
    shmid_count = shmget(key_count, SIZE, 0666 | IPC_CREAT);

    char (*arr)[ROWS];
    arr = shmat(shmid_grp, NULL, 0);
    int *count = (int *)shmat(shmid_count, NULL, 0);

    int k = *count;

    while (1) {
        while (*count == k) {
            usleep(10000);
        }
        k++;

        system("clear");
        for (int i = 0; i < k; i++) {
            printf("%s", arr[i]);
        }
        
    }
}

void *writs() {
    char name[ROWS];
    char tmp[ROWS];

    key_t key_msg = ftok("server.c", 'A');
    int shmid_msg = shmget(key_msg, SIZE, 0666 | IPC_CREAT);
    
    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);

    char *text = (char *)shmat(shmid_msg, NULL, 0);;

    strcpy(tmp, "New user - ");
    strcat(tmp, name);

    strcpy(text, tmp);

    name[strlen(name)-1] = ':';
    strcat(name, " ");

    char buf[ROWS];
    while (1) {
        char tp[ROWS];
        strcpy(buf, " ");
        fgets(buf, sizeof(buf), stdin);
        strcpy(tp, name);
        strcat(tp, buf);
        strcpy(text, tp);
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

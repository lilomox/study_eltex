#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 199
#define COUNT_OF_THREADS 15

#define PATH_TO_TCP "127.0.0.14"
#define PORT_TO_TCP 6666

#define PATH_TO_UDP "127.0.0.9"
#define PORT_TO_UDP 5555

int glob_tcp_fd = 0;
int glob_udp_fd = 0;
int glob_epoll = 0;

struct msgbuf {
  long mtype;
  int mtext;
};

struct newbuf {
  long mtype;
  struct sockaddr *mtext;
};

pthread_mutex_t mutexes[COUNT_OF_THREADS * 2];

void handle_sigint(int sig) {
  for (int i = 0; i < COUNT_OF_THREADS; i++) {
    pthread_mutex_destroy(&mutexes[i]);
  }
  close(glob_tcp_fd);
  close(glob_udp_fd);
  close(glob_epoll);
  printf("\n");
  exit(EXIT_SUCCESS);
}

void handle_sigtstp(int sig) {
  for (int i = 0; i < COUNT_OF_THREADS; i++) {
    pthread_mutex_destroy(&mutexes[i]);
  }
  close(glob_tcp_fd);
  close(glob_udp_fd);
  close(glob_epoll);
  printf("\n");
  exit(EXIT_SUCCESS);
}

void *process(void *arg) {
  int *tmp = (int *)arg;
  int i = *tmp;

  int msqid;
  key_t key;
  struct msgbuf buff;

  key = ftok(".", 'A');
  msqid = msgget(key, IPC_CREAT | 0666);
  if (msqid == -1) {
    perror("msgget");
    exit(1);
  }

  while (1) {
    if (pthread_mutex_trylock(&mutexes[i]) == 0) {
      if (msgrcv(msqid, &buff, sizeof(buff.mtext), 0, 0) == -1) {
        perror("msgrcv");
        exit(1);
      }

      int temp_fd = buff.mtext;
      if (send(temp_fd, "connection successful\n",
               strlen("connection successful\n"), 0) == -1) {
        perror("send");
        exit(1);
      }

      char buf[SIZE];
      if (recv(temp_fd, buf, sizeof(buf), 0) == -1) {
        perror("recv");
        exit(1);
      }

      printf("receive massage: %s", buf);

      pthread_mutex_unlock(&mutexes[i]);
    }
  }
  return NULL;
}

void *process2(void *arg) {
  int *tmp = (int *)arg;
  int i = *tmp;

  int msqid;
  key_t key;
  struct newbuf buff;

  key = ftok(".", 'B');
  msqid = msgget(key, IPC_CREAT | 0666);
  if (msqid == -1) {
    perror("msgget");
    exit(1);
  }

  while (1) {
    if (pthread_mutex_trylock(&mutexes[i]) == 0) {
      if (msgrcv(msqid, &buff, sizeof(buff.mtext), 0, 0) == -1) {
        perror("msgrcv");
        exit(1);
      }

      struct sockaddr_in *client = (struct sockaddr_in *)buff.mtext;
      socklen_t len_cln = sizeof(*client);

      if (sendto(glob_udp_fd, "connection successful\n",
                 strlen("connection successful\n"), 0,
                 (struct sockaddr *)client, len_cln) == -1) {
        perror("send");
        exit(1);
      }

      char buf[SIZE];
      if (recvfrom(glob_udp_fd, buf, sizeof(buf), 0, (struct sockaddr *)client,
                   &len_cln) == -1) {
        perror("recv");
        exit(1);
      }

      printf("receive massage: %s", buf);

      pthread_mutex_unlock(&mutexes[i]);
    }
  }
  return NULL;
}

int main() {
  if (signal(SIGINT, handle_sigint) == SIG_ERR) {
    perror("signal SIGINT");
    return 1;
  }

  if (signal(SIGTSTP, handle_sigtstp) == SIG_ERR) {
    perror("signal SIGTSTP");
    return 1;
  }

  int msqid1, msqid2;
  key_t key1, key2;
  struct msgbuf queue1;
  struct newbuf queue2;

  key1 = ftok(".", 'A');
  key2 = ftok(".", 'B');

  msqid1 = msgget(key1, IPC_CREAT | 0666);
  msqid2 = msgget(key2, IPC_CREAT | 0666);

  if (msqid1 == -1) {
    perror("msgget1");
    return 1;
  }
  if (msqid2 == -1) {
    perror("msgget2");
    return 1;
  }

  struct sockaddr_in tcp_serv, tcp_cln, udp_serv, udp_cln;
  int tcp_serv_fd, tcp_cln_fd, udp_serv_fd;
  socklen_t addrlen;

  addrlen = sizeof(tcp_cln);
  socklen_t len_cln = sizeof(udp_cln);

  tcp_serv_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (tcp_serv_fd == -1) {
    perror("socket - tcp");
    return 1;
  }
  glob_tcp_fd = tcp_serv_fd;
  tcp_serv.sin_family = AF_INET;
  tcp_serv.sin_port = htons(PORT_TO_TCP);
  inet_pton(AF_INET, PATH_TO_TCP, &(tcp_serv.sin_addr));

  udp_serv_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (udp_serv_fd == -1) {
    perror("socket - udp");
    return 1;
  }
  glob_udp_fd = udp_serv_fd;
  udp_serv.sin_family = AF_INET;
  udp_serv.sin_port = htons(PORT_TO_UDP);
  inet_pton(AF_INET, PATH_TO_UDP, &(udp_serv.sin_addr));

  if (bind(tcp_serv_fd, (struct sockaddr *)&tcp_serv, sizeof(tcp_serv)) == -1) {
    perror("bind to tcp");
    return 1;
  }
  if (bind(udp_serv_fd, (struct sockaddr *)&udp_serv, sizeof(udp_serv)) == -1) {
    perror("bind to udp");
    return 1;
  }

  if (listen(tcp_serv_fd, 5) == -1) {
    perror("listen");
    return 1;
  }

  struct epoll_event event, events;
  int epoll_fd, event_count;
  epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    perror("epoll_create1");
    return 1;
  }
  glob_epoll = epoll_fd;

  event.events = EPOLLIN;
  event.data.fd = tcp_serv_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_serv_fd, &event) < 0) {
    perror("epoll_ctl - tcp");
    return 1;
  }

  event.events = EPOLLIN;
  event.data.fd = udp_serv_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, udp_serv_fd, &event) < 0) {
    perror("epoll_ctl - udp");
    return 1;
  }

  for (int i = 0; i < COUNT_OF_THREADS * 2; i++) {
    pthread_mutex_init(&mutexes[i], NULL);
  }

  pthread_t treads[COUNT_OF_THREADS * 2];
  int arr[COUNT_OF_THREADS];
  for (int i = 0; i < COUNT_OF_THREADS; i++) {
    arr[i] = i;
    pthread_create(&treads[i], NULL, process, &arr[i]);
  }

  int arr2[COUNT_OF_THREADS];
  for (int i = COUNT_OF_THREADS; i < COUNT_OF_THREADS * 2; i++) {
    arr2[i] = i;
    pthread_create(&treads[i], NULL, process2, &arr2[i]);
  }

  while (1) {
    event_count = epoll_wait(epoll_fd, &events, COUNT_OF_THREADS, -1);
    if (event_count == -1) {
      perror("epoll_wait");
      return 1;
    }

    if (events.data.fd == tcp_serv_fd) {
      tcp_cln_fd = accept(tcp_serv_fd, (struct sockaddr *)&tcp_serv, &addrlen);
      if (tcp_cln_fd == -1) {
        perror("accept");
        return 1;
      }

      queue1.mtype = 1;
      queue1.mtext = tcp_cln_fd;
      if (msgsnd(msqid1, &queue1, sizeof(queue1.mtext), 0) == -1) {
        perror("msgsnd");
        return 1;
      }
    } else if (events.data.fd == udp_serv_fd) {
      char c;
      recvfrom(udp_serv_fd, &c, 1, 0, (struct sockaddr *)&udp_cln, &len_cln);

      queue2.mtype = 1;
      queue2.mtext = (struct sockaddr *)&udp_cln;
      if (msgsnd(msqid2, &queue2, sizeof(queue2.mtext), 0) == -1) {
        perror("msgsnd2");
        return 1;
      }
      sleep(1);
    }
  }

  for (int i = 0; i < COUNT_OF_THREADS; i++) {
    pthread_join(treads[i], NULL);
  }
}

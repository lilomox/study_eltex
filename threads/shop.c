#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COUNT_OF_SHOPS 4
#define TOTAL_BASKET 10000

pthread_mutex_t shop_mutexes[COUNT_OF_SHOPS];

int shops[COUNT_OF_SHOPS] = {1000, 1000, 1000, 1000};
int glob = 0;

void *shopper(void *arg) {
  int shopper_id = *(int *)arg;
  int basket = 0;

  while (basket < TOTAL_BASKET) {
    for (int i = 0; i < COUNT_OF_SHOPS; i++) {
      if (pthread_mutex_trylock(&shop_mutexes[i]) == 0) {
        if (shops[i] >= 500) {
          int tmp = (rand() % 500);
          shops[i] -= tmp;
          basket += tmp;
          printf("Shopper№%d take %d from Shop№%d. Basket of shopper = %d\n",
                 shopper_id + 1, tmp, i + 1, basket);
          sleep(1);
          pthread_mutex_unlock(&shop_mutexes[i]);
          break;
        } else {
          pthread_mutex_unlock(&shop_mutexes[i]);
        }
      }
    }
  }
  glob++;
  return NULL;
}

void *delivery() {
  srand(time(NULL));

  while (glob != 3) {
    for (int i = 0; i < COUNT_OF_SHOPS; i++) {
      if (pthread_mutex_trylock(&shop_mutexes[i]) == 0) {
        shops[i] += (rand() % 1000);

        printf("Delivery made to Shop№%d. Shop№%d = %d\n", i + 1, i + 1,
               shops[i]);
        sleep(1);
        pthread_mutex_unlock(&shop_mutexes[i]);
        break;
      }
    }
  }
  return NULL;
}

int main() {
  pthread_t shoppers[COUNT_OF_SHOPS - 1];
  pthread_t delivery_thread;
  int shopper_ids[COUNT_OF_SHOPS - 1] = {0, 1, 2};

  for (int i = 0; i < COUNT_OF_SHOPS; i++) {
    pthread_mutex_init(&shop_mutexes[i], NULL);
  }

  for (int i = 0; i < COUNT_OF_SHOPS - 1; i++) {
    pthread_create(&shoppers[i], NULL, shopper, &shopper_ids[i]);
  }
  pthread_create(&delivery_thread, NULL, delivery, NULL);

  for (int i = 0; i < COUNT_OF_SHOPS - 1; i++) {
    pthread_join(shoppers[i], NULL);
  }
  pthread_join(delivery_thread, NULL);

  for (int i = 0; i < COUNT_OF_SHOPS; i++) {
    pthread_mutex_destroy(&shop_mutexes[i]);
  }

  return 0;
}

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COUNT_OF_SHOPS 4
#define TOTAL_BASKET 5000

int shops[COUNT_OF_SHOPS] = {1000, 1000, 1000, 1000};

pthread_mutex_t shop_mutexes[COUNT_OF_SHOPS];
pthread_mutex_t delivery_mutex = PTHREAD_MUTEX_INITIALIZER;

int delivery_shop = -1;
int glob = 0;

void *shopper(void *arg) {
  int shopper_id = *(int *)arg;
  int basket = 0;

  while (basket < TOTAL_BASKET) {
    for (int i = 0; i < COUNT_OF_SHOPS; i++) {
      pthread_mutex_lock(&shop_mutexes[i]);
      if (shops[i] >= 500) {
        int tmp = (rand() % 500);
        shops[i] -= tmp;
        basket += tmp;
        printf("Shopper№%d take %d from Shop№%d. Basket of shopper = %d\n",
               shopper_id + 1, tmp, i + 1, basket);
        pthread_mutex_unlock(&shop_mutexes[i]);
        sleep(2);
        break;
      }

      pthread_mutex_unlock(&shop_mutexes[i]);
    }
  }
  glob++;
  return NULL;
}

void *delivery() {
  srand(time(NULL));

  while (glob != 3) {
    pthread_mutex_lock(&delivery_mutex);
    delivery_shop = (delivery_shop + 1) % COUNT_OF_SHOPS;

    shops[delivery_shop] += (rand() % 1000);

    printf("Delivery made to Shop№%d. Shop№%d = %d\n", delivery_shop + 1,
           delivery_shop + 1, shops[delivery_shop]);
    pthread_mutex_unlock(&delivery_mutex);

    sleep(2);
  }
  return NULL;
}

int main() {
  pthread_t shoppers[COUNT_OF_SHOPS];
  pthread_t delivery_thread;
  int shopper_ids[COUNT_OF_SHOPS] = {0, 1, 2};

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

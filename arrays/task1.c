#include <stdio.h>

#define N 4

int main() {
  int arr[N][N] = {
      {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%3d ", arr[i][j]);
    }
    printf("\n");
  }
}

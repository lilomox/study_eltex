#include <stdio.h>

#define N 3

int main()
{
	int arr[N][N] = {{1325, 12, 13},
			 {21, 341, 3},
			 {14, 34, 34}};
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N-i; j++) {
			arr[i][j] = 1;
		}
		for (int m = N-i; m < N; m++) {
			arr[i][m] = 0;
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}
}

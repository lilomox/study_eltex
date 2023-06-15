#include <stdio.h>

#define N 4

int main()
{
	int arr[N][N] = {0};
	int count = 1;
	int n = 0;
	int k = N;
	while (count <= N*N) {
		for (int j = n; j < k; j++) {
			arr[n][j] = count;
			count++;
		}

		for (int j = n+1; j < k; j++) {
                        arr[j][k-1] = count;
                        count++;
              	}

		for (int j = k-2; j >= n; j--) {
			arr[k-1][j] = count;
			count++;
		}

		for (int j = k-2; j >= n+1; j--) {
			arr[j][n] = count;
			count++;
		}

		n++;
		k--;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%3d ", arr[i][j]);
		}
		printf("\n");
	}
}

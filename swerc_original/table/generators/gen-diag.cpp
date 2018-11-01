#include <cstdio>

int N = 2000;
int D = 100000;

int main() {
  printf("%d %d %d %d\n", N, N, 2*N, D);
  for (int i = 0; i < N; ++i) {
    printf("%d %d %d %d\n", i, i + 1, i, i + 1);
    printf("%d %d %d %d\n", i, i + 1, N - i - 1, N - i);
  }
  int d = 0;
  for (int i = 1; i <= N; i+=3) {
    for (int j = 1; j <= i; j+=3) {
      printf("%d %d\n", i, j);
      if (++d == D) {
        return 0;
      }
      if(i != j) {
        printf("%d %d\n", j, i);
        if (++d == D) {
          return 0;
        }
      }
    }
  }
  return 1;
}

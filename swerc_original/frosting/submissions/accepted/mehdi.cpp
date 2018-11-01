#include <cstdio>

long long A[3], S[3];

int main() {

  int N;
  scanf("%d", &N);

  for (int i = 0; i < N; i++) {
    int a;
    scanf("%d", &a);
    A[i%3] += a;
  }
  for (int i = 1; i <= N; i++) {
    int b;
    scanf("%d", &b);
    for (int j = 0; j < 3; j++)
      S[(i+j)%3] += A[j] * b;
  }
  printf("%Ld %Ld %Ld\n", S[2], S[0], S[1]);
  return 0;
}

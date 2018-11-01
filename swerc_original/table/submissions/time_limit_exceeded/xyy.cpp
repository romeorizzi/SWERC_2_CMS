// Naive solution in O(X*Y²)

#include<cstdio>
#include<vector>
#include<algorithm>

using namespace std;

const int SZ = 2000;

bool table[SZ][SZ];
int counts[SZ+1][SZ+1];

int main() {
  int X, Y, N, D;
  scanf("%d%d%d%d", &X, &Y, &N, &D);

  for(int i = 0; i < N; i++) {
    int x1, x2, y1, y2;
    scanf("%d%d%d%d", &x1, &x2, &y1, &y2);
    for(int x = x1; x < x2; x++)
      for(int y = y1; y < y2; y++)
        table[x][y] = true;
  }

  vector<int> hist(Y, 0);
  for(int x = 0; x < X; x++) {
    for(int y = 0; y < Y; y++)
      hist[y] = table[x][y] ? 0 : hist[y]+1;

    for(int y1 = 0; y1 < Y; y1++) {
      int m = 1000000;
      for(int y2 = y1; m && y2 < Y; y2++) {
        m = min(m, hist[y2]);
        counts[m][y2-y1+1]++;
      }
    }
  }

  for(int x = X-1; x >= 1; x--)
    for(int y = Y; y >= 1; y--)
      counts[x][y] += counts[x+1][y];

  for(int i = 0; i < D; i++) {
    int x, y;
    scanf("%d%d", &x, &y);
    printf("%d\n", counts[x][y]);
  }

  return 0;
}

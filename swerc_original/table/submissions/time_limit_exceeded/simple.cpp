// Naive solution in O(X*Y*D)

#include <cstdio>
#include <vector>

using ::std::pair;
using ::std::vector;

int main() {
  int X, Y, N, D;
  scanf("%d%d%d%d", &X, &Y, &N, &D);
  vector<vector<int>> T(X, vector<int>(Y, 0));
  for (int _n = 0; _n < N; ++_n) {
    int x1, x2, y1, y2;
    scanf("%d%d%d%d", &x1, &x2, &y1, &y2);
    T[x1][y1] += 1;
    if (y2 < Y) {
      T[x1][y2] -= 1;
    }
    if (x2 < X) {
      T[x2][y1] -= 1;
    }
    if (x2 < X && y2 < Y) {
      T[x2][y2] += 1;
    }
  }
  for (int _a = 0; _a < 2; ++_a) {
    for (int i = 0; i < X; ++i) {
      for (int j = 0; j < Y; ++j) {
        T[i][j] += (i > 0 ? T[i - 1][j] : 0) + (j > 0 ? T[i][j - 1] : 0) -
                   (i > 0 && j > 0 ? T[i - 1][j - 1] : 0);
      }
    }
  }
  vector<pair<int, int>> dish(D);
  for (int d = 0; d < D; ++d) {
    scanf("%d%d", &dish[d].first, &dish[d].second);
  }
  vector<int> answer(D, 0);
  for (int i = 0; i < X; ++i) {
    for (int j = 0; j < Y; ++j) {
      for (int d = 0; d < D; ++d) {
        if (i + dish[d].first - 1 < X && j + dish[d].second - 1 < Y &&
            (i > 0 && j > 0 ? T[i - 1][j - 1] : 0) -
                    (j > 0 ? T[i + dish[d].first - 1][j - 1] : 0) -
                    (i > 0 ? T[i - 1][j + dish[d].second - 1] : 0) +
                    T[i + dish[d].first - 1][j + dish[d].second - 1] ==
                0) {
          ++answer[d];
        }
      }
    }
  }
  for (int d = 0; d < D; ++d) {
    printf("%d\n", answer[d]);
  }
  return 0;
}

#include <cstdio>
#include <vector>

using ::std::vector;

int main() {
  int X, Y, N, D;
  scanf("%d%d%d%d", &X, &Y, &N, &D);
  vector<vector<bool>> table(X, vector<bool>(Y, false));
  for (int n = 0; n < N; ++n) {
    int a, b, c, d;
    scanf("%d%d%d%d", &a, &b, &c, &d);
    for (int i = a; i < b; ++i) {
      for (int j = c; j < d; ++j) {
        table[i][j] = true;
      }
    }
  }
  vector<int> hist(Y + 1, 0);
  vector<int> biggest(std::max(X, Y) + 1, 0);
  for (int i = 0; i < X; ++i) {
    for (int j = 0; j < Y; ++j) {
      if (table[i][j]) {
        hist[j] = 0;
      } else {
        ++hist[j];
      }
    }
    vector<int> stack;
    for (int j = 0; j <= Y; ++j) {
      while (!stack.empty() && hist[j] < hist[stack.back()]) {
        int x = hist[stack.back()];
        stack.pop_back();
        int y = j;
        if (!stack.empty()) {
          y -= stack.back() + 1;
        }
        if (x > y) {
          int tmp = x;
          x = y;
          y = tmp;
        }
        if (x > biggest[y]) {
          biggest[y] = x;
        }
      }
      if (stack.empty() || hist[j] >= hist[stack.back()]) {
        stack.push_back(j);
      }
    }
  }
  for (int i = biggest.size() - 2; i >= 0; --i) {
    if (biggest[i] < biggest[i + 1]) {
      biggest[i] = biggest[i + 1];
    }
  }
  for (int d = 0; d < D; ++d) {
    int x, y;
    scanf("%d%d", &x, &y);
    if (x > y) {
      int tmp = x;
      x = y;
      y = tmp;
    }
    if (x <= biggest[y]) {
      printf("1\n");
    } else {
      printf("0\n");
    }
  }
  return 0;
}

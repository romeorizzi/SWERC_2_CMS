#include <cstdio>
#include <vector>

using ::std::vector;

typedef vector<vector<long long>> mat;

const long long mod = 1000000000;

mat mul(const mat& a, const mat& b) {
  mat c = mat(a.size(), vector<long long>(b[0].size(), 0));
  for (unsigned int i = 0; i < a.size(); ++i) {
    for (unsigned int j = 0; j < b[0].size(); ++j) {
      for (unsigned int k = 0; k < b.size(); ++k) {
        c[i][j] += a[i][k] * b[k][j];
        c[i][j] %= mod;
      }
    }
  }
  return c;
}

mat exp(const mat& a, long long n) {
  if (n == 1ll) {
    return a;
  } else {
    mat b = exp(a, n / 2ll);
    mat c = mul(b, b);
    if (n % 2ll == 0) {
      return c;
    } else {
      return mul(c, a);
    }
  }
}

long long transitions(const mat& mem, int i, int j) {
  if (j == 0) {
    return 1;
  }
  if ((j & 1) == 0) {
    return mem[i >> 1][j >> 1];
  }
  if ((j & 3) == 1) {
    if ((i & 1) == 0) {
      return mem[i >> 2][j >> 2];
    } else {
      return 0;
    }
  }
  if ((i & 1) == 0) {
    return mem[i >> 2][j >> 2] + mem[i >> 1][j >> 1];
  } else {
    return mem[i >> 2][j >> 2];
  }
}

int main() {
  int N;
  long long M;
  scanf("%d%lld", &N, &M);
  if (N == 0 || M == 0) {
    printf("0\n");
  } else {
    int S = 1 << N;
    mat T = mat(S, vector<long long>(S));
    for (int i = 0; i < S; ++i) {
      for (int j = 0; j < S; ++j) {
        T[i][j] = transitions(T, i, j);
      }
    }
    mat TT = exp(T, M);
    long long ans = 0;
    for (int i = 0; i < S; ++i) {
      ans += TT[S - 1][i];
    }
    printf("%lld\n", ans % 1000000000);
  }
  return 0;
}

// Author: Raphael

#include <assert.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const int MIN_TIMESTAMP = 0;
const int MAX_TIMESTAMP = 1000 * 1000 * 1000;
const int MAX_K = 10000;
const int MIN_K = 1;

using int64 = long long;
const int64 kint64max = std::numeric_limits<int64>::max();

// Naive solution in K^3.
int GetShiftK3(const std::vector<int>& S1, const std::vector<int>& S2) {
  int max_coincidences = 0;
  int best_shift = 0;
  for (int i = 0; i < S1.size(); ++i) {
    for (int j = 0; j < S2.size(); ++j) {
      int m = S2[j] - S1[i];
      if (m < 0) {
        continue;
      }

      int coincidences = 0;
      // could also do ii=i, jj=j.
      for (int ii = 0, jj = 0; ii < S1.size() && jj < S2.size();) {
        int a = S1[ii];
        int b = S2[jj] - m;
        if (a == b) {
          ++coincidences;
          ++ii;
          ++jj;
        } else if (a > b) {
          ++jj;
        } else {
          assert(a < b);
          ++ii;
        }
      }
      if (coincidences > max_coincidences ||
          (coincidences == max_coincidences && m < best_shift)) {
        max_coincidences = coincidences;
        best_shift = m;
      }
    }
  }
  return best_shift;
}

int main() {
  std::vector<int> S1, S2;
  int K, L;
  std::cin >> K >> L;
  for (int i = 0; i < K; ++i) {
    int e;
    std::cin >> e;
    S1.push_back(e);
  }
  for (int i = 0; i < L; ++i) {
    int e;
    std::cin >> e;
    S2.push_back(e);
  }
  std::cout << GetShiftK3(S1, S2) << std::endl;
}

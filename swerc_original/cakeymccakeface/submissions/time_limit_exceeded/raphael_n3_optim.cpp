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

// Similar to GetShiftK3(), except that we check each candidate shift
// only once. There is max K^2 of them, but in practice their number
// might be much smaller. We do the O(K) merge sort only once for each
// unique value.
// This might cost O(K^2) of memory.
int GetShiftK3_SetOptim(const std::vector<int>& S1,
                        const std::vector<int>& S2) {
  std::unordered_set<int> candidate_shifts;
  for (int i = 0; i < S1.size(); ++i) {
    for (int j = 0; j < S2.size(); ++j) {
      int m = S2[j] - S1[i];
      if (m < 0) {
        continue;
      }
      candidate_shifts.insert(m);
    }
  }
  std::vector<int> candidate_shifts_v(candidate_shifts.begin(),
                                      candidate_shifts.end());
  std::sort(candidate_shifts_v.begin(), candidate_shifts_v.end());
  int max_coincidences = 0;
  int best_shift = 0;
  for (const int m : candidate_shifts_v) {
    int coincidences = 0;
    for (int i = 0, j = 0; i < S1.size() && j < S2.size();) {
      int a = S1[i];
      int b = S2[j] - m;
      if (a == b) {
        ++coincidences;
        ++i;
        ++j;
      } else if (a > b) {
        ++j;
      } else {
        assert(a < b);
        ++i;
      }
    }
    if (coincidences > max_coincidences) {
      max_coincidences = coincidences;
      best_shift = m;
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
  std::cout << GetShiftK3_SetOptim(S1, S2) << std::endl;
}

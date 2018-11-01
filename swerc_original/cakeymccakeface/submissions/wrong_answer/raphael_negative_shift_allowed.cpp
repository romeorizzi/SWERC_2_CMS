// O(N*M) solution.

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

int solve(const std::vector<int>& S1, const std::vector<int>& S2) {
  std::unordered_map<int, int> shift_to_num_coincidences;
  for (int front : S1) {
    for (int back : S2) {
      int m = back - front;
      // Bug here: We don't filter out negative shifts.
      //      if (m < 0) {
      //        continue;
      //      }
      ++shift_to_num_coincidences[m];
    }
  }
  int max_coincidences = 0;
  int best_shift = 0;
  for (const auto p : shift_to_num_coincidences) {
    if (p.second > max_coincidences ||
        p.second == max_coincidences && p.first < best_shift) {
      best_shift = p.first;
      max_coincidences = p.second;
    }
  }
  return best_shift;
}


int main() {
  int N,M;
  std::cin >> N >> M;
  std::vector<int> S1,S2;
  for (int i = 0; i < N; ++i) {
    int v;
    std::cin >> v;
    S1.push_back(v);
  }
  for (int i = 0; i < M; ++i) {
    int v;
    std::cin >> v;
    S2.push_back(v);
  }
  std::cout << solve(S1, S2) << std::endl;
}

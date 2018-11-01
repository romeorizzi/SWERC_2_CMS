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

const bool DEBUG = false;

using int64 = long long;
const int64 kint64max = std::numeric_limits<int64>::max();

// Returns:
// Min(m' | (e - m' is in S) and (m' >= m)).
// If no such value exists, kint64max is returned.
int64 LeftGap(const std::vector<int>& S, int e, int64 m) {
  assert(S.size() >= 1);
  assert(e >= MIN_TIMESTAMP);
  assert(e <= MAX_TIMESTAMP);
  assert(m <= MAX_TIMESTAMP);
  assert(m >= MIN_TIMESTAMP);
  std::vector<int>::const_iterator it =
      std::lower_bound(S.begin(), S.end(), e - m);
  //  [2 5], e=5,
  //     ^
  //  [2 5], e=4,
  //     ^
  if (it != S.end() && *it == e - m) {
    return m;
  }
  if (it == S.begin()) {
    return kint64max;
  }
  --it;
  assert(e >= *it);
  return e - *it;
}

// Represents an element in S2, for a given (implicit) time-shift
// between S1 and S2.
struct Element {
  int e;  // Element of S2.
  // This is the shift that gets us to the next coincidence between
  // the current element in S2 with the "next" element in S1.
  // More formally, this is:
  // Min{s | s >= current_implicit_shift and (e - m is in S1)}
  // If the set is empty we set this to kint64max.
  int64 next_coincidence_shift;
};

struct ElementHeapCpm {
  bool operator()(const Element& left, const Element& right) const {
    return std::tie(left.next_coincidence_shift, left.e) >
           std::tie(right.next_coincidence_shift, right.e);
  }
};

// Solves the problem in O(K^2 Log(K)) where K is the size of the
// sets.
int GetShiftK2LogK(const std::vector<int>& S1, const std::vector<int>& S2) {
  // We only store elements where next_coincidence_shift != kint64max.
  std::vector<Element> S2_elements;
  for (int e : S2) {
    S2_elements.push_back({e, LeftGap(S1, e, 0)});
    if (DEBUG) {
      std::cout << "e=" << e
                << " next shift: " << S2_elements.back().next_coincidence_shift
                << std::endl;
    }
  }
  //  std::cout << std::endl;
  std::make_heap(S2_elements.begin(), S2_elements.end(), ElementHeapCpm());
  int max_coincidences = 0;
  int64 best_shift = 0;
  std::vector<Element> popped;
  // Only for finding interesting inputs (not used to solve the problem).
  int count_shifts_with_max_coincidences = 0;
  int64 total_sum_coincidences = 0;
  while (!S2_elements.empty() &&
         S2_elements.front().next_coincidence_shift != kint64max) {
    const int64 cur_shift = S2_elements.front().next_coincidence_shift;
    if (DEBUG) {
      std::cout << "== shift: " << cur_shift << std::endl;
    }
    popped.clear();
    // Check number of coincidences.
    int coincidences = 0;
    // Remove from heap those that are coincident.
    while (!S2_elements.empty() &&
           S2_elements.front().next_coincidence_shift == cur_shift) {
      std::pop_heap(S2_elements.begin(), S2_elements.end(), ElementHeapCpm());
      Element cur = S2_elements.back();
      S2_elements.pop_back();
      assert(cur.next_coincidence_shift == cur_shift);
      ++coincidences;
      if (DEBUG) {
        std::cout << "coincidence: " << cur.e << std::endl;
      }
      popped.push_back(cur);
    }
    total_sum_coincidences += coincidences;
    if (coincidences > max_coincidences) {
      max_coincidences = coincidences;
      best_shift = cur_shift;
      count_shifts_with_max_coincidences = 1;
    } else if (coincidences == max_coincidences) {
      count_shifts_with_max_coincidences++;
    }
    if (S2_elements.empty()) {
      break;
    }
    assert(!S2_elements.empty());
    // Insert back ones that were popped out, looking for the next
    // coincidence after the current ones.
    for (Element cur : popped) {
      cur.next_coincidence_shift = LeftGap(S1, cur.e, cur_shift + 1);
      if (cur.next_coincidence_shift == kint64max) {
        // This element will never coincide again, we can get forget it.
        continue;
      }
      S2_elements.push_back(cur);
      if (DEBUG) {
        std::cout << "New: e=" << cur.e
                  << " next_coincidence_shift=" << cur.next_coincidence_shift
                  << std::endl;
      }
      std::push_heap(S2_elements.begin(), S2_elements.end(), ElementHeapCpm());
    }
  }
  assert(max_coincidences >= 0);
  if (DEBUG) {
    std::cout << "= max_coincidences: " << max_coincidences
	      << " count_shifts_with_max_coincidences: "
	      << count_shifts_with_max_coincidences
	      << " total_sum_coincidences: " << total_sum_coincidences
	      << std::endl;
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
  std::cout << GetShiftK2LogK(S1, S2) << std::endl;
}

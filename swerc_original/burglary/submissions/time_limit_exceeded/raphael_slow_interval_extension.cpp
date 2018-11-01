// Solution similar to the accepted solution with complexity O(N * max_ladders ^
// 4).
//
// However, the interval extension procedure is in O(M) instead of
// O(1). The complexity of this code is therefore O(N * M *
// max_ladders^4), and should therefore time out.

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

const int UNSET = -1;
const int INVALID = -2;
const int MAXN = 1000 + 1;  // +1 for the floor.
const int MAXM = 5000;
const int MAXLADDERS = 10;

int M;
int jars[MAXN][MAXM];               // 0 if no jar, >0 if jar.
int partial_sums_jars[MAXN][MAXM];  // [r][i] -> sum(jars[r][0..i]) (inclusive).
// For each row, positions of the ladders that go up.
std::vector<int> up_ladder_pos[MAXN];
int dyna[MAXN][MAXLADDERS][MAXLADDERS];

// Represents interval [i,j].
// Empty if i > j.
struct Interval {
  Interval() : i(-1), j(-1) {}
  Interval(int i, int j) : i(i), j(j) {}
  static Interval FromUnorderedEndpoints(int i, int j) {
    if (i > j) std::swap(i, j);
    return Interval(i, j);
  }
  Interval Intersect(const Interval& other) {
    Interval res(std::max(i, other.i), std::min(j, other.j));
    return res;
  }
  bool IsEmpty() const { return i > j; }
  int i, j;
};

int get_score(int r, const Interval& interval) {
  if (interval.IsEmpty()) {
    return 0;
  }
  if (interval.i == 0) return partial_sums_jars[r][interval.j];
  return partial_sums_jars[r][interval.j] -
         partial_sums_jars[r][interval.i - 1];
}

// Max score from row r (where we arrive with ladders input_i <= input_j) to the
// top.
int solve(int r, int input_i_index, int input_j_index, int input_i,
          int input_j) {
  if (r == 0) {
    return 0;
  }
  int& res = dyna[r][input_i_index][input_j_index];
  if (res != UNSET) return res;
  res = INVALID;

  const std::vector<int>& up_ladders = up_ladder_pos[r];

  for (int output_i_index = 0; output_i_index < up_ladders.size();
       ++output_i_index) {
    const int output_i = up_ladders[output_i_index];
    for (int output_j_index = output_i_index;
         output_j_index < up_ladders.size(); ++output_j_index) {
      const int output_j = up_ladders[output_j_index];
      Interval left = Interval::FromUnorderedEndpoints(input_i, output_i);
      Interval right = Interval::FromUnorderedEndpoints(input_j, output_j);
      Interval inter = left.Intersect(right);

      // Check compatibility of inputs/output. inter is the interval
      // we are forced to travel twice. Hence it should not contain
      // any jar.
      if (get_score(r, inter) != 0) {
        continue;
      }

      // Compute score of jars we can steal at this row r.
      // Extend intervals.
      while (left.i > 0 && jars[r][left.i] == 0) --left.i;
      while (left.j + 1 < M && jars[r][left.j] == 0 && left.j + 1 < right.i)
        ++left.j;
      while (right.i > 0 && jars[r][right.i] == 0 && right.i - 1 > left.j)
        --right.i;
      while (right.j + 1 < M && jars[r][right.j] == 0) ++right.j;
      int up_score =
          solve(r - 1, output_i_index, output_j_index, output_i, output_j);
      if (up_score == INVALID) {
        // Require that we reach the top.
        continue;
      }
      int cur_score = get_score(r, left) + get_score(r, right) + up_score;
      res = std::max(res, cur_score);
    }
  }
  return res;
}

// Max score where we start at row r to the top, without visiting rows
// below r.
int solve_no_input_ladders(int r) {
  if (r == 0) return 0;
  const std::vector<int>& up_ladders = up_ladder_pos[r];
  int best = 0;
  for (int output_i_index = 0; output_i_index < up_ladders.size();
       ++output_i_index) {
    const int output_i = up_ladders[output_i_index];
    for (int output_j_index = output_i_index;
         output_j_index < up_ladders.size(); ++output_j_index) {
      const int output_j = up_ladders[output_j_index];
      Interval center(output_i, output_j);
      while (center.i > 0 && jars[r][center.i] == 0) --center.i;
      while (center.j + 1 < M && jars[r][center.j] == 0) ++center.j;
      int up_score =
          solve(r - 1, output_i_index, output_j_index, output_i, output_j);
      if (up_score == INVALID) {
        continue;
      }
      int cur_score = get_score(r, center) + up_score;
      best = std::max(best, cur_score);
    }
  }
  return best;
}

int main() {
  std::fill(**dyna, **dyna + MAXN * MAXLADDERS * MAXLADDERS, UNSET);
  std::fill(*jars, *jars + MAXN * MAXM, 0);
  int N;
  std::cin >> N;
  std::cin >> M;
  std::cin.ignore();
  for (int r = 0; r < N; ++r) {
    std::string shelf;
    std::getline(std::cin, shelf);
    for (int i = 0; i < shelf.size(); ++i) {
      jars[r][i] = shelf[i] == '-' ? 0 : shelf[i] - '0';
    }
    partial_sum(&jars[r][0], &jars[r][M], &partial_sums_jars[r][0]);
    std::string ladders;
    std::getline(std::cin, ladders);
    // '|': ladder.
    for (int i = 0; i < ladders.size(); ++i) {
      if (ladders[i] == '|') up_ladder_pos[r + 1].push_back(i);
    }
    assert(up_ladder_pos[r + 1].size() <= MAXLADDERS);
  }
  int best = 0;
  for (int r = 0;
       r < N + 1 /*+1 because we can walk on the floor below the bottom shelf*/;
       ++r) {
    best = std::max(best, solve_no_input_ladders(r));
  }
  std::cout << best << std::endl;
}

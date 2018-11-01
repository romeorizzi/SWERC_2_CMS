#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <limits>
#include <vector>

struct Coords {
  int x;
  int y;
  Coords(int _x, int _y) : x(_x), y(_y) {}
  unsigned distance(Coords const &other) const {
    return abs(x-other.x)+abs(y-other.y);
  }
  friend std::istream& operator>>(std::istream& in, Coords& coords) {
    return in >> coords.x >> coords.y;
  }
};

typedef enum { UNMARKED, STARRED, PRIMED } mark_t;
typedef std::vector<std::pair<size_t, size_t>> path_t;

class Problem {
  size_t const njobs;
  std::vector<std::vector<unsigned>> costs; // costs[courier][bottle] == costs[row][col]
  std::vector<bool> row_covered;
  std::vector<bool> col_covered;
  std::vector<std::vector<mark_t>> marked;

  path_t step1() {
    // Find the smallest element in each row and remove
    // it from the row.
    for (size_t row = 0; row < njobs; row++) {
      unsigned min = *std::min_element(costs[row].cbegin(), costs[row].cend());
      for (auto &cost: costs[row])
        cost -= min;
    }
    return step2();
  }

  path_t step2() {
    // Star zeroes with max one zero in every column and line
    std::vector<bool> covered_cols(njobs, false);
    for (size_t row = 0; row < njobs; row++)
      for (size_t col = 0; col < njobs; col++)
        if (!covered_cols[col] && costs[row][col] == 0.0f) {
          marked[row][col] = STARRED;
          covered_cols[col] = true;
          break;
        }
    return step3();
  }

  path_t step3() {
    // Count zeroes in uncovered columns and mark them covered
    // if they contain one. Go to step 7 if the problem is solved,
    // to step 4 otherwise.
    size_t total = 0;
    for (size_t row = 0; row < njobs; row++)
      for (size_t col = 0; col < njobs; col++)
        if (!col_covered[col] && marked[row][col] == STARRED) {
          col_covered[col] = true;
          total++;
        }
    return total >= njobs ? step7() : step4();
  }

  bool find_uncovered_zero(size_t &row, size_t &col) {
    size_t row_init = row;
    size_t col_init = col;
    for (; row < njobs; row++)
      if (!row_covered[row])
        for (col = row == row_init ? col_init : 0; col < njobs; col++)
          if (!col_covered[col] && costs[row][col] == 0.0f)
            return true;
    return false;
  }

  path_t step4() {
    // Prime an uncovered zero. If there are no starred zero in
    // its row, go to step 5. Otherwise, cover the row and uncover
    // the column of the starred zero, and go on until we cannot
    // uncover any zero. Save the position of the smallest uncovered
    // value and go to step 6.
    size_t row = 0;
    size_t col = 0;
    while (find_uncovered_zero(row, col)) {
      marked[row][col] = PRIMED;
      auto star_col = std::find(marked[row].cbegin(), marked[row].cend(), STARRED);
      if (star_col == marked[row].cend())
        return step5(row, col);
      col = std::distance(marked[row].cbegin(), star_col);
      row_covered[row] = true;
      col_covered[col] = false;
    }
    return step6();
  }

  path_t step5(size_t row, size_t col) {
    // Construct a serie of alternating prime and starred zeroes.
    path_t path;
    path.push_back(std::make_pair(row, col));
    for (;;) {
      bool done = true;
      for (row = 0; row < njobs; row++)
        if (marked[row][col] == STARRED) {
          path.push_back(std::make_pair(row, col));
          done = false;
          break;
        }
      if (done)
        break;
      for (col = 0; col < njobs; col++)
        if (marked[row][col] == PRIMED) {
          path.push_back(std::make_pair(row, col));
          break;
        }
    }
    // Invert star mark along the path
    for (auto coords: path)
      marked[coords.first][coords.second] =
        marked[coords.first][coords.second] == STARRED ? UNMARKED : STARRED;
    // Clear coverage
    for (size_t i = 0; i < njobs; i++)
      col_covered[i] = row_covered[i] = false;
    // Unmark primed values
    for (row = 0; row < njobs; row++)
      for (col = 0; col < njobs; col++)
        if (marked[row][col] == PRIMED)
          marked[row][col] = UNMARKED;
    return step3();
  }

  path_t step6() {
    // Find the smallest uncovered value
    unsigned min = std::numeric_limits<unsigned>::max();
    for (size_t row = 0; row < njobs; row++)
      if (!row_covered[row])
        for (size_t col = 0; col < njobs; col++)
          if (!col_covered[col] && costs[row][col] < min)
            min = costs[row][col];
    // Add it to every covered row and remove it from every
    // uncovered column.
    for (size_t row = 0; row < njobs; row++)
      for (size_t col = 0; col < njobs; col++) {
        if (row_covered[row])
          costs[row][col] += min;
        if (!col_covered[col])
          costs[row][col] -= min;
      }
    return step4();
  }

  path_t step7() {
    path_t result;
    for (size_t row = 0; row < njobs; row++)
      for (size_t col = 0; col < njobs; col++)
        if (marked[row][col] == STARRED)
          result.push_back(std::make_pair(row, col));
    return result;
  }

  public:
  Problem(std::vector<Coords> const &bottles, std::vector<Coords> &couriers, Coords const& restaurant)
    : njobs(bottles.size() + couriers.size() - 1)
      , row_covered(njobs, false)
      , col_covered(njobs, false)
      , marked(njobs, std::vector<mark_t>(njobs, UNMARKED))
  {
    size_t const nbottles = bottles.size();
    // Add phantom couriers based at the restaurant position
    couriers.reserve(njobs);
    for (size_t c = couriers.size(); c < njobs; c++)
      couriers.push_back(restaurant);
    // Build cost matrix
    for (auto courier: couriers) {
      std::vector<unsigned> row;
      row.reserve(njobs);
      for (auto bottle: bottles) {
        unsigned cost = courier.distance(bottle) + bottle.distance(restaurant);
        row.push_back(cost);
      }
      for (size_t b = nbottles; b < njobs; b++)
        row.push_back(0.0f);
      costs.push_back(row);
    }
  }

  unsigned solve() {
    // Make a copy of costs because the algorithm will modify the
    // matrix in place.
    std::vector<std::vector<unsigned>> saved_costs(costs);

    unsigned total = 0;
    path_t const assignments = step1();
    for (auto assignment: assignments) {
      unsigned cost = saved_costs[assignment.first][assignment.second];
      total += cost;
    }
    return total;
  }

};

int main() {
  size_t M;
  size_t N;
  std::cin >> N >> M;
  std::vector<Coords> bottles;
  for (size_t n = 0; n < N; n++) {
    bottles.push_back(Coords(0.0f, 0.0f));
    std::cin >> bottles.back();
  }
  std::vector<Coords> couriers;
  for (size_t m = 0; m < M; m++) {
    couriers.push_back(Coords(0.0f, 0.0f));
    std::cin >> couriers.back();
  }
  Coords restaurant(0.0f, 0.0f);
  std::cin >> restaurant;
  Problem problem = Problem(bottles, couriers, restaurant);
  std::cout << problem.solve() << std::endl;
}

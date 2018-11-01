#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <limits>
#include <numeric>
#include <utility>
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

class Problem {
  size_t const nx, ny;
  std::vector<std::vector<int>> costs; // costs[courier][bottle] == costs[row][col]

  public:
  Problem(std::vector<Coords> const &bottles, std::vector<Coords> &couriers, Coords const& restaurant)
    : nx(bottles.size()), ny(bottles.size() + couriers.size() - 1)
  {
    // Add phantom couriers based at the restaurant position
    couriers.reserve(ny);
    for (size_t c = couriers.size(); c < ny; c++)
      couriers.push_back(restaurant);
    // Build cost matrix with negative costs (to find the minimum one)
    for (auto bottle: bottles) {
      std::vector<int> row;
      row.reserve(ny);
      for (auto courier: couriers) {
        int cost = courier.distance(bottle) + bottle.distance(restaurant);
        row.push_back(-cost);
      }
      costs.push_back(row);
    }
  }

  unsigned solve() {
    std::vector<int> xy(nx, -1), yx(ny, -1), lx(nx), ly(ny, 0), augmenting(ny);
    std::vector<bool> s(nx);
    std::vector<std::pair<int, int>> slack(ny, std::make_pair(0, 0));
    for (unsigned x = 0; x < nx; x++)
      lx[x] = *std::max_element(costs[x].cbegin(), costs[x].cend());
    for (unsigned root = 0; root < nx; root++) {
      std::fill(augmenting.begin(), augmenting.end(), -1);
      std::fill(s.begin(), s.end(), false);
      s[root] = true;
      for (unsigned y = 0; y < ny; y++)
        slack[y] = std::make_pair(lx[root] + ly[y] - costs[root][y], root);
      int y = -1;
      for (;;) {
        int delta = INT_MAX, x = -1;
        for (unsigned yy = 0; yy < ny; yy++)
          if (augmenting[yy] == -1)
            if (slack[yy].first < delta) {
              delta = slack[yy].first;
              x = slack[yy].second;
              y = yy;
            }
        assert(s[x]);
        if (delta > 0) {
          for (unsigned x = 0; x < nx; x++)
            if (s[x])
              lx[x] -= delta;
          for (unsigned y = 0; y < ny; y++)
            if (augmenting[y] > -1)
              ly[y] += delta;
            else
              slack[y].first -= delta;
        }
        assert(lx[x] + ly[y] == costs[x][y]);
        augmenting[y] = x;
        x = yx[y];
        if (x == -1)
          break;
        s[x] = true;
        for (unsigned y = 0; y < ny; y++)
          if (augmenting[y] == -1) {
            std::pair<int, int> alt = std::make_pair(lx[x] + ly[y] - costs[x][y], x);
            if (slack[y].first > alt.first)
              slack[y] = alt;
          }
      }
      while (y > -1) {
        int x = augmenting[y];
        int prec = xy[x];
        yx[y] = x;
        xy[x] = y;
        y = prec;
      }
    }
    return -(std::accumulate(lx.cbegin(), lx.cend(), 0) + std::accumulate(ly.cbegin(), ly.cend(), 0));
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

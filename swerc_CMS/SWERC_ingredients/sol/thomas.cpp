#include <cstdio>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using ::std::make_pair;
using ::std::pair;
using ::std::string;
using ::std::unordered_map;
using ::std::unordered_set;
using ::std::vector;

pair<int, int> get_score(
    const unordered_map<string, vector<pair<string, pair<int, int>>>>& dep,
    unordered_map<string, pair<int, int>>* scores, const string& dish) {
  if (scores->count(dish) > 0) {
    return scores->at(dish);
  }
  pair<int, int> score(0, 0);
  if (dep.count(dish) > 0) {
    for (const auto& d : dep.at(dish)) {
      pair<int, int> s = get_score(dep, scores, d.first);
      s.first += d.second.first;
      s.second += d.second.second;
      if (score.second == 0 || s.first < score.first ||
          (s.first == score.first && s.second > score.second)) {
        score = s;
      }
    }
  }
  scores->emplace(dish, score);
  return score;
}

pair<int, int> knapsack(vector<vector<pair<int, int>>>* d,
                        const vector<pair<int, int>>& t, int b, int i) {
  if (b <= 0 || i == 0) {
    return make_pair(0, 0);
  }
  if ((*d)[i - 1][b - 1].second >= 0) {
    return (*d)[i - 1][b - 1];
  }
  pair<int, int> with = knapsack(d, t, b - t[i - 1].first, i - 1);
  with.first += t[i - 1].first;
  with.second += t[i - 1].second;
  pair<int, int> without = knapsack(d, t, b, i - 1);
  if (with.first > b || without.second > with.second ||
      (without.second == with.second && without.first < with.first)) {
    with = without;
  }
  (*d)[i - 1][b - 1] = with;
  return with;
}

int main() {
  int B, N;
  scanf("%d%d", &B, &N);
  unordered_map<string, vector<pair<string, pair<int, int>>>> dep;
  for (int i = 0; i < N; ++i) {
    char dish[21], base[21];
    int price, prestige;
    scanf("%s%s%*s%d%d", dish, base, &price, &prestige);
    dep[dish].emplace_back(base, make_pair(price, prestige));
  }
  unordered_map<string, pair<int, int>> scores;
  vector<pair<int, int>> t;
  for (const auto& d : dep) {
    pair<int, int> score = get_score(dep, &scores, d.first);
    if (score.second > 0) {
      t.push_back(score);
      // printf("%d %d\n", score.first, score.second);
    }
  }
  vector<vector<pair<int, int>>> d(
      t.size(), vector<pair<int, int>>(B, make_pair(-1, -1)));
  pair<int, int> ans = knapsack(&d, t, B, t.size());
  printf("%d\n%d\n", ans.second, ans.first);
  return 0;
}

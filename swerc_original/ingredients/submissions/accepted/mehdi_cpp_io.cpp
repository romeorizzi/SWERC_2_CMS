#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

vector<vector<pair<int, pair<int, int> > > > edges;
map<string,size_t> nodes;
const int MAX_DISHES = 10000;
pair<int, int> dishes[MAX_DISHES];
int known[MAX_DISHES];
const int MAX_COST = 10000;
const int INF = MAX_DISHES * MAX_COST + 1;
const int MAX_BUDGET = 10000;
int prest[MAX_BUDGET+1];

size_t node(string& s) {
  size_t& n = nodes[s];
  if (!n) {
    edges.push_back(vector<pair<int, pair<int, int> > >());
    n = edges.size();
  }
  return n - 1;
}

pair<int, int>& get(size_t n) {
  pair<int, int>& res = dishes[n];
  if (known[n]++ || edges[n].size() == 0)
    return res;
  res = make_pair(INF, 0);
  for (size_t i = 0; i < edges[n].size(); i++) {
    pair<int, int>& b = get(edges[n][i].first);
    res = min(res, make_pair(b.first + edges[n][i].second.first, - (b.second + edges[n][i].second.second)));
  }
  res.second = -res.second;
  return res;
}

int main() {
  int B, N;
  cin >> B >> N;

  for (int i = 0; i < N; i++) {
    string derived, base, _ingredient;
    int added_price, added_prestige;
    cin >> derived >> base >> _ingredient >> added_price >> added_prestige;
    size_t der = node(derived), bas = node(base);
    edges[der].push_back(make_pair(bas, make_pair(added_price, added_prestige)));
  }
  for (int i = int(edges.size()) - 1; i >= 0; i--) {
    pair<int, int>& d = get(i);
    int price = d.first, prestige = d.second;
    for (int b = 0; b <= B - price; b++)
      prest[b] = max(prest[b], prest[b + price] + prestige);
  }
  pair<int, int> best = make_pair(-1, 0);
  for (int b = 0; b <= B; b++)
    best = max(best, make_pair(prest[b], b));
  cout << best.first << endl << B - best.second << endl;

  return 0;
}

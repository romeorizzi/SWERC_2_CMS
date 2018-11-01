#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <tuple>
#include <limits>

using namespace std;

typedef vector<list<tuple<int,int,int>>> graph_t;

const int INFINITY_COST=numeric_limits<int>::max();

// Topological sort via DFS, iterative to avoid deep recursions
void dfs(const graph_t &g, list<int> &order, vector<bool> &visited)
{
  vector<pair<unsigned,bool>> stack;

  for(unsigned start=0;start<g.size();++start) {
    if(visited[start])
      continue;

    stack.push_back(make_pair(start,true));

    while(!stack.empty()) {
      auto p=stack.back();
      stack.pop_back();

      if(p.second) {
        // Regular DFS browse
        if(visited[p.first])
          continue;

        stack.push_back(make_pair(p.first,false));
        for(auto e : g[p.first]) {
          stack.push_back(make_pair(get<0>(e),true));
        }
        visited[p.first]=true;
      } else {
        // We revisit a parent after its children
        order.push_front(p.first);
      }
    }
  }
}

int main()
{
  int budget, n;

  cin >> budget >> n;

  vector<string> name;
  unordered_map<string, int> index;
  graph_t graph;

  cin.ignore();

  unsigned nb_dishes=0;
  for(int i=0; i<n; ++i) {
    string s;
    getline(cin, s, ' ');

    if(index.find(s)==index.end()) {
      name.push_back(s);
      index[s]=nb_dishes;
      ++nb_dishes;
    }
    unsigned to=index[s];

    string base;
    getline(cin, base, ' ');

    if(index.find(base)==index.end()) {
      name.push_back(base);
      index[base]=nb_dishes;
      ++nb_dishes;
    }

    // Ignore ingredient name
    string ingredient;
    getline(cin, ingredient, ' ');

    int c2, p2;
    cin >> c2 >> p2;
    
    // Eat newline
    cin.ignore();

    unsigned from=index[base];
    if(graph.size()<nb_dishes)
      graph.resize(nb_dishes);

    graph[from].push_back(make_tuple(to,c2,p2));
  }

  list<int> order;
  vector<bool> visited(nb_dishes);

  // Topological sort
  dfs(graph,order,visited);

  // Get prestige and cost of every dish
  vector<int> cost(nb_dishes);
  vector<int> prestige(nb_dishes);

  for(unsigned i=0;i<nb_dishes;++i) {
    for(auto t : graph[i])
      cost[get<0>(t)] = INFINITY_COST;
  }

  for(auto o : order) {
    for(auto t : graph[o]) {
      if(cost[get<0>(t)]>cost[o]+get<1>(t)) {
        cost[get<0>(t)]=cost[o]+get<1>(t);
        prestige[get<0>(t)]=prestige[o]+get<2>(t);
      } else if(cost[get<0>(t)]==cost[o]+get<1>(t)) {
        prestige[get<0>(t)]=max(prestige[get<0>(t)],prestige[o]+get<2>(t));
      }
    }
  }
  
  // Dynamic programming knapsack
  vector<int> best(budget+1);
  vector<vector<bool>> used(budget+1);
  for(int k=0;k<=budget;++k) {
    used[k].resize(nb_dishes);
    if(k>0)
      best[k]=best[k-1];

    int besti=-1;
    for(unsigned i=0;i<nb_dishes;++i) {
      if(cost[i]<=k && !used[k-cost[i]][i] && prestige[i]+best[k-cost[i]]>best[k]) {
        best[k]=prestige[i]+best[k-cost[i]];
        besti=i;
      }

      if(besti!=-1) {
        used[k]=used[k-cost[besti]];
        used[k][besti]=true;
      } else if(k>0)
        used[k]=used[k-1];
    }
  }

  int tp=0, tc=0, nd=0;  
  for(unsigned i=0;i<nb_dishes;++i) {
    if(used[budget][i]) {
      tp+=prestige[i];
      tc+=cost[i];
      ++nd;
    }
  }

  cout << tp << endl;
  cout << tc << endl;
}

#include<cstdio>
#include<map>
#include<vector>
#include<algorithm>

using namespace std;

const int mod = 10000000;
const int SIZE_RULES_MAX = 60 ;
const int SIZE_ALPHABET_MAX = 62 ;
char rules[SIZE_RULES_MAX+1];
char letters[SIZE_ALPHABET_MAX+1];
vector<int> rules_start;
vector<int> rules_end;

map<pair<int,vector<int>>, int> dyn;
int count(int len, vector<int> states) {
  for(int rule = 0; rule < states.size(); rule++)
    switch(rules[states[rule]]) {
      case '>': states[rule]++; break;
      case 0 : case '|': states[rule] = rules_start[rule]; break;
    }

  if(len == 0) {
    for(int rule = 0; rule < states.size(); rule++)
      if(states[rule] >= rules_end[rule])
        return 0;
    return 1;
  }

  auto it = dyn.insert(make_pair(make_pair(len, states), 0));
  int &res = it.first->second;
  if(it.second) {
    for(int l = 0; letters[l]; l++) {
      vector<int> states_new;
      for(int rule = 0; rule < states.size(); rule++) {
        int start_pat =
          states[rule] >= rules_end[rule] ?
          rules_end[rule] : rules_start[rule];
        if(rules[states[rule]] == letters[l])
          states_new.push_back(states[rule] + 1);
        else if(rules[start_pat] == letters[l])
          states_new.push_back(start_pat + 1);
        else
          states_new.push_back(start_pat);
      }
      res = res + count(len - 1, states_new);
      if(res >= mod) res -= mod;
    }
  }
  return res;
}

int main() {
  int len;
  scanf("%d%s%s", &len, letters, rules);
  rules_start.push_back(0);
  for(int c = 0; rules[c]; c++)
    switch(rules[c]) {
      case '>': rules_end.push_back(c+1); break;
      case '|': rules_start.push_back(c+1); break;
    }

  printf("%d\n", count(len, rules_start));
  return 0;
}

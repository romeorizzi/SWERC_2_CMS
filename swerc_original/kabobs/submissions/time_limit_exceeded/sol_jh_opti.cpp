#include<cstdio>
#include<map>
#include<vector>
#include<algorithm>
#include<cstring>

using namespace std;

const int mod = 10000000;
const int SIZE_RULES_MAX = 60 ;
const int SIZE_ALPHABET_MAX = 62 ;
char rules[SIZE_RULES_MAX+1];
char letters[SIZE_ALPHABET_MAX+1];
vector<int> rules_start;
vector<int> rules_end;

map<vector<int>, int> state2id;
vector<vector<int>> id2state;
vector<vector<int>> trans;

int next_state(int state, int letter) {
  int &r = trans[state][letter];
  if(r == -1) {
    vector<int> state_new = id2state[state];
    for(int rule = 0; rule < state_new.size(); rule++) {
      int & pos = state_new[rule];
      int start_pat = pos >= rules_end[rule] ? rules_end[rule] : rules_start[rule];
      if(rules[pos] == letters[letter]) pos++;
      else if(rules[start_pat] == letters[letter]) pos = start_pat + 1;
      else pos = start_pat;
      switch(rules[pos]) {
        case '>': pos++; break;
        case 0 : case '|': pos = rules_start[rule]; break;
      }
    }
    auto it = state2id.insert(make_pair(state_new, id2state.size()));
    if(it.second) {
      id2state.push_back(state_new);
      trans.push_back(vector<int>(SIZE_ALPHABET_MAX, -1));
    }
    r = it.first->second;
  }
  return r;
}

int dyn[501][100000];
int count(int len, int state) {
  if(len == 0) {
    const vector<int>& st = id2state[state];
    for(int rule = 0; rule < st.size(); rule++)
      if(st[rule] >= rules_end[rule])
        return 0;
    return 1;
  }

  int &res = dyn[len][state];
  if(res == -1) {
    res = 0;
    for(int l = 0; letters[l]; l++) {
      res = res + count(len - 1, next_state(state, l));
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

  state2id[rules_start] = 0;
  id2state.push_back(rules_start);
  trans.push_back(vector<int>(SIZE_ALPHABET_MAX, -1));
  memset(dyn, -1, sizeof(dyn));
  printf("%d\n", count(len, 0));
  return 0;
}

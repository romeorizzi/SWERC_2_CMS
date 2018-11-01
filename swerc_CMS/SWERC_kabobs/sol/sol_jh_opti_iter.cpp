#include<cstdio>
#include<map>
#include<vector>
#include<algorithm>
#include<cstring>

using namespace std;

const int mod = 10000000;
const int SIZE_RULES_MAX = 70 ;
const int SIZE_ALPHABET_MAX = 62 ;
char rules[SIZE_RULES_MAX+1];
char letters[SIZE_ALPHABET_MAX+1];

vector<int> rules_start;
vector<int> rules_end;

map<vector<int>, int> state2id;
vector<vector<int>> id2state;
vector<vector<int>> trans;

void build_auto(const vector<int>& state) {
  int id = id2state.size();
  id2state.push_back(state);
  trans.push_back(vector<int>(SIZE_ALPHABET_MAX, -1));
  for(int letter = 0; letters[letter]; letter++) {
    vector<int> state_new = state;
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
    if(it.second) build_auto(state_new);
    trans[id][letter] = it.first->second;
  }
}

int count(int len) {
  int dyn[2][id2state.size()];
  for(int state = 0; state < id2state.size(); state++) {
    dyn[0][state] = 1;
    const vector<int>& st = id2state[state];
    for(int rule = 0; rule < st.size(); rule++)
      if(st[rule] >= rules_end[rule])
        dyn[0][state] = 0;
  }
  for(int i = 1; i <= len; i++) {
    int cur = i & 1;
    for(int state = 0; state < id2state.size(); state++) {
      long long s = 0;
      for(int l = 0; letters[l]; l++)
        s += dyn[cur ^ 1][trans[state][l]];
      dyn[cur][state] = s % mod;
    }
  }
  return dyn[len & 1][0];
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
  build_auto(rules_start);
  printf("%d\n", count(len));
  return 0;
}

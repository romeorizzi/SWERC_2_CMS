#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

unordered_map<string, int> canEarn;
unordered_map<string, int> _maxEarn;

int maxEarn(const string& chain) {
  int& me = _maxEarn[chain];
  if (me > 0)
    return me - 1;
  me = 0;
  for (auto ce : canEarn) {
    const string& portion = ce.first;
    size_t pos = 0;
    for (;;pos++) {
      pos = chain.find(portion, pos);
      if (pos == string::npos)
        break;
      string newChain = chain.substr(0, pos) + chain.substr(pos + portion.length());
      me = max(me, maxEarn(newChain) + ce.second - 1);
    }
  }
  me++;
  return me - 1;
}

int main() {
  string initChain;
  int nbChildren;
  cin >> initChain >> nbChildren;

  for (int child = 0; child < nbChildren; child++) {
    string portion;
    int willingToPay;
    cin >> portion >> willingToPay;
    canEarn[portion] = max(canEarn[portion], willingToPay + 1);
    reverse(portion.begin(), portion.end());
    canEarn[portion] = max(canEarn[portion], willingToPay + 1);
  }

  cout << maxEarn(initChain) << endl;

  return 0;
}

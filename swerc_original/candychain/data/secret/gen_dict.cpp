#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

const char MIN_CHAR = 'a';
const char MAX_CHAR = 'd';
const int SEED = 331113;

const int MAX_DICT_WORD_LENGTH = 10;

int main() {
  srand(time(0));
  std::map<std::string, int> d;
  for (char c = MIN_CHAR; c <= MAX_CHAR; ++c) {
    d[std::string(1, c)] = 1;
  }
  for (int T = 0; T < 100000 && d.size() < 200; ++T) {
    int l = 1 + rand() % MAX_DICT_WORD_LENGTH;
    std::string s(l, 'a');
    for (int i = 0; i < l; ++i) {
      s[i] = rand() % (MAX_CHAR - MIN_CHAR + 1) + MIN_CHAR;
    }
    d[s] = l * l + rand() % (l*l*l*l);
  }
  assert(d.size() <= 200);
  std::cout << d.size() << std::endl;
  for (const auto& e : d) {
    std::cout << e.first << " " << e.second << std::endl;
  }
}

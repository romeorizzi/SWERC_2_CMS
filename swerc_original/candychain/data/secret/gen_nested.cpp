// Generator that works by building the candy chain (starting from the
// empty chain, iteratively adding a word somewhere). This builds
// tests where the optimal solution for the test case potentially has
// a high nesting depth.

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

const int N = 50;
const int SEED = 3311131;

int main() {
  // Input a dictionnary.
  int w;
  std::cin >> w;
  std::cin.ignore();
  std::vector<std::pair<std::string, int>> input_dict;
  std::vector<std::string> dict;
  for (int i = 0; i < w; ++i) {
    std::string word;
    int score;
    std::cin >> word >> score;
    input_dict.push_back(std::make_pair(word, score));
    dict.push_back(word);
    std::reverse(word.begin(), word.end());
    dict.push_back(word);
  }

  std::string s;
  for (int T = 0; T < N * 10; ++T) {
    int insert_pos = rand() % (s.size() + 1);
    int insert_word = rand() % dict.size();
    std::string candidate =
        s.substr(0, insert_pos) + dict[insert_word] + s.substr(insert_pos);
    if (candidate.size() > N) continue;
    s = candidate;
  }
  std::cout << s << std::endl;

  assert(input_dict.size() <= 200);
  std::cout << input_dict.size() << std::endl;

  for (const auto& e : input_dict) {
    std::cout << e.first << " " << e.second << std::endl;
  }
}

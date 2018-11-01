#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

using ::std::pair;
using ::std::string;
using ::std::unordered_map;
using ::std::vector;

// Rule of the form A -> BC with weight w.
struct Rule {
  int a;
  int b;
  int c;
  int w;
};

int main() {
  char objective[100];
  scanf("%s", objective);
  int N = strlen(objective);
  if (N == 0) {
    printf("0\n");
    return 0;
  }

  // Creating grammar from dictionnary.
  int d;
  scanf("%d", &d);
  vector<pair<string, int>> dictionary;
  for (int _i = 0; _i < d; ++_i) {
    char word[1000];
    int w;
    scanf("%s%d", word, &w);
    dictionary.emplace_back(word, w);
    string reversed(word);
    std::reverse(reversed.begin(), reversed.end());
    dictionary.emplace_back(reversed, w);
  }
  std::sort(dictionary.begin(), dictionary.end(),
            [](const pair<string, int>& a, const pair<string, int>& b) {
              return a.first < b.first ||
                     (a.first == b.first && a.second > b.second);
            });
  vector<Rule> G;
  vector<int> letter(26, -1);
  G.push_back({
      0,
      0,
      0,
      0,
  });
  for (int c = 2; c < 28; ++c) {
    G.push_back({
        c,
        1,
        c,
        0,
    });
  }
  int x = 42;
  vector<int> path;
  string previous = "";
  for (const auto& e : dictionary) {
    const string& word = e.first;
    int w = e.second;
    int n = word.size();
    if (n > N || word == previous) {
      continue;
    }
    if (n == 1) {
      if (w > letter[word[0] - 'a']) {
        letter[word[0] - 'a'] = w;
      }
      continue;
    }
    if (n == 2) {
      G.push_back({
          0,
          word[0] - 'a' + 2,
          word[1] - 'a' + 2,
          w,
      });
      G.push_back({
          1,
          word[0] - 'a' + 2,
          word[1] - 'a' + 2,
          w,
      });
      continue;
    }
    int m = 0;
    while (m < n - 2 && m < previous.size() - 2 && word[m] == previous[m]) {
      ++m;
    }
    if (m == 0) {
      G.push_back({
          0,
          word[0] - 'a' + 2,
          x,
          0,
      });
      G.push_back({
          1,
          word[0] - 'a' + 2,
          x,
          0,
      });
      // fprintf(stderr, "%d -> %c %d\n", 0, word[0], x);
    } else if (m == n - 2) {
      G.push_back({
          path[m - 1],
          word[m] - 'a' + 2,
          word[n - 1] - 'a' + 2,
          w,
      });
      // fprintf(stderr, "%d -> %c %c\n", path[m - 1], word[m], word[n - 1]);
      previous = word;
      continue;
    } else {
      G.push_back({
          path[m - 1],
          word[m] - 'a' + 2,
          x,
          0,
      });
      // fprintf(stderr, "%d -> %c %d\n", path[m - 1], word[m], x);
    }
    path.resize(m);
    path.push_back(x);
    for (int i = m + 1; i < n - 2; ++i) {
      G.push_back({
          x,
          word[i] - 'a' + 2,
          x + 1,
          0,
      });
      // fprintf(stderr, "%d -> %c %d\n", x, word[i], x + 1);
      ++x;
      path.push_back(x);
    }
    G.push_back({
        x,
        word[n - 2] - 'a' + 2,
        word[n - 1] - 'a' + 2,
        w,
    });
    // fprintf(stderr, "%d -> %c %c\n", x, word[n - 2], word[n - 1]);
    ++x;
    previous = word;
  }

  // CYK
  vector<vector<unordered_map<int, int>>> T(N,
                                            vector<unordered_map<int, int>>(N));
  for (int i = 0; i < N; ++i) {
    if (letter[objective[i] - 'a'] >= 0) {
      T[i][i][0] = letter[objective[i] - 'a'];
      T[i][i][1] = letter[objective[i] - 'a'];
    } else {
      T[i][i][0] = 0;
    }
    T[i][i][objective[i] - 'a' + 2] = 0;
  }
  for (int l = 1; l < N; ++l) {
    for (int i = 0; i + l < N; ++i) {
      for (int k = i; k < i + l; ++k) {
        for (const auto& r : G) {
          const auto left = T[i][k].find(r.b);
          const auto right = T[k + 1][i + l].find(r.c);
          if (left != T[i][k].end() && right != T[k + 1][i + l].end() &&
              right->second + left->second + r.w > T[i][i + l][r.a]) {
            T[i][i + l][r.a] = right->second + left->second + r.w;
          }
        }
      }
    }
  }
  printf("%d\n", T[0][N - 1][0]);
  return 0;
}

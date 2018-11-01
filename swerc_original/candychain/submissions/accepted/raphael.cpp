// DP algorithm for word_search in O(N^4 * W) (time), O(N^3 * W) (memory).

#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

const bool DEBUG = false;

struct TupleHasher {
  std::size_t operator()(const std::tuple<int, int, int, int>& k) const {
    return std::get<0>(k) ^ (std::get<1>(k) << 8) ^
      (std::get<2>(k) << 16) ^ (std::get<3>(k) << 30);
 }
};

std::unordered_map<std::tuple<int, int, int, int>, int, TupleHasher> dyna;
//std::map<std::tuple<int, int, int, int>, int> dyna;

struct TrieNode {
  TrieNode() : word_score(-1), children('z' - 'a' + 1, -1) {}
  int word_score;
  // Indexes of children. -1 if no child.
  std::vector<int> children;
};

struct Trie {
  Trie() : nodes_(1) {}
  void Add(const std::string& word, int score) {
    // position 0 means the root.
    assert(score >= 0);
    int trie_pos = 0;
    if (DEBUG) {
      std::cout << "Adding: '" << word << "': " << score << std::endl;
    }
    TrieNode* cur = &nodes_[0];
    for (int i = 0; i < word.size(); ++i) {
      int c = word[i] - 'a';
      int& next_index = cur->children.at(c);
      if (next_index < 0) {
        next_index = nodes_.size();
        nodes_.push_back(TrieNode());
      }
      cur = &nodes_[next_index];
    }
    // Use max in case of duplicate.
    cur->word_score = std::max(cur->word_score, score);
  }

  int GetScore(int trie_pos) const {
    if (trie_pos < 0) {
      return -1;
    }
    return nodes_.at(trie_pos).word_score;
  }

  // Returns new tree pos, negative if 'trie_pos' is already invalid.
  int Next(int trie_pos, char c) const {
    if (trie_pos < 0) {
      return trie_pos;
    }
    return nodes_.at(trie_pos).children.at(c - 'a');
  }

  std::vector<TrieNode> nodes_;
};

int solve(const std::string& s, const Trie& trie, int i, int j, int trie_pos,
          bool require_match, int debug_word_start) {
  if (DEBUG) {
    std::cout << "COMPUTE: i=" << i << " j = " << j << " trie_pos: " << trie_pos
              << " debug_word_start: " << debug_word_start << std::endl;
  }
  if (i == j) {
    int score = trie.GetScore(trie_pos);
    if (DEBUG) {
      std::cout << "SCORE: i=" << i << " trie_pos: " << trie_pos
                << " score: " << score << std::endl;
    }
    if (!require_match && score < 0) return 0;
    return score;
  }
  assert(i < j);
  assert(j <= s.size());
  auto t = std::make_tuple(i, j, trie_pos, require_match);
  auto it = dyna.find(t);
  if (it != dyna.end()) return it->second;
  int& res = dyna[t];
  res = require_match ? -1 : 0;

  // First case: make progress on current word (advance from i to i+1).
  const int next_trie_pos = trie.Next(trie_pos, s[i]);
  if (next_trie_pos >= 0) {
    int res_next = solve(s, trie, i + 1, j, next_trie_pos, require_match,
                         debug_word_start);
    if (DEBUG) {
      std::cout << "continue: i=" << i << " j=" << j << " trie_pos=" << trie_pos
                << " debug_word_start=" << debug_word_start
                << " res= " << res_next << std::endl;
    }
    if (res_next >= 0) {
      res = res_next;
    }
  }

  // Second case: skip [i, k) for our current (set of)
  // words. Continue our set of words from k onwards.
  // k == i+1 just means: we are just requiring to continue the
  // current set of words.
  for (int k = i + 1; k <= j; ++k) {
    if (k == j && trie_pos == 0) continue;
    int left = solve(s, trie, i, k, 0, /*require_match*/ trie_pos != 0,
                     /*debug_word_start*/ i);
    int right = solve(s, trie, k, j, trie_pos, require_match, debug_word_start);
    if (DEBUG) {
      std::cout << "skip: i=" << i << " j=" << j << " trie_pos=" << trie_pos
                << " k1=" << i + 1 << " k=" << k
                << " debug_word_start=" << debug_word_start << " left=" << left
                << " right=" << right << std::endl;
    }
    if (left >= 0 && right >= 0) {
      res = std::max(res, left + right);
    }
  }
  return res;
}

int main() {
  Trie trie;
  std::string s;
  std::cin >> s;
  int w;
  std::cin >> w;
  std::cin.ignore();
  for (int i = 0; i < w; ++i) {
    std::string word;
    int score;
    std::cin >> word >> score;
    trie.Add(word, score);
    std::reverse(word.begin(), word.end());
    trie.Add(word, score);
  }
  if (DEBUG) {
    std::cout << "input string:'" << s << "'" << std::endl;
    std::cout << "Trie size: " << trie.nodes_.size() << std::endl;
  }
  int res = solve(s, trie, 0, s.size(), 0, false, 0);
  std::cout << res << std::endl;
}

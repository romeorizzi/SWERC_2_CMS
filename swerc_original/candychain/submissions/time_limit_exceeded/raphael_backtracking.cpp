// Backtracking algorithm for Candy Chain. Should time out.

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

int solve(std::string s, const Trie& trie) {
  if (s.empty()) {
    return 0;
  }
  int best_score = 0;

  for (int start = 0; start < s.size(); ++start) {
    for (int trie_pos = 0, i = 0;
	 trie_pos >= 0 && i + start < s.size();
	 ++i) {
      trie_pos = trie.Next(trie_pos, s[i + start]);
      int word_score = trie.GetScore(trie_pos);
      if (word_score >= 0) {
	int score = word_score + solve(s.substr(0, start) + s.substr(start + i + 1), trie);
	if (score > best_score) {
	  best_score = score;
	}
      }
    }
  }
  return best_score;
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
  int res = solve(s, trie);
  std::cout << res << std::endl;
}

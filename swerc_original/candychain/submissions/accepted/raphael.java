// Raphael. This is raphael.cpp translated into Java.

import java.util.*;

class TrieNode {
    TrieNode() {
	word_score = -1;
	children = new Vector<Integer>();
	for (int i = 0; i < 'z' - 'a' + 1; ++i) {
	    children.add(-1);
	}
    }
    int word_score;
    Vector<Integer> children;
}

class Trie {
    Trie() {
	nodes = new Vector<TrieNode>();
	nodes.add(new TrieNode());
    }
    void Add(String word, int score) {
	int pos = 0;
	TrieNode cur = nodes.get(0);
	for (int i = 0; i < word.length(); ++i) {
	    int c = word.charAt(i) - 'a';
	    int next = cur.children.get(c);
	    if (next < 0) {
		next = nodes.size();
		cur.children.set(c, next);
		nodes.add(new TrieNode());
	    }
	    cur = nodes.get(next);
	}
	cur.word_score = Math.max(cur.word_score, score);
    }
    int GetScore(int pos) {
	if (pos < 0) return -1;
	return nodes.get(pos).word_score;
    }
    int Next(int pos, char c) {
	if (pos < 0) return pos;
	return nodes.get(pos).children.get(c - 'a');
    }
    Vector<TrieNode> nodes;
}

class State {
    int i;
    int j;
    int trie_pos;
    int require_match;
    @Override
    public int hashCode() {
        return i + (j << 8) + (trie_pos << 16) + (require_match << 30);
    }
    @Override
    public boolean equals(Object obj) {
	State that = (State) obj;
	return i == that.i && j == that.j && trie_pos == that.trie_pos &&
	    require_match == that.require_match;
    }
}

class raphael {

    static String text;
    static int n;
    static Trie trie = new Trie();
    static HashMap<State, Integer> dyna = new HashMap<>();
	
    static int solve(int i, int j, int trie_pos,
		     boolean require_match) {
	if (i == j) {
	    int score = trie.GetScore(trie_pos);
	    if (!require_match && score < 0) return 0;
	    return score;
	}
	State state = new State();
	state.i = i; state.j = j; state.trie_pos = trie_pos;
	state.require_match = require_match ? 1 : 0;
	Integer it = dyna.get(state);
	if (it != null) {
	    return it;
	}
	int res = require_match ? -1 : 0;

	int next_trie_pos = trie.Next(trie_pos, text.charAt(i));
	if (next_trie_pos >= 0) {
	    int res_next = solve(i + 1, j, next_trie_pos, require_match);
	    if (res_next >= 0) {
		res = res_next;
	    }
	}

	for (int k = i + 1; k <= j; ++k) {
	    if (k == j && trie_pos == 0) continue;
	    int left = solve(i, k, 0, /*require_match*/ trie_pos != 0);
	    int right = solve(k, j, trie_pos, require_match);
	    if (left >= 0 && right >= 0) {
		res = Math.max(res, left + right);
	    }
	}
	dyna.put(state, res);
	return res;
    }

    
    static String reverse(String w) {
        StringBuffer sb = new StringBuffer();
        for (int i = w.length() - 1; i >= 0; i--)
            sb.append(w.charAt(i));
        return sb.toString();
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        text = in.nextLine();
        n = text.length();
        // reading words
        int w = in.nextInt();
        while (w-- > 0) {
            String word = in.next();
            int score = in.nextInt();
            String wordr = reverse(word);
            trie.Add(word, score);
	    trie.Add(wordr, score);
        }
        in.close();
        // System.err.println(words.size() + " words");
        System.out.println(solve(0, n, 0, false));
    }
}

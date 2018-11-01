// Jean-Christophe

import java.util.*;

// algorithm: we generalize to any text segment [i..j[ + memoization
// with two functions:
//   score(i,j)      = maximal score, possibly ignoring some characters
//   scoreExact(i,j) = maximal score using all characters, -1 otherwise

class Word {
    final String chars;
    final int    score;
    Word(String chars, int score) { this.chars = chars; this.score = score; }
}

class CandyChain {

    static String text;
    static int n;
    static Vector<Word> words = new Vector<>();

    // sub-sequence test (used only for optimization)
    static boolean isSubSeq(String s, String t) {
        int m = s.length(), n = t.length();
        if (m > n) return false;
        int j = 0;
        for (int i = 0; j < m && i < n; i++)
            if (s.charAt(j) == t.charAt(i))
                j++;
        return j == m;
    }

    static int add(int x, int y) {
        if (x < 0 || y < 0) return -1;
        return x+y;
    }

    static int[][] memoScore;

    // maximal score in text[i..j[, possibly ignoring some characters
    // result >= 0
    static int score(int i, int j) {
        assert 0 <= i && i <= j && j <= n;
        if (i == j) return 0;
        if (memoScore[i][j] != -2) return memoScore[i][j];

        int score = scoreExact(i, j);
        score = Math.max(score, Math.max(score(i+1, j), score(i, j-1)));
        for (int k = i+1; k < j; k++)
            score = Math.max(score, score(i, k) + score(k, j));

        assert score >= 0;
        memoScore[i][j] = score;
        return score;
    }

    static int[][] memoExact;

    // maximal score using exactly text[i..j[
    // -1 if not possible
    static int scoreExact(int i, int j) {
        assert 0 <= i && i <= j && j <= n;
        if (memoExact[i][j] != -2) return memoExact[i][j];
        if (i == j) return 0;
        int score = -1;
        if (j - i == 1) {
            for (Word w: words)
                if (w.chars.length() == 1 && w.chars.charAt(0) == text.charAt(i))
                    score = Math.max(score, w.score);
            memoExact[i][j] = score;
            return score;
        }
        // split in two non-empty halves
        for (int k = i+1; k < j; k++) {
            int left = scoreExact(i, k), right = scoreExact(k, j);
            score = Math.max(score, add(left, right));
        }
        // otherwise, a single word starts at i and ends at j
        for (Word w: words) {
            int m = w.chars.length();
            if (m < 2 || m > j - i) continue;
            if (w.chars.charAt(0) != text.charAt(i)) continue;
            if (w.chars.charAt(m-1) != text.charAt(j-1)) continue;
            // OPTIM (seems useless)
	    if (!isSubSeq(w.chars.substring(1, m-1), text.substring(i+1, j-1)))
		continue;
            score = Math.max(score,
                             add(w.score, solve(w.chars, i, i+1, j-1, 1)));
        }

        // System.out.format("exact %d..%d => %d\n", i, j, score);
        memoExact[i][j] = score;
        return score;
    }

    static HashMap<String, HashMap<Integer, Integer>> memoSolve =
        new HashMap<>();

    // looks for w[k..[ in text[next..end[ and maximizes scores of
    // unused characters in the process
    // (the last character has already been checked in scoreExact)
    static int solve(String w, int prev, int next, int end, int k) {
        assert prev < next;
        assert next <= end;
        assert k < w.length();
        if (k == w.length() - 1) return scoreExact(next, end); // done
        if (end - next < w.length() - 1 - k) return -1; // not enough characters
        HashMap<Integer, Integer> tab = memoSolve.get(w);
        int key = (prev << 24) | (next << 16) | (end << 8) | k;
        if (tab.containsKey(key)) return tab.get(key);
        int score = -1;
        if (w.charAt(k) == text.charAt(next))
            score = Math.max(score,
                             add(scoreExact(prev+1, next),
                                 solve(w, next, next+1, end, k+1)));
        score = Math.max(score, solve(w, prev, next+1, end, k));
        tab.put(key, score);
        return score;
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
        // memo tables
        memoExact = new int[n+1][n+1];
        memoScore = new int[n+1][n+1];
        for (int i = 0; i <= n; i++)
            for (int j = 0; j <= n; j++) {
                memoExact[i][j] = -2;
                memoScore[i][j] = -2;
            }
        // reading words
        int w = in.nextInt();
        while (w-- > 0) {
            String word = in.next();
            int score = in.nextInt();
            String wordr = reverse(word);
            if(!isSubSeq(word, text) && !isSubSeq(wordr, text)) continue;
            words.add(new Word(word, score));
            memoSolve.put(word, new HashMap<>());
            if (!wordr.equals(word)) {
                words.add(new Word(wordr, score));
                memoSolve.put(wordr, new HashMap<>());
            }
        }
        in.close();
        // System.err.println(words.size() + " words");
        System.out.println(score(0, n));
    }
}

/*
Local Variables:
compile-command: "javac CandyChain.java"
End:
*/


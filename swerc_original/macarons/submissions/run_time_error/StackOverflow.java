// Jean-Christophe
// correct computation, but may overflow the stack
// (and takes too much time on other tests)

import java.util.*;

class Main {

    // all computations modulo
    static final int MOD = 1000000000;

    static int N;
    static long M;

    // row i and column j
    static int solve(boolean[] current, boolean[] next, int i, int j) {
        if (j == M) return 1;
        if (i == N) return solve(next, new boolean[N], 0, j+1);
        if (current[i]) return solve(current, next, i+1, j);
        // macaron 1x1
        int s1 = solve(current, next, i+1, j);
        // 1x2 horizontal
        next[i] = true;
        int s2 = j < M-1 ? solve(current, next, i+1, j) : 0;
        next[i] = false;
        // 2x1 vertical
        int s3 = i < N-1 && !current[i+1] ? solve(current, next, i+2, j) : 0;
        return (s1 + s2 + s3) % MOD;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        N = in.nextInt();
        M = in.nextLong();
        in.close();
        System.out.println(solve(new boolean[N], new boolean[N], 0, 0));
    }
}

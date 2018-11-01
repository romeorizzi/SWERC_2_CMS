// Jean-Christophe
// correct computation, but may overflow the stack or the memory
// (and takes too much time on other tests)

import java.util.*;

class Main {

    // all computations modulo
    static final int MOD = 1000000000;

    static int N;
    static long M;

    static boolean[][] table;

    // row i and column j
    static int solve(int i, int j) {
        if (j == M) return 1;
        if (i == N) return solve(0, j+1);
        if (table[i][j]) return solve(i+1, j);
        // macaron 1x1
        int s = solve(i+1, j);
        // 1x2 horizontal
        if (j < M-1) {
            table[i][j+1] = true;
            s += solve(i+1, j);
            table[i][j+1] = false;
        }
        // 2x1 vertical
        if (i < N-1 && !table[i+1][j])
            s += solve(i+2, j);
        return s % MOD;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        N = in.nextInt();
        M = in.nextLong();
        table = new boolean[N][(int)M];
        in.close();
        System.out.println(solve(0, 0));
    }
}

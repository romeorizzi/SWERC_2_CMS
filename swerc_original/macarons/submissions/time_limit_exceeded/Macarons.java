// Jean-Christophe
// fast exponentiation replaced by naive multiplication
// ==> takes too much time

import java.util.Scanner;

class Main {

    // all computations modulo M
    static final int M = 1000000000;

    static long[][] mul(long[][] a, long[][] b) {
        int n = a.length; // square matrices only
        long[][] c = new long[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++) {
                    c[i][j] += a[i][k] * b[k][j];
                    c[i][j] %= M;
                }
        return c;
    }

    static long[][] id(int s) {
        long[][] mat = new long[s][s];
        for (int i = 0; i < s; i++)
            mat[i][i] = 1;
        return mat;
    }

    static long[][] pow(long[][] a, long n) {
        long[][] b = id(a.length);
        for (int k = 0; k < n; k++)
            b = mul(a, b);
        return b;
    }

    static long[] fib;

    static boolean bit(int x, int i) { return (x & (1 << i)) != 0; }

    // transition matrix: states are bit masks representing the transition
    // between two columns on the grid table, with 1 indicating the middle
    // of a long macaron and 0 indicating a space between two distinct macarons
    static long transition(int n, int x, int y) {
        long c = 1;
        int i = 0;
        while (i < n) {
            if (bit(x, i) && bit(y, i)) return 0l;
            if (bit(x, i) || bit(y, i)) { i++; continue; }
            int len = 0; // maximal 0-run on both sides
            while (i < n && !bit(x, i) && !bit(y, i)) { i++; len++; }
            c *= fib[len + 1];
        }
        return c;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        long m = in.nextLong();
        in.close();
        fib = new long[n+2];
        fib[1] = 1;
        for (int i = 2; i < n+2; i++)
            fib[i] = fib[i-1] + fib[i-2];
        int s = 1 << n;
        long[][] mat = new long[s][s];
        for (int i = 0; i < s; i++)
            for (int j = 0; j < s; j++)
                mat[i][j] = transition(n, i, j);
        long[][] mat_m  = pow(mat, m);
        System.out.println(mat_m[0][0]);
    }
}

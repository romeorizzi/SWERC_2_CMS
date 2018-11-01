// Jean-Christophe

import java.util.Scanner;

class Frosting {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        int m = 3*n; // 0 modulo 3
        long[] w = new long[3]; // widths
        for (int i = 1; i <= n; i++)
            w[i % 3] += in.nextInt();
        long[] a = new long[3];; // areas
        for (int j = 1; j <= n; j++) {
            int b = in.nextInt();
            a[0] += b * w[(m  -j) % 3];
            a[1] += b * w[(m+1-j) % 3];
            a[2] += b * w[(m+2-j) % 3];
        }
        System.out.format("%d %d %d\n", a[0], a[1], a[2]);
    }
}

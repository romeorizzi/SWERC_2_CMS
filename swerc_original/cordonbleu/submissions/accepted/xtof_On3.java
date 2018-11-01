/* swerc - c.durr - 2017

   Cordon Bleu

   implementation in O(n^3)
*/

import java.util.Scanner;

class xtof_On3 {

  static final long INT_MAX = Long.MAX_VALUE;

  final int nx;
  final int ny;
  final long[][] costs;

  long solve() {
    final int xy[] = new int[nx];
    final long lx[] = new long[nx];
    final boolean s[] = new boolean[nx];
    for (int x = 0; x < nx; x++) {
      xy[x] = -1;
    }
    final int yx[] = new int[ny];
    final long ly[] = new long[ny];
    final int augmenting[] = new int[ny];
    final long slack1[] = new long[ny];
    final int slack2[] = new int[ny];
    for (int y = 0; y < ny; y++) {
      yx[y] = -1;
      ly[y] = 0;
      slack1[y] = 0;
      slack2[y] = 0;
    }
    for (int x = 0; x < nx; x++) {
      long max = 0;
      for (int y = 0; y < ny; y++) {
        if (costs[x][y] > max)
          max = costs[x][y];
      }
      lx[x] = max;
    }
    for (int root = 0; root < nx; root++) {
      for (int y = 0; y < ny; y++)
        augmenting[y] = -1;
      for (int x = 0; x < nx; x++)
        s[x] = false;
      s[root] = true;
      for (int y = 0; y < ny; y++) {
        slack1[y] = lx[root] + ly[y] - costs[root][y];
        slack2[y] = root;
      }
      int y = -1;
      for (;;) {
        long delta = INT_MAX;
        int  x = -1;
        for (int yy = 0; yy < ny; yy++)
          if (augmenting[yy] == -1)
            if (slack1[yy] < delta) {
              delta = slack1[yy];
              x = slack2[yy];
              y = yy;
            }
        assert(s[x]);
        if (delta > 0) {
          for (int xx = 0; xx < nx; xx++)
            if (s[xx])
              lx[xx] -= delta;
          for (int yy = 0; yy < ny; yy++)
            if (augmenting[yy] > -1)
              ly[yy] += delta;
            else
              slack1[yy] -= delta;
        }
        assert(lx[x] + ly[y] == costs[x][y]);
        augmenting[y] = x;
        x = yx[y];
        if (x == -1)
          break;
        s[x] = true;
        for (int yy = 0; yy < ny; yy++)
          if (augmenting[yy] == -1) {
            long first = lx[x] + ly[yy] - costs[x][yy];
            if (slack1[yy] > first) {
              slack1[yy] = first;
              slack2[yy] = x;
            }
          }
      }
      while (y > -1) {
        int x = augmenting[y];
        int prec = xy[x];
        yx[y] = x;
        xy[x] = y;
        y = prec;
      }
    }
    long total = 0;
    for (int x = 0; x < nx; x++)
      total += lx[x];
    for (int y = 0; y < ny; y++)
      total += ly[y];
    return total;
  }

  public xtof_On3(int nx, int ny) {
    this.nx = nx;
    this.ny = ny;
    this.costs = new long[nx][ny];
  }

  public static long dist(long x1, long y1, long x2, long y2) {
    return Math.abs(x1 - x2) + Math.abs(y1 - y2);
  }

  public static void main(String args[]) {
    final Scanner in = new Scanner(System.in);
    final int nb_bottles = in.nextInt();
    final int nb_couriers = in.nextInt();
    final long bottle_x[] = new long[nb_bottles];
    final long bottle_y[] = new long[nb_bottles];
    final long courier_x[] = new long[nb_couriers];
    final long courier_y[] = new long[nb_couriers];
    for (int i=0; i < nb_bottles; i++) {
      bottle_x[i] = in.nextInt();
      bottle_y[i] = in.nextInt();
    }
    for (int i=0; i < nb_couriers; i++) {
      courier_x[i] = in.nextInt();
      courier_y[i] = in.nextInt();
    }
    final long lab_x = in.nextInt();
    final long lab_y = in.nextInt();
    final int n = nb_couriers + nb_bottles - 1;
    final xtof_On3 G = new xtof_On3(nb_bottles, n);
    for (int j=0; j < nb_bottles; j++) {
      final long d_bl = dist(bottle_x[j], bottle_y[j], lab_x, lab_y);
      for (int i=0; i<nb_couriers; i++) {
        final long d_cb = dist(courier_x[i], courier_y[i], bottle_x[j], bottle_y[j]);
        G.costs[j][i] = - d_cb - d_bl;
      }
      for (int i=nb_couriers; i < n; i++) {
        G.costs[j][i] = -2 * d_bl;
      }
    }
    System.out.println( -G.solve() );
  }

}

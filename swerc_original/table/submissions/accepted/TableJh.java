import java.util.Scanner;
import java.util.Stack;

class Pair {
    public int X, Y;
    public Pair(int X, int Y) { this.X = X; this.Y = Y; }
}

public class TableJh {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);

        int X = in.nextInt();
        int Y = in.nextInt();
        int N = in.nextInt();
        int D = in.nextInt();

        boolean[][] table = new boolean[X][Y+1];
        int[][] counts = new int[X+1][Y+1];

        for(int i = 0; i < N; i++) {
            int x1 = in.nextInt();
            int x2 = in.nextInt();
            int y1 = in.nextInt();
            int y2 = in.nextInt();
            for(int x = x1; x < x2; x++)
                for(int y = y1; y < y2; y++)
                    table[x][y] = true;
        }

        for(int x = 0; x < X; x++) table[x][Y] = true;

        int[] hist = new int[Y+1];
        for(int x = 0; x < X; x++) {
            Stack<Pair> stk = new Stack<Pair>();
            stk.push(new Pair(0, -1));
            for(int y = 0; y <= Y; y++) {
                hist[y] = table[x][y] ? 0 : hist[y]+1;
                int y0 = y;
                while(stk.peek().X > hist[y]) {
                    y0 = stk.peek().Y;
                    counts[stk.peek().X][y - y0]++;
                    stk.pop();
                    counts[Math.max(stk.peek().X, hist[y])][y - y0]--;
                }
                stk.push(new Pair(hist[y], y0));
            }
        }

        for(int x = X; x >= 1; x--) {
            for(int y = Y-1; y >= 1; y--)
                counts[x][y] += counts[x][y+1];
            for(int y = Y-1; y >= 1; y--)
                counts[x][y] += counts[x][y+1];
        }
        for(int x = X-1; x >= 1; x--)
            for(int y = Y; y >= 1; y--)
                counts[x][y] += counts[x+1][y];

        for(int i = 0; i < D; i++) {
            int x = in.nextInt();
            int y = in.nextInt();
            System.out.println(counts[x][y]);
        }
    }
}

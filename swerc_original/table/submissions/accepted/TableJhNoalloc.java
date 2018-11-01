import java.util.Scanner;
import java.util.Stack;

public class TableJhNoalloc {
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
        int[] stkx = new int[Y+2];
        int[] stky = new int[Y+2];
        int stktop;
        for(int x = 0; x < X; x++) {
            stktop = 1;
            stkx[0] = 0;
            stky[0] = -1;
            for(int y = 0; y <= Y; y++) {
                hist[y] = table[x][y] ? 0 : hist[y]+1;
                int y0 = y;
                while(stkx[stktop-1] > hist[y]) {
                    stktop--;
                    y0 = stky[stktop];
                    counts[stkx[stktop]][y - y0]++;
                    counts[Math.max(stkx[stktop-1], hist[y])][y - y0]--;
                }
                stkx[stktop]=hist[y];
                stky[stktop]=y0;
                stktop++;
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

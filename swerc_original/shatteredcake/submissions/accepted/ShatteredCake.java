
import java.util.Scanner;

class ShatteredCake {

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        long W = in.nextLong();
        int N = in.nextInt();
        long area = 0;
        while (N-- > 0) {
            int w = in.nextInt();
            int l = in.nextInt();
            area += w * l;
        }
        in.close();
        System.out.println(area / W);
    }
}

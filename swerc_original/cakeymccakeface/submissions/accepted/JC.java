// Jean-Christophe
// NOTE: reads a single problem
// assumes both input lists to be sorted in increasing order

import java.util.*;

class JC {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int N = in.nextInt();
        int M = in.nextInt();
        int[] sent = new int[N];
        for (int i = 0; i < N; i++)
            sent[i] = in.nextInt();
        HashMap<Integer, Integer> counts = new HashMap<>();
        int best_count = 0, best_offset = 0;
        int start = -1;
        while (M-- > 0) {
            int rcv = in.nextInt();
            while (start < N-1 && sent[start+1] <= rcv)
                start++;
            for (int i = 0; i <= start; i++) {
                int ofs = rcv - sent[i];
                assert ofs >= 0;
                int c = counts.containsKey(ofs) ? counts.get(ofs) : 0;
                counts.put(ofs, ++c);
                if (c > best_count || c == best_count && ofs < best_offset) {
                    best_count = c;
                    best_offset = ofs;
                }
            }
        }
        in.close();
        System.out.println(best_offset);
    }
}

// Local Variables:
// compile-command: "javac Radar.java && java Main < radar01.in"
// End:

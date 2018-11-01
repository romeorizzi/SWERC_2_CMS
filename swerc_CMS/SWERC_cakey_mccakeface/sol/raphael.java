// Author: Raphael

import java.util.*;

class raphael {

    static HashMap<Integer, Integer> m = new HashMap<>();

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
	int N = in.nextInt();
	int M = in.nextInt();
	int []A = new int[N];
	for (int i = 0; i < N; ++i) {
	    A[i] = in.nextInt();
	}
	for (int i = 0; i < M; ++i) {
	    int b = in.nextInt();
	    for (int j = 0; j < N; ++j) {
		int diff = b - A[j];
		if (diff >= 0) {
		    if (m.get(diff) == null)
			m.put(diff, 1);
		    else
			m.put(diff, m.get(diff) + 1);
		}
	    }
	}
	int res = 0;
	int best = 0;
	for (Map.Entry<Integer, Integer> entry : m.entrySet()) {
	    if (best < entry.getValue() ||
		(best == entry.getValue() && res > entry.getKey())) {
		best = entry.getValue();
		res = entry.getKey();
	    }
	}
        System.out.println(res);
    }
}

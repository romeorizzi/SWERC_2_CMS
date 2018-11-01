// Jean-Christophe

import java.util.*;

class Point {
    final long x, y;
    Point(Scanner in) { this.x = in.nextInt(); this.y = in.nextInt(); }
    public String toString() { return x + "," + y; }
    static int sign(double d) {
        return Math.abs(d) < 1e-9 ? 0 : d < 0 ? -1 : +1;
    }

    // on which side of (p1p2) is point p3?
    // returns -1 : p3 is on the right side of (p1p2)
    //          0 : p3 is on (p1p2)
    //         +1 : p3 is on the left side of (p1p2)
    static int crossProduct(Point p1, Point p2, Point p3) {
        double r = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);
        return sign(r);
    }

    // distance from point p3 to line (p1p2)
    static double distance(Point p1, Point p2, Point p3) {
        // (p1p2) has equation ax - by + c = 0 with
        double a = p2.y - p1.y, b = p2.x - p1.x;
        double c = p2.x * p1.y - p2.y * p1.x;
        return Math.abs(a * p3.x - b * p3.y + c) / Math.sqrt(a*a + b*b);
    }
}

// to compare 2 points wrt to the angle they make with a third point p
// in case of equality, sort in increasing distance
class AngleComparator implements Comparator<Point> {
    private Point p;
    public AngleComparator(Point p) { this.p = p; }
    @Override
        public int compare(Point p1, Point p2) {
        int res = Point.crossProduct(p, p2, p1);
        if (res == 0)
            return Point.sign((p.x-p1.x)*(p.x-p1.x) + (p.y-p1.y)*(p.y-p1.y) -
                             ((p.x-p2.x)*(p.x-p2.x) + (p.y-p2.y)*(p.y-p2.y)));
        else return res;
    }
}

class BlowingCandles {
    static int N, R;
    static Point[] points;

    // Graham algorithm
    static Vector<Point> convexHull() {
        assert points.length >= 3;
        // find the (leftmost) bottom point
        Point p = points[0];
        for (Point q: points)
            if (q.y < p.y || q.y == p.y && q.x < p.x)
                p = q;
        // sort points according to the angle they make with p
        AngleComparator c = new AngleComparator(p);
        Arrays.sort(points, c);
        assert points[0] == p; // OK to use == here
        // build the hull
        Vector<Point> h = new Vector<Point>();
        h.add(p);
        h.add(points[1]);
        for (int i = 2; i < points.length; i++) {
            while (h.size() >= 2 &&
                   Point.crossProduct(h.get(h.size() - 2), h.get(h.size() - 1), points[i]) <= 0)
                h.remove(h.size() - 1);
            h.add(points[i]);
        }
        return h;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        N = in.nextInt();
        R = in.nextInt();
        points = new Point[N];
        for (int i = 0; i < N; i++) points[i] = new Point(in);
        in.close();
        Vector<Point> hull = convexHull();
        int s = hull.size();
        // assert s >= 3; // ??
        // System.err.println(s + " points on the hull");
        Point last = hull.get(s - 1), first = hull.get(0);
        // start with side first--last and finds furthest point
        double minw = 0.;
        int opposite = 0;
        for (int i = 0; i < s; i++) {
            double w = Point.distance(last, first, hull.get(i));
            if (w > minw) { minw = w; opposite = i; }
        }
        // then traverse all sides of the hull, in order,
        for (Point p: hull) {
            double w = Point.distance(last, p, hull.get(opposite));
            while (true) { // and move opposite point as needed
                int next = (opposite + 1) % s;
                double d = Point.distance(last, p, hull.get(next));
                if (d <= w) break;
                w = d; opposite = next;
            }
            minw = Math.min(w, minw);
            last = p;
        }
        System.out.println(minw);
    }
}

/*
  Local Variables:
  compile-command: "javac BlowingCandles.java && java -ea Main < test.in"
  End:
*/

/* swerc 2017 - christoph dürr

Blowing candles

solution in time O(N log N)
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

typedef long long INT;
typedef pair<INT, INT> Point;

bool left_turn(Point a, Point b, Point c) {
    return (a.first - c.first) * (b.second - c.second) -
           (a.second - c.second) * (b.first - c.first) > 0;
}


double dist2(Point p0, Point p1, Point p2) {
    /* squared distance from point p0 to line (p1,p2)
    https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points
    */
    INT x0 = p0.first;
    INT y0 = p0.second;
    INT x1 = p1.first;
    INT y1 = p1.second;
    INT x2 = p2.first;
    INT y2 = p2.second;
    double a = ((y2 - y1)*x0 - (x2 - x1)*y0 + x2 * y1 - y2 * x1);
    double b = (y2 - y1)*(y2 - y1) + (x2 - x1)*(x2 - x1);
    return a * a / b;
}


void andrew(vector<Point> &S) {
    sort(S.begin(), S.end());
    vector<Point> top, bot;
    int t = 0, b = 0;
    for (Point p : S) {
        while (t >= 2 && ! left_turn(p, top[t - 1], top[t - 2])) {
            top.pop_back();
            t--;
        }
        top.push_back(p);
        t++;
        while (b >= 2 && ! left_turn(bot[b - 2], bot[b - 1], p)) {
            bot.pop_back();
            b--;
        }
        bot.push_back(p);
        b++;
    }
    S.clear();
    for (int i = 0; i < bot.size() - 1; i++)
        S.push_back(bot[i]);
    for (int i = top.size() - 1; i > 0; i--)
        S.push_back(top[i]);
}

double solve(vector<Point> &S) {
    andrew(S);
    double best;
    int n = S.size();
    int b = 0;
    for (int a=0; a < n; a++) {
        int a1 = (a - 1 + n) % n;
        int b1 = (b + 1) % n;
        double d = dist2(S[b], S[a], S[a1]);
        double d1 = dist2(S[b1], S[a], S[a1]);
        while (d < d1) {
            b = b1;
            b1 = (b + 1) % n;
            d = d1;
            d1 = dist2(S[b1], S[a], S[a1]);
        }
        if (a == 0 or d < best)
            best = d;
    }
    return sqrt(best);    // compute just once sqrt.
}

int main() {
    int n, radius;
    cin >> n >> radius;
    vector<Point> S;
    S.push_back(Point(0,0));
    for (int i = 0; i < n; ++i)
    {
        INT x, y;
        cin >> x >> y;
        S.push_back(Point(x, y));
    }
    cout.precision(8);
    cout << solve(S) << endl;
    return 0;
}

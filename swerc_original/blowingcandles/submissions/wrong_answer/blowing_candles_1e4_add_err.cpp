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

const int MAX = 3500000;
//              3453484
INT x[MAX];
INT y[MAX];
int S[MAX];
int n;
int top[MAX];
int bot[MAX];

bool left_turn(int a, int b, int c) {
    return (x[a] - x[c]) * (y[b] - y[c]) -
           (y[a] - y[c]) * (x[b] - x[c]) > 0;
}


double dist2(int p0, int p1, int p2) {
    /* squared distance from point p0 to line (p1,p2)
    https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points
    */
    double a = ((y[p2] - y[p1])*x[p0] - (x[p2] - x[p1])*y[p0] + x[p2] * y[p1] - y[p2] * x[p1]);
    double b = y[p2] - y[p1];
    double c = x[p2] - x[p1];
    return a * a / (b * b + c * c);
}

bool lexico(int i, int j) {
    return x[i] < x[j] || (x[i] == x[j] && y[i] < y[j]); }

void print() {
    for (int i = 0; i < n; ++i)
    {
        printf("%lld %lld\n", x[S[i]], y[S[i]]);
    }
}

void andrew() {
    sort(S, S + n, lexico);
    int t = 0, b = 0;
    for (int i = 0; i < n; ++i)
    {
        int p = S[i];
        while (t >= 2 && ! left_turn(p, top[t - 1], top[t - 2]))
            t--;
        top[t++] = p;
        while (b >= 2 && ! left_turn(bot[b - 2], bot[b - 1], p))
            b--;
        bot[b++] = p;
    }
    n = 0;
    for (int i = 0; i < b - 1; i++)
        S[n++] = bot[i];
    for (int i = t - 1; i > 0; i--)
        S[n++] = top[i];
}

double solve() {
    andrew();
    double best;
    int b = 1;
    for (int a=0; a < n; a++) {
        int a1 = (a + 1) % n;
        int b1 = (b + 1) % n;
        double d = dist2(S[b], S[a], S[a1]);
        double d1 = dist2(S[b1], S[a], S[a1]);
        while (d < d1) {
            b = b1;
            d = d1;
            b1 = (b + 1) % n;
            d1 = dist2(S[b1], S[a], S[a1]);
        }
        if (a == 0 || d < best)
            best = d;
    }
    return sqrt(best);    // compute just once sqrt.
}

int main() {
    int radius;
    scanf("%d%d", &n, &radius);
    for (int i = 0; i < n; ++i) {
        scanf("%lld%lld", &x[i], &y[i]);
        S[i] = i;
    }
    printf("%.8f\n", solve()+1e-4);
    return 0;
}

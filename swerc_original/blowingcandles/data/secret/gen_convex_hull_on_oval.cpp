/* swerc 2017 - christoph dürr

Blowing candles

generates an instance in convex position with points close to a circle

g++ -std=c++11 gen_convex_hull_on_oval.cpp -o gen_convex_hull_on_oval

*/

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cmath>

using namespace std;


typedef long long INT;

INT *x;
INT *y;
int *S;
long long n;
int *top;
int *bot;

void print() {
    // printf("n=%li\n", n);
    // for (int i = 0; i < n; ++i)
    // {
    //     printf("%lld %lld\n", x[S[i]], y[S[i]]);
    // }
}

bool left_turn(int a, int b, int c) {
    return (x[a] - x[c]) * (y[b] - y[c]) -
           (y[a] - y[c]) * (x[b] - x[c]) > 0;
}


bool lexico(int i, int j) {
    return x[i] < x[j] || (x[i] == x[j] && y[i] < y[j]); }


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


int main(int argc, char*argv[]) {
    assert(argc==2);
    n = atoll(argv[1]);
    long R = n;
    S = new int[n];
    top = new int[n];
    bot = new int[n];
    x = new INT[n];
    y = new INT[n];
    for (INT i = 0; i < n; ++i)
    {
        S[i] = i;
        x[i] = i;
        double yy = sqrt( n * n - i * i );
        double y0 = floor(yy);
        double y1 = ceil(yy);
        if (yy - y0 < y1 - yy)
            y[i] = y0;
        else
            y[i] = y1;
    }
    andrew();
    cout << 4 * n << " " << R + 1 << endl;
    for (int i=0; i < n; i++)
    {
        int p = S[i];
        cout <<  x[p] << " " <<  y[p] << endl;
        cout << -x[p] << " " << -y[p] << endl;
        cout <<  x[p] << " " << -y[p] << endl;
        cout << -x[p] << " " <<  y[p] << endl;
    }
    return 0;
}

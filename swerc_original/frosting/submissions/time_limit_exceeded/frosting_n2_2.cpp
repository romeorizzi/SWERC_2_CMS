/* Solution C Ioana; compile bien avec g++ aussi ;) )*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAXN 100000
int n;
int a[MAXN+1];
long long results[3];

int main()
{
  int i, j, b;
    scanf("%d",&n);
    for (i = 1; i<=n; ++i) 
        scanf("%d",&a[i]); 
    for (j = 1; j<=n; ++j) {
        scanf("%d",&b);
        for(int c = 0 ; c < 3 ; c++ )
          {
            long long res = 0;
            for(i=(j*2+c)%3+1 ; i <= n ; i+=3)
              res += a[i] ;
            results[c] += res * b;
          }
      }
    printf("%lld %lld %lld\n", results[2], results[0], results[1]);
    return 0;
}


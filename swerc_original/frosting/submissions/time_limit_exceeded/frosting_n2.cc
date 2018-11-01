/* Solution C Ioana; compile bien avec g++ aussi ;) )*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAXN 100000
int n;
int a[MAXN+1];
int b[MAXN+1];
long results[3] ;

int main()
{
    int i, j, m;
    scanf("%d",&n);
    for (i = 1; i<=n; ++i) 
        scanf("%d",&a[i]); 
    for (i = 1; i<=n; ++i) {
        scanf("%d",&b[i]);
    }
    for(j=1 ; j <= n ; j++ )
      {
        long long res = 0;
        for(int c = 0 ; c < 3 ; c++ )
          {
            res = 0;
            for(i=(2+c+2*j)%3+1 ; i <= n ; i+=3)
              res += a[i] ;
            results[c] += res * b[j] ;
          }
      }
    printf("%ld %ld %ld\n", results[0], results[1], results[2]);
    return 0;
}


#include <cstdio>
#include <vector>

using namespace std;

int N, M;
const unsigned int size = 1<<16, mask = size-1 ;
int * sent,* recv ;
unsigned int  count[size] ;
vector<unsigned int> bucket[size] ;

int * read(int l)
{
  int * t = new int[l];
  for(int i = 0 ; i < l ; i++)
    scanf("%d",t+i);
  return t;
}
int main()
{
  scanf("%d %d",&N,&M);
  sent=read(N);
  recv=read(M);
  for(int  i = 0 ; i < M; i++)
    for(int j = 0 ; j < N  && sent[j]<=recv[i]; j++) {
      const unsigned int m = recv[i]-sent[j] ;
      bucket[m>>16].push_back(m&mask);
    }
  unsigned int max = 0, maxi = 0 ;
  for(size_t b = 0 ; b < size ; b++ ) {
    for(auto cur : bucket[b]) {
      unsigned int & c = count[cur] ;
      ++c ;
      if(c>=max ) {
        const unsigned int new_best=(b<<16) + cur;
        if(maxi>new_best || c>max)
          maxi = new_best ;
        max = c ;
      }
    }
    for(auto cur : bucket[b])
      count[cur] = 0 ;
  }
  printf("%d\n",maxi);
}

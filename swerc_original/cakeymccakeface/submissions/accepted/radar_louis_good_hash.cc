#include <cstdio>
#include <vector>
#include <unordered_map>

using namespace std;

int N, M, *sent, *recv;
unordered_map<int,int> diff;
pair<int,int> best;

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
    for(int j = 0 ; j < N  && sent[j]<=recv[i]; j++)
      diff[recv[i]-sent[j]]++;
  for(auto p : diff)
    if(p.second > best.second || (p.second==best.second && p.first<best.first))
      best=p;
  printf("%d\n",best.first);
}

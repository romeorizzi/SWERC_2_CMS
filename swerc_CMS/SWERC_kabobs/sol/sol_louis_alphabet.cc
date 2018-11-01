#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <algorithm>

using namespace std;

char rules [100], candy[100] ;
int alphabet_size ;
const int A = 64, M = 10000000 ;

struct automaton
{
  int (*trans)[A] ;
  bool * is_final ;
  int nb_states ;

  automaton (int n):nb_states(n)
  {
    is_final = new bool[nb_states] ;
    trans = new int[nb_states][A] ;
  }

  void destroy()
  {
    delete[] trans;
    delete[] is_final;
  }
} ;

automaton create_from(int beg, int end)
{
  automaton res(end-beg-1) ;
  int decal = 0 ;
  int fail = 0 ;
  for(int i = beg ; i < end-1 ; i++)
      {
        const int cur = i-beg ;
        if(rules[i]=='>')
          {
            decal = 1;
            fail = cur ;
          }
        res.is_final[cur] = (decal==0) ;
        for( int c = 0 ; c < alphabet_size ; c++)
          if( candy[c]==rules[i+decal] )
            res.trans[cur][c] = (cur+1)%res.nb_states ;
          else
            {
              res.trans[cur][c] = fail ;// in case cur==fail
              res.trans[cur][c] = res.trans[fail][c] ;
              // in case cur!=fail and trans[c][fail] == fail+1
            }
      }
  return res ;
}  

automaton product(automaton a, automaton b)
{
  int * remap = new int [ a.nb_states * b.nb_states] ;
  int * map = new int [ a.nb_states * b.nb_states] ;
  fill(remap,remap+a.nb_states * b.nb_states,-1);
  fill(map,map+a.nb_states * b.nb_states,-1);
  remap[0] = 0; // va de #a×#a à <nb_remap
  map[0] = 0 ; // map va de < nb_remap à #a×#b
  int nb_map = 1;

  for(int s  = 0 ; s < nb_map ; s++ )
    {
      const int s_a = map[s] / b.nb_states ;
      const int s_b = map[s] % b.nb_states ;
      
      for(int c = 0 ; c < alphabet_size ; c++ )
        {
          const int ns = a.trans[s_a][c]*b.nb_states + b.trans[s_b][c] ;
          if(remap[ns] < 0)
            {
              map[nb_map] = ns ;
              remap[ns] = nb_map++ ;
            }
        }
    }
  automaton res(nb_map) ;
  for(int s  = 0 ; s < nb_map ; s++ )
    {
      const int s_a = map[s] / b.nb_states ;
      const int s_b = map[s] % b.nb_states ;
      for(int c = 0 ; c < alphabet_size ; c++ )
        res.trans[s][c] = remap[a.trans[s_a][c]*b.nb_states + b.trans[s_b][c]] ;
      res.is_final[s] = a.is_final[s_a] && b.is_final[s_b] ;
    }
  
  delete[] map;
  delete[] remap;
  a.destroy();
  b.destroy();
  return res ;
}

automaton compute_automaton(int beg, int end)
{
  for(int i = beg ; i < end ; i++)
    if(rules[i] == '|')
      return product(create_from(beg,i), compute_automaton(i+1,end)) ;
  return create_from(beg,end) ;
}

int count(automaton a, int size)
{
  long long * valid[2] ;
  valid[0] = new long long[a.nb_states] ;
  valid[1] = new long long[a.nb_states] ;
  fill(valid[0],valid[0]+a.nb_states,0);
  valid[0][0] = 1 ;
  for(int step = 0 ; step < size ; step++)
    {
      const int cur = step%2;
      const int nxt = (step+1)%2 ;
      for(int s = 0 ; s < a.nb_states ; s++)
        {
          valid[cur][s] %= M ;
          valid[nxt][s] = 0 ;
        }
      for(int s = 0 ; s < a.nb_states ; s++)
        for(int c = 0 ; c < alphabet_size ; c++)
          valid[nxt][a.trans[s][c]] += valid[cur][s] ;
    }
  int res = 0 ;
  for(int i = 0 ; i < a.nb_states ; i++ )
    if(a.is_final[i])
      res = (res+valid[size%2][i]) % M ;
  delete[] valid[0] ;
  delete[] valid[1] ;
  return res;
}

int main()
{
  int size ;
  scanf("%d %s\n%s\n",&size,candy,rules);
  alphabet_size = strlen(candy) ;
  automaton res = compute_automaton(0,strlen(rules));
  printf("%d\n",count(res,size));
  res.destroy();
  return 0;
}

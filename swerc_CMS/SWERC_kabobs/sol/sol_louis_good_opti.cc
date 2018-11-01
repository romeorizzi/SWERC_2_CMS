#include <cstdio>
#include <string.h>
#include <algorithm>
#include <vector>

using namespace std;

char rules [100], candy[100] ;
int alphabet_size ;
const int modulo = 10000000 ;

struct automaton
{
  vector<vector< pair<char,int> >> trans ;
  vector<bool> is_final ;
  int nb_states ;

  automaton( const int size) : nb_states(size)
  {
    is_final.resize(nb_states) ;
    trans.resize(nb_states) ;
  }
} ;

void list_trans(vector<pair<char,int> > & a, vector<pair<char,int> > & b, const int mult,vector<pair<char,int> > & res )
{
  res.clear();
  auto x = a.begin(), y = b.begin();
  while( x != a.end() || y != b.end())
    {
      if(  x != a.end() && y != b.end() && x->first == y->first)
        {
          res.push_back( make_pair(x->first, x->second*mult+y->second));
          ++x; ++y ;
        }
      else
        if( y == b.end() || (x != a.end() && x->first < y->first))
          {
            res.push_back( make_pair(x->first, x->second*mult+b[0].second));
            ++ x;
          }
        else
          {
            res.push_back( make_pair(y->first, a[0].second*mult+y->second));
            ++ y;
          }
    }
}

automaton product(automaton a, automaton b)
{
  vector<int> remap(a.nb_states * b.nb_states,-1), map(a.nb_states * b.nb_states,-1) ;
  remap[0] = 0; // va de #a×#a à <nb_remap
  map[0] = 0 ; // map va de < nb_remap à #a×#b
  int nb_map = 1;
  automaton res(0) ;
  for(int s  = 0 ; s < nb_map ; s++ )
    {
      res.trans.push_back(vector<pair<char,int> >());
      list_trans(a.trans[map[s]/b.nb_states],b.trans[map[s] % b.nb_states], b.nb_states, res.trans[s]);
      for( auto & tr : res.trans[s] )
        {
          if(remap[tr.second] < 0)
            {
              map[nb_map] = tr.second ;
              remap[tr.second] = nb_map++ ;
            }
          tr.second = remap[tr.second];
        }
      res.is_final.push_back(a.is_final[map[s]/b.nb_states] && b.is_final[map[s]%b.nb_states]);
    }
  res.nb_states = nb_map;
  return res ;
}

automaton compute_automaton(int beg, int end)
{
  for(int i = beg ; i < end ; i++)
    if(rules[i] == '|')// case of multiple rule
      return product(compute_automaton(beg,i), compute_automaton(i+1,end)) ;
  
  automaton res (end-beg-1) ;
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
      res.trans[cur].push_back(make_pair( 0, fail)) ; // 0 is the default transition
      res.trans[cur].push_back( make_pair(rules[i+decal], (cur+1)%res.nb_states) );
      if( res.trans[fail][1].first != rules[i+decal])//if we fail a match then we might 
        res.trans[cur].push_back( res.trans[fail][1] );
      sort(res.trans[cur].begin(),res.trans[cur].end()); // we need default trans in [0]
    }
  return res ;
}

int count(const automaton a, int size)
{
  vector<int> cur(a.nb_states,0), nxt (a.nb_states,0) ;
  for(int i = 0 ; i < a.nb_states ; i++ )
    if(a.is_final[i])
      cur[i]=1;
  for(int step = 0 ; step < size ; step++)
    {
      for(int s = 0 ; s < a.nb_states ; s++)
        {
          long long val = cur[a.trans[s][0].second]*(alphabet_size-a.trans[s].size()+1);
          for(size_t i = 1 ; i < a.trans[s].size() ; i++)
            val += cur[a.trans[s][i].second] ;
          nxt[s] = val%modulo ;
        }
      cur.swap(nxt);
    }
  return cur[0];
}

int main()
{
  int size ;
  scanf("%d %s\n%s\n",&size,candy,rules);
  alphabet_size = strlen(candy) ;
  automaton res = compute_automaton(0,strlen(rules));
  printf("%d\n",count(res,size));
  return 0;
}

#include <cstdio>
#include <cstdlib>
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
  int get_tr()
  {
    int r=0;
    for(int i =0;i<nb_states;i++)
      r+=trans[i].size();
    return r;
  }
  
} ;

automaton create_from(int beg, int end)
{
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

void list_trans(automaton & a, automaton & b, const int s_a, const int s_b, vector<pair<char,int> > & res )
{
  res.clear();
  for( auto t_a : a.trans[s_a] )
    {
      bool found = false ;
      for( auto & t_b : b.trans[s_b] )
        if(t_a.first == t_b.first )
          {
            res.push_back(make_pair(t_a.first, t_a.second*b.nb_states+t_b.second));
            found = true ;
          }
      if(!found)
        res.push_back( make_pair(t_a.first,t_a.second*b.nb_states+b.trans[s_b][0].second));
    }
  
  for(auto t_b : b.trans[s_b])
    {
      bool found = false ;
      for( auto t_a : a.trans[s_a] )
        if(t_a.first == t_b.first)
          found = true ;
      if(!found)
        res.push_back(make_pair(t_b.first,a.trans[s_a][0].second*b.nb_states+t_b.second));
    }
  sort(res.begin(),res.end());
}


automaton product(automaton a, automaton b)
{
  vector<int> remap(a.nb_states * b.nb_states,-1), map(a.nb_states * b.nb_states,-1) ;
  remap[0] = 0; // va de #a×#a à <nb_remap
  map[0] = 0 ; // map va de < nb_remap à #a×#b
  int nb_map = 1;

  vector< pair<char,int> > r ;
  for(int s  = 0 ; s < nb_map ; s++ )
    {
      list_trans(a,b,map[s] / b.nb_states,map[s] % b.nb_states, r);
      for( auto tr : r )
        if(remap[tr.second] < 0)
          {
            map[nb_map] = tr.second ;
            remap[tr.second] = nb_map++ ;
          }
    }
  automaton res(nb_map) ;
  for(int s  = 0 ; s < nb_map ; s++ )
    {
      list_trans(a,b,map[s] / b.nb_states,map[s] % b.nb_states, res.trans[s]);
      for(auto & t : res.trans[s])
        t.second = remap[t.second] ;
      res.is_final[s] = a.is_final[map[s]/b.nb_states] && b.is_final[map[s]%b.nb_states] ;
    }
  return res ;
}

automaton compute_automaton(int beg, int end)
{
  int i = beg ;
  while(++i != end)
    if(rules[i] == '|')
      return product(create_from(beg,i), compute_automaton(i+1,end)) ;
  return create_from(beg,end) ;
}

int count(automaton a, int size)
{
  vector<long long> cur(a.nb_states,0), nxt (a.nb_states,0) ;
  nxt[0]=1;
  for(int step = 0 ; step < size ; step++)
    {
      cur=nxt;
      for(int s = 0 ; s < a.nb_states ; s++)
        {
          cur[s] %= modulo ;
          nxt[s] = 0 ;
        }
      for(int s = 0 ; s < a.nb_states ; s++)
        {
          for(auto tr : a.trans[s] )
            nxt[tr.second] += cur[s] ;
          nxt[a.trans[s][0].second] += cur[s] * (alphabet_size-a.trans[s].size()) ;
        }
    }
  int res = 0 ;
  for(int i = 0 ; i < a.nb_states ; i++ )
    if(a.is_final[i])
      res = (res+nxt[i]) % modulo ;
  return res;
}

int main()
{
  int size ;
  scanf("%d %s\n%s\n",&size,candy,rules);
  alphabet_size = strlen(candy) ;
  automaton res = compute_automaton(0,strlen(rules));
  //  printf("%d %d\n",res.nb_states,res.get_tr());
  printf("%d\n",count(res,size));
  return 0;
}

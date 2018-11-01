#include <cstdio>
#include <map>
#include <vector>

using namespace std;

typedef unsigned int ui;
map<ui,vector<ui>> todo;
ui N, M, *sent, *recv, * nb_s, * nb_r, *assoc;
ui best, best_score, cur_score;

void read(ui & l, ui ** data, ui **mult)
{
  *data = new ui[l+1];
  *mult = new ui[l+1];
  ui written = 0;
  for(ui read = 0 ; read < l ; read++)  {
    scanf("%u",(*data)+written);
    if(written>0 && (*data)[written-1]==(*data)[written])
      (*mult)[written-1]++;
    else {
      (*mult)[written]=1;
      written++;
    }
  }
  l=written;
}

int main()
{
  scanf("%u %u",&N,&M);
  read(N,&sent,&nb_s);
  read(M,&recv,&nb_r);
  recv[M] = recv[M-1]+sent[N-1] ;
  assoc=new ui[N+1];
  assoc[0]=0;
  for(ui i = 0 ; i < N ; i++) {
    while(sent[i]>recv[assoc[i]])
      assoc[i]++;
    if(assoc[i]!=M)
      todo[recv[assoc[i]]-sent[i]].push_back(i);
    assoc[i+1]=assoc[i];
  }
  
  while(!todo.empty()) {
    const auto cur = todo.begin();
    cur_score=0;
    for(auto i : cur->second) {
      cur_score+=nb_r[assoc[i]]*nb_s[i];
      assoc[i]++;
      if(assoc[i]!=M)
        todo[recv[assoc[i]]-sent[i]].push_back(i);
    }
    
    if(cur_score>best_score) {
      best_score=cur_score;
      best=cur->first;
    }
    todo.erase(cur);
  }  
  printf("%d\n",best);
}

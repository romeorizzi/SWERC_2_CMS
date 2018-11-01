#include <cstdio>

typedef unsigned int ui;
ui N, M, *sent, *recv, * nb_s, * nb_r, *assoc;
ui best, best_score;

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
  assoc=new ui[N];
  for(ui i = 0 ; i < N ; i++)
    assoc[i]=0;
  ui diff = 0;
  while(assoc[0] != M) {
    ui score = 0, nxt_diff = recv[M];
    for(ui i = 0 ; i < N && assoc[i] != M; i++ ) {
      //increase assoc[i] such that sent[i]+diff<=recv[assoc[i]]
      while(sent[i]+diff>recv[assoc[i]])
        assoc[i]++;
      
      if(sent[i]+diff==recv[assoc[i]]) {
        score+=1;
	// We don't need to use nb_s nb_r, because problem statement guarantees there are no dups:
	//nb_s[i]*nb_r[assoc[i]];
        assoc[i]++;
      }
      const ui cur_diff = recv[assoc[i]]-sent[i];
      if(cur_diff < nxt_diff)
        nxt_diff = cur_diff;
    }

    if(score>best_score) {
      best_score=score;
      best=diff;
    }
    diff=nxt_diff;
  }  
  printf("%d\n",best);
}

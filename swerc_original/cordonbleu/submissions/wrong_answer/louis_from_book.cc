#include <cstdio>
#include <algorithm>

using namespace std;

int infty=1000*1000*1000 ; 
// Computes a bipartite matching with minimum cost.
const int MAX_L = 2010, MAX_R = 2010 ;
pair<int,int> bottle[MAX_L], courrier[MAX_R] ;
int L, R; // input: size of the sides of the bipartite graph
int cost[MAX_L][MAX_R]; // input: the graph,
// set cost[l][r] = infty if edge l->r doesn't exist

bool adj[MAX_L][MAX_R];
bool reachedL[MAX_L], reachedR[MAX_R];
bool matchedL[MAX_L], visitedL[MAX_L];
// output: matchR : r in R is matched with matchR[r] in L
int matchR[MAX_R], slackL[MAX_L], slackR[MAX_R];
bool explore(int l) {
  reachedL[l] = 1;
  visitedL[l] = 1;
  for(int r = 0; r < R; r++)
    if(adj[l][r]) {
      reachedR[r] = 1;
      if(matchR[r] < 0 || (!visitedL[matchR[r]] && explore(matchR[r]))) {
        matchR[r] = l;
        return 1;
      }
    }
  return 0;
}

bool bm() {
  fill_n(visitedL, L, 0);
  for(int l = 0; l < L; l++)
    if(!matchedL[l] && explore(l)) {
      fill_n(visitedL, L, 0);
      matchedL[l] = 1;
    }
  for(int l = 0; l < L; l++)
    if(!matchedL[l])
      return 0;
  return 1;
}

// Returns the weight of the match
int weighted_bm() {
  fill_n(matchR, R, -1);
  fill_n(matchedL, L, 0);
  fill_n(*adj, MAX_R*MAX_L, 0);
  fill_n(slackR, R, 0);
  
  for(int l = 0; l < L; l++) {
    int rmin = 0;
    for(int r = 1; r < R; r++)
      if(cost[l][rmin] > cost[l][r])
        rmin = r;
    slackL[l] = cost[l][rmin];
    adj[l][rmin] = 1;
  }
  
  for(;;) {
    fill_n(reachedL, L, 0);
    fill_n(reachedR, R, 0);
    
    if(bm())
      break;
    
    int m = infty;
    for(int l = 0; l < L; l++)
      for(int r = 0; r < R; r++)
        if(reachedL[l] && !reachedR[r] && !adj[l][r])
          m = min(m, cost[l][r] - slackL[l] - slackR[r]);
    
    for(int l = 0; l < L; l++)
      if(reachedL[l])
        slackL[l] += m;
    for(int r = 0; r < R; r++)
      if(reachedR[r])
        slackR[r] -= m;
    
    for(int l = 0; l < L; l++)
      for(int r = 0; r < R; r++)
        if((reachedL[l] && !reachedR[r]) || (!reachedL[l] && reachedR[r]))
          adj[l][r] = slackL[l] + slackR[r] == cost[l][r];
  }
  
  int sum = 0;
  for(int r = 0; r < R; r++)
    if(matchR[r] >= 0)
      sum += cost[matchR[r]][r];
  return sum;
}

int dist(pair<int,int> a,pair<int,int> b) {return abs(a.first-b.first)+abs(a.second-b.second);}
int main()
{
  scanf("%d %d",&L,&R);
  for(int i = 0 ; i < L ; i++)
    scanf("%d %d",&bottle[i].first,&bottle[i].second);
  for(int i = 0 ; i < R ; i++)
    scanf("%d %d",&courrier[i].first,&courrier[i].second);
  scanf("%d %d",&courrier[R].first,&courrier[R].second);
  for(int i = R+1 ; i < L+R ; i++)
    courrier[i]=courrier[R];
  R+=L-1;
  for(int r = 0 ; r < R ; r++)
    for(int l = 0 ; l < L ; l++ )
      cost[l][r] = dist(courrier[r],bottle[l])+dist(courrier[R],bottle[l]) ;
  printf("%d\n",weighted_bm());
  /*
  int s =0;
  for(int r = 0 ; r < R ; r++)
    if(matchR[r]>=0)
      printf("%d %d\n",matchR[r],r),s+= dist(courrier[r],bottle[matchR[r]])+dist(courrier[R],bottle[matchR[r]]);
  printf("%d\n",s);
  */
}

#include <stdio.h>
#include <stdlib.h>

#define MAXN 1000
#define MAXM 5000
#define lad '|'
#define emptyslot '-'

int n,m;
int candy[MAXN][MAXM];
int sumcandyleft[MAXN][MAXM];
int sumcandyright[MAXN][MAXM];
int closestcandyleft[MAXN][MAXM];
int closestcandyright[MAXN][MAXM];

int upladders[MAXN+1][10];
int countupladders[MAXN+1];


int result[10][10];
int prevresult[10][10];

int finalresult;

void addupladder(int shelf, int coord) {
    upladders[shelf][countupladders[shelf]] = coord;
    countupladders[shelf]++;
}

void readData()
{
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int i,j;
    scanf("%d %d", &n, &m);
    read = getline(&line, &len, stdin);
    for (i = 0; i<n;++i) {
        //shelf
        read = getline(&line, &len, stdin);
        for (j = 0; j<m; ++j) {
            if (line[j] != emptyslot) 
                candy[i][j] = line[j]-48;
            else
                candy[i][j] = 0;
        }      
        //ladders
        read = getline(&line, &len, stdin);       
        for (j = 0; j<m; ++j) if (line[j] == lad) 
            addupladder(i+1,j);
    }
}

int getmaxcandybetween(int shelf, int l1, int l2)
{
    int aux, candyintersect, totalcandy, cright, cleft, intersect, end;
    if (l1>l2) {aux = l1; l1 = l2; l2 = aux;}
    totalcandy = 0;
    if (closestcandyleft[shelf][l1]!=l1 && closestcandyleft[shelf][l1]!=-1) totalcandy += candy[shelf][closestcandyleft[shelf][l1]];
    if (closestcandyright[shelf][l2]!=l2 && closestcandyright[shelf][l2]!=-1) totalcandy += candy[shelf][closestcandyright[shelf][l2]];
    
    totalcandy+=sumcandyleft[shelf][l2]-sumcandyleft[shelf][l1]+candy[shelf][l1];
    return totalcandy;       
}

int getmaxcandy(int shelf, int l1, int l2, int pl1, int pl2) {
    int aux, candyintersect, totalcandy, cright, cleft, intersect, end;
  
    if (l1>pl1) {aux = l1; l1 = pl1; pl1 = aux;}
    if (l2>pl2) {aux = l2; l2 = pl2; pl2 = aux;}
    if (l1>l2) {aux = l1; l1 = l2; l2 = aux; aux = pl1; pl1 = pl2; pl2 = aux;}

    totalcandy = 0;
    end = pl2;
    if (pl1>pl2) end = pl1;
        
    if (l2<=pl1) {//intersect 
        end = pl1;
        if (pl2<pl1) end = pl2;
        intersect = sumcandyleft[shelf][end]-sumcandyleft[shelf][l2]+candy[shelf][l2];
        if (intersect>0) 
            return -1;
    } else 
        intersect = 0;
    
    
    totalcandy+=sumcandyleft[shelf][pl1]-sumcandyleft[shelf][l1]+candy[shelf][l1];
    totalcandy+=sumcandyleft[shelf][pl2]-sumcandyleft[shelf][l2]+candy[shelf][l2];
    totalcandy-=intersect;
    return totalcandy;
} 

void solve()
{
    int i, j, precsum, closest, k, pi, pj, max;
    
    finalresult = 0;
    
    for (i = 0; i<n; ++i) {
        precsum = 0;
        closest = -1;
        for (j = 0; j<m; ++j) {
            precsum = precsum + candy[i][j];
            sumcandyleft[i][j] = precsum;
            if (candy[i][j])
                closest = j;
            closestcandyleft[i][j] = closest;
        }
        precsum = 0;
        closest = -1;
        for (j = m-1; j>=0; j--) {
            precsum = precsum + candy[i][j];
            sumcandyright[i][j] = precsum;
            if (candy[i][j])
                closest = j;
            closestcandyright[i][j] = closest;    
        }
    }
    
    for (k = 1; k<=n; ++k) {
       for (i = 0; i<countupladders[k]; ++i)
         for (j = 0; j<countupladders[k]; ++j) {
            if ( k == 1)
                result[i][j] = 0;
            else {
                result[i][j] = -1;
                for (pi = 0; pi<countupladders[k-1]; ++pi)
                    for (pj = 0; pj<countupladders[k-1]; ++pj) if (prevresult[pi][pj]!=-1) {
                        max = getmaxcandy(k-1, upladders[k][i], upladders[k][j], upladders[k-1][pi], upladders[k-1][pj]);
                        if (max != -1 && prevresult[pi][pj]+max > result[i][j])
                            result[i][j] = max+prevresult[pi][pj];
                    }
            }
            if (result[i][j]!=-1) {
                max = k==n? 0: getmaxcandybetween(k, upladders[k][i], upladders[k][j]);
                if (result[i][j]+max > finalresult)
                    finalresult = result[i][j]+max;
            }
        }        
        for (i = 0; i<countupladders[k]; ++i)
         for (j = 0; j<countupladders[k]; ++j)
            prevresult[i][j] = result[i][j];
    }
        
    printf("%d\n", finalresult);       
}

int main()
{
    readData();
    solve();
    return 0;
}




   

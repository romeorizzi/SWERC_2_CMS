#include <cstdio>
#include <map>

using namespace std;

int frontTimes[2000];
map<int, int> diffs;

int main() {
  int nbFront, nbExit;
  scanf("%d%d", &nbFront, &nbExit);
  for (int f = 0; f < nbFront; f++)
    scanf("%d", &frontTimes[f]);
  pair<int, int> best = make_pair(-1, 1);
  for (int e = 0; e < nbExit; e++) {
    int exitTime;
    scanf("%d", &exitTime);
    for (int f = 0; f < nbFront; f++) {
      int d = exitTime - frontTimes[f];
      if (d >= 0) {
        diffs[d]++;
        best = max(best, make_pair(diffs[d], -d));
      }
    }
  }
  printf("%d\n", -best.second);
  return 0;
}

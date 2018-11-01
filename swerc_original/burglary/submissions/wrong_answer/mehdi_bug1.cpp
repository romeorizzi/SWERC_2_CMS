#include <cstdio>
#include <algorithm>

using namespace std;

const int MAX_WIDTH = 5000;
const int MAX_SHELVES = 1000;
const int MAX_LADDERS = 10;
const int MIN_INFTY = - MAX_WIDTH * MAX_SHELVES * 10;

int width, nbShelves;
int candies[MAX_SHELVES][MAX_WIDTH];
int cumulCandies[MAX_SHELVES][MAX_WIDTH+1];
int lastCandy[MAX_SHELVES][MAX_WIDTH];
int nextCandy[MAX_SHELVES][MAX_WIDTH];
int nbLadders[MAX_SHELVES];
int ladderSlot[MAX_SHELVES+1][MAX_LADDERS];

int _funAndProfit[MAX_SHELVES][MAX_LADDERS][MAX_LADDERS];

int funAndProfit(int shelf, int ladLeft, int ladRight) {
  if (shelf >= nbShelves)
    return 1;
  if (_funAndProfit[shelf][ladLeft][ladRight] != 0)
    return _funAndProfit[shelf][ladLeft][ladRight];
  int curLeftSlot = ladderSlot[shelf][ladLeft];
  int curRightSlot = ladderSlot[shelf][ladRight];
  int nbLad = nbLadders[shelf+1];
  int best = cumulCandies[shelf][curRightSlot+1] - cumulCandies[shelf][curLeftSlot];
  int lastCandyLeft = lastCandy[shelf][curLeftSlot];
  if (lastCandyLeft >= 0)
    best += candies[shelf][lastCandyLeft];
  int nextCandyRight = nextCandy[shelf][curRightSlot];
  if (nextCandyRight >= 0)
    best += candies[shelf][nextCandyRight];
  best++;
  for (int nextLeft = 0; nextLeft < nbLad; nextLeft++) {
    int nextLeftSlot = ladderSlot[shelf+1][nextLeft];
    int minLeft = min(curLeftSlot, nextLeftSlot);
    int maxLeft = max(curLeftSlot, nextLeftSlot);
    int candiesLeft = cumulCandies[shelf][maxLeft+1] - cumulCandies[shelf][minLeft];
    int nextCandyLeft = nextCandy[shelf][maxLeft];
    lastCandyLeft = lastCandy[shelf][minLeft];
    for (int nextRight = nextLeft; nextRight < nbLad; nextRight++) {
      int nextRightSlot = ladderSlot[shelf+1][nextRight];
      int minRight = min(curRightSlot, nextRightSlot);
      if (maxLeft >= minRight) {
        int candiesOnIntersect = cumulCandies[shelf][maxLeft+1] - cumulCandies[shelf][minRight];
        if (candiesOnIntersect > 0)
          continue;
      }
      int collected = funAndProfit(shelf + 1, nextLeft, nextRight);
      int maxRight = max(curRightSlot, nextRightSlot);
      int lastCandyRight = lastCandy[shelf][minRight];
      nextCandyRight = nextCandy[shelf][maxRight];
      int candiesRight = cumulCandies[shelf][maxRight+1] - cumulCandies[shelf][minRight];
      collected += candiesLeft + candiesRight;
      if (lastCandyLeft >= 0 && !candies[shelf][minLeft])
        collected += candies[shelf][lastCandyLeft];
      if (nextCandyRight >= 0 && !candies[shelf][maxRight])
        collected += candies[shelf][nextCandyRight];
      if (nextCandyLeft >= 0 && nextCandyLeft < minRight) {
        if (!candies[shelf][maxLeft]) {
            collected += candies[shelf][nextCandyLeft];
            if (lastCandyRight >= 0 && lastCandyRight > nextCandyLeft && !candies[shelf][minRight])
              collected += candies[shelf][lastCandyRight];
        } else {
          if (lastCandyRight >= 0 && lastCandyRight > maxLeft && !candies[shelf][minRight])
            collected += candies[shelf][lastCandyRight];
        }
      }
      best = max(best, collected);
    }
  }
  _funAndProfit[shelf][ladLeft][ladRight] = best;
  return best;
}

int readChar() {
  for (;;) {
    int c = getchar();
    if (c == '-' || c == '.')
      return 0;
    if (c == '|')
      return 1;
    if (c >= '1' && c <= '9')
      return c - '0';
  }
}

int main(void) {

  scanf("%d%d", &nbShelves, &width);

  nbLadders[0] = 1;

  for (int shelf = 0; shelf < nbShelves; shelf++) {
    int last = -1;
    for (int slot = 0; slot < width; slot++) {
      lastCandy[shelf][slot] = last;
      int cand = readChar();
      candies[shelf][slot] = cand;
      cumulCandies[shelf][slot+1] = cumulCandies[shelf][slot] + cand;
      if (cand > 0)
        last = slot;
    }
    int next = -1;
    for (int slot = width - 1; slot >= 0; slot--) {
      nextCandy[shelf][slot] = next;
      if (candies[shelf][slot] > 0)
        next = slot;
    }
    for (int slot = 0; slot < width; slot++)
      if (readChar()) {
        ladderSlot[shelf+1][nbLadders[shelf+1]] = slot;
        nbLadders[shelf+1]++;
      }
  }

  printf("%d\n", funAndProfit(0, 0, 0) - 1);

  return 0;
}

#include<cstdlib>
#include<cstdio>

const int SZ = 2000;
bool table[SZ][SZ];

int main() {
  srandom(42);
  int X, Y, N, Lx, Ly, Dx, Dy;
  scanf("%d%d%d%d%d%d%d", &X, &Y, &Lx, &Ly, &N, &Dx, &Dy);

  printf("%d %d %d %d\n", X, Y, N, ((Dx-1)/3+1) * ((Dy-1)/4+1));

  for(int i = 0; i < N; i++) {
    for(;;) {
      int dx, x1, x2, dy, y1, y2;
      dx = random()%Lx+1;
      dy = random()%Ly+1;
      x1 = random()%(X - dx + 1);
      x2 = x1 + dx;
      y1 = random()%(Y - dy + 1);
      y2 = y1 + dy;
      bool ok = true;
      for(int x = x1; ok && x < x2; x++)
        for(int y = y1; ok && y < y2; y++)
          if(table[x][y]) ok = false;
      if(ok) {
        for(int x = x1; ok && x < x2; x++)
          for(int y = y1; ok && y < y2; y++)
            table[x][y] = true;
        printf("%d %d %d %d\n", x1, x2, y1, y2);
        break;
      }
    }
  }

  for(int x = 1; x <= Dx; x+=3)
    for(int y = 1; y <= Dy; y+=4)
      printf("%d %d\n", x, y);

  return 0;
}

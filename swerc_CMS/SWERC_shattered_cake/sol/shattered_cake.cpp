#include <iostream>

int W;
int L;
long N;
long A;

int main(int, char**){
  std::ios::sync_with_stdio(false);
  std::cin >> W >> N;
  A = 0;
  for(int i=0; i<N; i++){
    int w,l;
    std::cin >> w >> l;
    A += w*l;
  }
  L = A/W;
  std::cout << L;
}

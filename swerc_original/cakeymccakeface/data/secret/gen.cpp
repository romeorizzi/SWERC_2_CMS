// Author: Raphael

#include <algorithm>
#include <assert.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const int MIN_TIMESTAMP = 0;
const int MAX_TIMESTAMP = 1000*1000*1000;
//const int MAX_TIMESTAMP = 10*1000;

using int64 = long long;

void PrintVector(std::vector<int> &v) {
  for (int i = 0; i < v.size(); ++i) {
    if (i)
      std::cout << " ";
    std::cout << v[i];
  }
  std::cout << std::endl;
}

std::vector<int> GenerateRandomSet(int num_elements, int element_bound) {
  assert(num_elements >= 0);
  assert(element_bound >= num_elements);
  std::set<int> ret;
  while (ret.size() < num_elements) {
    // Not efficient when element_bound and num_elements are close, but hey.
    int e = rand() % (element_bound + 1);
    ret.insert(e);
  }
  return std::vector<int>(ret.begin(), ret.end());
}

std::vector<int> GenerateRandomSet2(int num_elements,
				    int element_min,
				    int element_max) {
  assert(num_elements >= 0);
  std::set<int> ret;
  while (ret.size() < num_elements) {
    // Not efficient when element_bound and num_elements are close, but hey.
    int e = element_min + rand() % (element_max - element_min + 1);
    ret.insert(e);
  }
  return std::vector<int>(ret.begin(), ret.end());
}

// Generates a pair of sets where the total number of coincidences
// (summed over all possible shifts) is O(num_elements^2).
std::pair<std::vector<int>, std::vector<int>>
GenerateSetsWithK2PossibleShifts(int num_elements, int element_bound) {
  assert(static_cast<int64>(num_elements) * num_elements * 11 < element_bound);
  assert(num_elements >= 0);
  assert(element_bound >= num_elements);
  std::vector<int> S1, S2;
  for (int i = 0; i < num_elements; ++i) {
    S1.push_back(100 * i + rand() % 100);
  }
  for (int i = 0; i < num_elements; ++i) {
    //    S2.push_back(S1[i] + i * 5 + rand() % 5);
    S2.push_back(num_elements * 100 * i +
		 rand() % (num_elements * 100)); // optional
  }
  return std::make_pair(S1, S2);
}

int main() {
  srand(time(0));
  int K, L;
  std::cin >> K >> L;
  std::vector<int> S1, S2;
  
  //  S1 = GenerateRandomSet(K, MAX_TIMESTAMP);
  //  S2 = GenerateRandomSet(L, MAX_TIMESTAMP);
  

  assert(K == L);
  std::tie(S1, S2) =
    GenerateSetsWithK2PossibleShifts(K, MAX_TIMESTAMP);
  
  //  S1 = GenerateRandomSet2(K, 500*1000*1000, 500*1000*1000 + 3000);
  //  S2 = GenerateRandomSet2(K, 900*1000*1000, 900*1000*1000 + 3000);

  std::cout << K << "\n" << L << "\n";
  PrintVector(S1);
  PrintVector(S2);
}

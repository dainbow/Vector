#include "../include/main.hpp"
#include <vector>

int main() {
  Vector<int> vector;

  vector.push_back(228);
  vector.push_back(777);

  for (uint64_t cur_idx = 0; cur_idx < vector.size(); cur_idx++) {
    std::cout << vector[cur_idx] << " ";
  }
  std::cout << std::endl;

  Vector<bool> vect;

  vect.push_back(1);
  vect.push_back(0);
  vect.push_back(1);

  vect[1] = 1;

  for (uint64_t cur_idx = 0; cur_idx < vect.size(); cur_idx++) {
    std::cout << vect[cur_idx] << " ";
  }
  std::cout << std::endl;
}

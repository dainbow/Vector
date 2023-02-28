#include "../include/main.hpp"

#include <vector>

int main() {
  Vector<int> vector;

  vector.push_back(228);
  vector.push_back(777);

  std::vector<bool> vect;

  auto it = vect.begin();

  std::_Bit_iterator'

  std::iterator_traits<std::vector<bool>::iterator>::pointer;

  for (auto& elem : vector) {
    std::cout << elem << " ";
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

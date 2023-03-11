#include "../include/main.hpp"

#include <vector>

int main() {
  {
    std::vector<uint64_t, PageAllocator<uint64_t>> vec;

    vec.push_back(12);

    for (auto& elem : vec) {
      std::cout << elem << std::endl;
    }
  }

  Vector<int, StackMemory> vector;

  vector.push_back(777);
  vector.push_back(228);

  std::sort(vector.begin(), vector.end());
  for (auto& elem : vector) {
    Print("% ", elem);
  }
  std::cout << std::endl;

  Vector<bool> vect(70, 1);
  vect.push_back(0);

  std::sort(vect.begin(), vect.end());
  for (auto& elem : vect) {
    Print("% ", elem);
  }
  std::cout << std::endl;

  std::cout << std::find(vect.begin(), vect.end(), 1) - vect.begin()
            << std::endl;

  std::cout << std::endl;
}

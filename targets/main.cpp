#include "../include/main.hpp"

int main() {
  Vector<int> vector;

  vector.push_back(777);
  vector.push_back(228);

  std::sort(vector.begin(), vector.end());
  for (auto& elem : vector) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;

  Vector<bool> vect(70, 1);
  vect.push_back(0);

  std::sort(vect.begin(), vect.end());
  for (auto& elem : vect) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;

  std::cout << std::find(vect.begin(), vect.end(), 1) - vect.begin() << std::endl;


  std::cout << std::endl;
}

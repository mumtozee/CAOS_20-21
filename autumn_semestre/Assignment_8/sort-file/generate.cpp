#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>

int main() {
  std::ofstream ofs{};
  ofs.open("data.dat", std::ios::out | std::ios::binary);

  /*std::mt19937 gen{ std::random_device{}() };
  std::uniform_int_distribution<> dist (0, 14);
  size_t size = 15;
  int *array = new int[size];
  std::generate (array, array + size, [&dist, &gen] () { return dist (gen);
  });*/
  const size_t size = 15;
  int array[size] = {14, 0, 3, 6, 5, 8, 12, 13, 10, 9, 1, 4, 2, 7, 11};
  ofs.write(reinterpret_cast<char*>(array), size << 2);
  ofs.close();
  // delete array;
  return 0;
}
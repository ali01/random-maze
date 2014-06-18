#include "maze.h"

#include <iostream>

using std::cout;

int
main(int argc, char **argv) {
  RandomMaze maze(30, 19);
  cout << maze;
  return 0;
}

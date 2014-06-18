#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>

#include "maze.h"

using std::cout;
using std::invalid_argument;
using std::stoi;

void
print_usage(const char *program_name) {
  printf("\n");
  printf("usage: %s %s %s\n\n", program_name, "<num_rows>", "<num_cols>");
  printf("Options:\n");
  printf("  -h  show this help message and exit\n\n");
}

int
main(int argc, char **argv) {
  // parsing command line options
  int c_i;
  while ((c_i = getopt(argc, argv, "h")) != -1) {
    switch (c_i) {
      case 'h':
      case '?':
      default:
        print_usage(argv[0]);
        return 0;
    }
  }

  // parse non-option arguments; two expected
  if (optind + 2 != argc) {
    print_usage(argv[0]);
    return -1;
  }

  const int rows = stoi(argv[optind]);
  const int cols = stoi(argv[++optind]);

  if (rows < 1 || cols < 1) {
    printf("Error: the number of rows and columns must be greater than 0.\n");
    return -1;
  }

  try {
    // heavy lifting done by RandomMaze
    RandomMaze maze(rows, cols);

    // output maze to stdout
    cout << maze;

  } catch (const invalid_argument& e) {
    cout << "Error: " << e.what() << "\n";
    return -1;
  }

  return 0;
}

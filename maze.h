#ifndef __MAZE_H__
#define __MAZE_H__

#include <array>
#include <iostream>
#include <random>
#include <set>

using std::array;
using std::default_random_engine;
using std::ostream;
using std::set;
using std::vector;


class RandomMaze {
public:

  RandomMaze(unsigned int rows, unsigned int cols);

  // stream operator used to output the maze's ASCII representation
  friend ostream& operator<<(ostream &out, const RandomMaze& maze);

  ~RandomMaze();


private:

  // -- matrix cell definition --

  // Bitmasks to control a cell's flags;
  // Walls: only East and South status flags are necessary;
  // The status of a cell's N/W walls is defined by
  // the E/S walls of neighboring cells.
  enum { EAST_WALL = 0x1, SOUTH_WALL = 0x2, START_CELL = 0x4, DEST_CELL = 0x8 };

  struct cell_t {
    cell_t() : cell_flags(EAST_WALL|SOUTH_WALL) {}
    unsigned int row;
    unsigned int col;
    uint8_t cell_flags;
  };


  // -- private member functions --

  // maze generation

  void generate_maze();
  int disable_wall(cell_t *cell_a, cell_t *cell_b);

  array<cell_t*,4> cell_neighbors(int row, int col) const;
  bool is_within_bounds(int row, int col) const;

  // input/output

  ostream& output_row(ostream& out, int row) const;


  // -- data members --

  const unsigned int rows_;
  const unsigned int cols_;

  cell_t **matrix_; // two dimensional array
  set<cell_t*> cells_seen_; // cells visited during depth-first search
  default_random_engine prg_; // pseudo-random number generator


  // -- operations disallowed --
  RandomMaze(const RandomMaze&); // copy constructor
  void operator=(const RandomMaze&); //assignment operator
};

#endif

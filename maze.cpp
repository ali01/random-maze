#include "maze.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>

using std::array;
using std::chrono::system_clock;
using std::default_random_engine;
using std::ostream;
using std::set;
using std::shuffle;
using std::string;
using std::vector;


// -- construction --

RandomMaze::RandomMaze(unsigned int rows, unsigned int cols)
: rows_(rows), cols_(cols) {

  // initialize random number generator
  unsigned seed = system_clock::now().time_since_epoch().count();
  prg_ = default_random_engine(seed);

  // contiguous region of memory to efficiently contain
  // all cells of two-dimensional array
  cell_t *cells = new cell_t[rows * cols];

  matrix_ = new cell_t*[rows];
  for (int ix = 0; ix < rows; ++ix) {
    matrix_[ix] = &cells[ix * cols];
  }

  for (int ix = 0; ix < rows; ++ix) {
    for (int jx = 0; jx < cols; ++jx) {
      matrix_[ix][jx].row = ix;
      matrix_[ix][jx].col = jx;
    }
  }

  // set start and destination cells
  matrix_[rows_ - 1][0].cell_flags |= START_CELL;
  matrix_[0][cols_ - 1].cell_flags |= DEST_CELL;

  generate_maze();
}


RandomMaze::~RandomMaze() {
  delete[] matrix_[0]; // deallocating single contiguous block of cells
  delete[] matrix_;    // deallocating array of pointers to matrix rows
}


// -- public interface --

ostream&
operator<<(ostream& out, const RandomMaze& maze) {
  for (int row = maze.rows_ - 1; row >= 0; --row) {
    maze.output_row(out, row);
  }

  return out;
}


// -- private interface --

// maze generation

void
RandomMaze::generate_maze() {
  vector<cell_t*> cell_stack;
  cell_stack.push_back(&matrix_[0][0]);

  while (cell_stack.size() > 0) {
    cell_t *curr_cell = cell_stack.back();
    cell_stack.pop_back();

    auto neighbors = cell_neighbors(curr_cell->row, curr_cell->col);
    shuffle(neighbors.begin(), neighbors.end(), prg_);

    for (cell_t *neighbor : neighbors) {
      if (neighbor && cells_seen_.find(neighbor) == cells_seen_.end()) {
        // cell is unseen so far
        disable_wall(curr_cell, neighbor);
        cells_seen_.insert(neighbor);
        cell_stack.push_back(neighbor);
      }
    }
  }
}


int
RandomMaze::disable_wall(cell_t *cell_a, cell_t *cell_b) {
  if (cell_a == cell_b) {
    return -1; // error
  }

  if (cell_a->row == cell_b->row - 1 && cell_a->col == cell_b->col) {
    // cell_b is north of cell_a; disable cell_b's south wall
    cell_b->cell_flags &= ~SOUTH_WALL;

  } else if (cell_a->row == cell_b->row && cell_a->col == cell_b->col - 1) {
    // cell_b is east of cell_a; disable cell_a's east wall
    cell_a->cell_flags &= ~EAST_WALL;

  } else if (cell_a->row == cell_b->row + 1 && cell_a->col == cell_b->col) {
    // cell_b is south of cell_a; disable cell_a's south wall
    cell_a->cell_flags &= ~SOUTH_WALL;

  } else if (cell_a->row == cell_b->row && cell_a->col == cell_b->col + 1) {
    // cell_b is west of cell_a; disable cell_b's east wall
    cell_b->cell_flags &= ~EAST_WALL;

  } else {
    return -1; // error; cells are not neighboring each other
  }

  return 0;
}


array<RandomMaze::cell_t*,4>
RandomMaze::cell_neighbors(int row, int col) const {
  cell_t *N = is_within_bounds(row + 1, col) ? &matrix_[row + 1][col] : NULL;
  cell_t *E = is_within_bounds(row, col + 1) ? &matrix_[row][col + 1] : NULL;
  cell_t *S = is_within_bounds(row - 1, col) ? &matrix_[row - 1][col] : NULL;
  cell_t *W = is_within_bounds(row, col - 1) ? &matrix_[row][col - 1] : NULL;

  return {{ N, E, S, W }};
}


bool
RandomMaze::is_within_bounds(int row, int col) const {
  if (row < 0 || row >= rows_) {
    return false;
  }

  if (col < 0 || col >= cols_) {
    return false;
  }

  return true;
}



// input/output

ostream&
RandomMaze::output_row(ostream& out, int row) const {
  // north wall
  if (row + 1 == rows_) {
    for (int col = 0; col < cols_; ++col) {
      out << "+---";
    }

    out << "+\n";
  }

  // west wall
  out << "|";

  for (int col = 0; col < cols_; ++col) {
    const cell_t *cell = &matrix_[row][col];

    // cell contents
    if (cell->cell_flags & START_CELL) {
      out << " S ";
    } else if (cell->cell_flags & DEST_CELL) {
      out << " D ";
    } else {
      out << "   ";
    }

    // east wall
    string wall = (cell->cell_flags & EAST_WALL) ? "|" : " ";
    out << wall;
  }

  out << "\n";
  out << "+";

  // south wall
  for (int col = 0; col < cols_; ++col) {
    string wall = (matrix_[row][col].cell_flags & SOUTH_WALL) ? "---+" : "   +";
    out << wall;
  }

  out << "\n";

  return out;
}

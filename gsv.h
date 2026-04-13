#include "iostream"
#include<stdlib.h>
#include<string.h>
#include"time.h"
#include<fstream>
#include<math.h>
#include "Grid2D.h"
#include "Grid3D.h"

using namespace std;

int display_table(Grid2D& table, int size);
int display_options_table(Grid3D& options_table, int size, int add);

void write_file(Grid2D& table, int size, char* filename);
int m1_GENERAT_SUDOKU_METHOD(Grid3D& options_table,Grid2D& table,Grid2D& c_table,int size,int blk_size);
int m2_GENERAT_SUDOKU_METHOD(Grid3D& options_table,Grid2D& table,Grid2D& c_table,int size,int blk_size);
int m3_GENERAT_SUDOKU_METHOD(Grid3D& options_table,Grid2D& table,Grid2D& c_table,int size,int blk_size);
int m4_GENERAT_SUDOKU_METHOD(Grid3D& options_table, Grid2D& table, Grid2D& c_table, int size, int blk_size);
int display_multiple_solution_list(Grid3D& table, int size);

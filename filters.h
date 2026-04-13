#include <iostream>
#include<stdlib.h>
#include<string.h>
#include"time.h"
#include<fstream>
#include<iomanip>
#include<math.h>
#include"Grid2D.h"
#include"Grid3D.h"

using namespace std;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int build_options_table(Grid3D& p_OT, Grid2D& table, int size, int blk_size);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_symbol_cts_method(Grid3D& p_OT, Grid2D& p_table, Grid2D& p_temp_1, Grid2D& p_temp_2, int size, int blk_size, int max_set_size, int pid, int stat_data[]);
int find_hidden_symbol(Grid3D& p_OT_1, Grid2D& p_temp_2, int size, int blk_size, int pid, int stat_data[]);
int filter_block_symbol(Grid3D& p_OT, Grid2D& p_temp_2, int size, int blk_size, int pid, int stat_data[]);
int find_twin_set(Grid3D& p_OT_1, int size, int blk_size, int pid, int stat_data[]);
int find_n_symbol_n_cell_tally_dsc(Grid3D& p_OT, Grid2D& p_temp_1, Grid2D& p_temp_2, int size, int blk_size, int max_set_size, int pid, int stat_data[]);
int find_n_symbol_n_cell_tally_asc(Grid3D& p_OT, Grid2D& p_temp_1, Grid2D& p_temp_2, int size, int blk_size, int max_set_size, int pid, int stat_data[]);
int filter_symbol_sets_group_method(Grid3D& p_OT, Grid2D& p_temp_1, Grid2D& p_temp_2, int size, int blk_size, int max_set_size, int pid, int stat_data[]);
int remove_invalid_options_from_bound_cells(Grid3D& p_OT, Grid2D& p_temp_2, int size, int blk_size, int pid, int stat_data[]);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol(Grid3D& p_OT, Grid2D& p_table, Grid2D& s_table, Grid2D& p_temp_2, int size, int blk_size, int& pid, int stat_data[]);





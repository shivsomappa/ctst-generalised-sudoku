#include"filters.h"
using namespace std;

int insert_symbol_into_table(Grid3D& options_table, Grid2D& p_table, int size, int blk_size, int stat_data[]);
int insert_symbol_into_table_fx(Grid3D& options_table, int s, int x, int y, int size, int blk_size);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_symbol_cts_method(Grid3D& options_table, Grid2D& p_table, Grid2D& temp_1, Grid2D& temp_2, int size, int blk_size, int max_set_size, int pid, int stat_data[])
{
	int flag = 1;

	while (flag == 1)
	{
		flag = insert_symbol_into_table(options_table, p_table,size, blk_size, stat_data);
		if (flag == 0) { flag = find_hidden_symbol(options_table, temp_2, size, blk_size, pid, stat_data); }
		if (flag == 0) { filter_block_symbol(options_table, temp_2, size, blk_size, pid, stat_data); }
		if (flag == 0) { flag = find_twin_set(options_table, size, blk_size, pid, stat_data); }
		if (flag == 0) { flag = find_n_symbol_n_cell_tally_dsc(options_table, temp_1, temp_2, size, blk_size, max_set_size, pid, stat_data); }
		if (flag == 0) { flag = find_n_symbol_n_cell_tally_asc(options_table, temp_1, temp_2, size, blk_size, max_set_size, pid, stat_data); }
		if (flag == 0) { flag = filter_symbol_sets_group_method(options_table, temp_1, temp_2, size, blk_size, max_set_size, pid, stat_data); }
		if (flag == 0) { flag = remove_invalid_options_from_bound_cells(options_table, temp_2, size, blk_size, pid, stat_data); }
		
		if(pid==0)
		{
			cout <<"Celled filled by CTST method in initial stage            " << stat_data[3] << "  " << '\r';
			cout.flush();
		}
	}

	return flag;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int insert_symbol_into_table(Grid3D& options_table, Grid2D& p_table, int size, int blk_size, int stat_data[])
{
	int i = 0, j, s, flag = 1;

	while (flag == 1)
	{
		flag = 0; i = 0;
		for(i=0;i<size;i++)
		{
			for(j=0;j<size;j++)
			{
				if (options_table(i,j,0) == 1)
				{
					s = 1;
					while (options_table(i,j,s) == 0) { s++; }

					p_table(i,j) = s;
					options_table(i,j,s) = 0;
					options_table(i,j,0) = 0;
					stat_data[3]++;
					flag = 1; 

					insert_symbol_into_table_fx(options_table, s, i, j, size, blk_size);
				}
			}
		}
	}

	return 0;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int insert_symbol_into_table_fx(Grid3D& options_table, int s, int x, int y, int size, int blk_size)
{
	int i = 0, j = 0, bx = 0, by = 0, bz = 0;
	int pid = options_table(x,y,size + 1);


	//remove vertically
	for (i = 0; i < size; i++)
	{
		if (options_table(i,y,s) > 0)
		{

			options_table(i,y,s) = 0;
			options_table(i,y,0)--;
			options_table(i,y,size + 1) = pid;
		}
	}
	//remove horizontally
	for (i = 0; i < size; i++)
	{
		if (options_table(x,i,s) > 0)
		{
			options_table(x,i,s) = 0;
			options_table(x,i,0)--;
			options_table(x,i,size + 1) = pid;
		}
	}
	//remove from block
	bx = (x / blk_size) * blk_size; by = (y / blk_size) * blk_size;

	for (i = bx; i < bx + blk_size; i++)
	{
		for (j = by; j < by + blk_size; j++)
		{
			if (options_table(i,j,s) > 0)
			{
				options_table(i,j,s) = 0;
				options_table(i,j,0)--;
				options_table(i,j,size + 1) = pid;
			}
		}
	}
	return 0;
}

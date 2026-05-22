// =============================================================================
// P_filter_symbol_condition_7.cpp
// Bound-set filter (symbol-first approach).
//
// For each symbol, identifies cells where that symbol is "bound" to a small
// set of rows or columns within a block.  Removes the symbol from all other
// cells in those rows/columns that are outside the bounding block.
// Exposed entry point: remove_invalid_options_from_bound_cells()
// =============================================================================

#include"filters.h"

using namespace std;

int sort_symbol_count_dsc(Grid2D& work_buf, int size);

int find_bound_cells_hrz(Grid3D& opt_tbl,  Grid2D& work_buf, int x, int size, int block_size, int pred_id, int stats[]);
int find_cell_to_symbol_tally_hrz(Grid3D& opt_tbl,  Grid2D& work_buf, int x, int size, int pred_id, int stats[]);
int remove_invalid_symbols_hrz(Grid3D& opt_tbl,  Grid2D& work_buf, int s, int x, int size, int pred_id, int stats[]);

int find_bound_cells_vrt(Grid3D& opt_tbl,  Grid2D& work_buf, int y, int size, int block_size, int pred_id, int stats[]);
int find_cell_to_symbol_tally_vrt(Grid3D& opt_tbl,  Grid2D& work_buf, int y, int size, int pred_id, int stats[]);
int remove_invalid_symbols_vrt(Grid3D& opt_tbl,  Grid2D& work_buf, int s, int y, int size, int pred_id, int stats[]);

int find_bound_cells_blk(Grid3D& opt_tbl,  Grid2D& work_buf, int bx, int by, int size, int block_size, int pred_id, int stats[]);
int find_cell_to_symbol_tally_blk(Grid3D& opt_tbl,  Grid2D& work_buf, int bx, int by, int size, int block_size, int pred_id, int stats[]);
int remove_invalid_symbols_blk(Grid3D& opt_tbl,  Grid2D& work_buf, int s, int bx, int by, int size, int block_size, int pred_id, int stats[]);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int remove_invalid_options_from_bound_cells(Grid3D& opt_tbl,  Grid2D& work_buf, int size, int block_size, int pred_id, int stats[])
{
	int i = 0, j = 0, flag = 0;

	while (i < size && flag == 0)
	{
		flag = find_bound_cells_hrz(opt_tbl, work_buf, i, size, block_size, pred_id, stats);
		i++;
	}

	if (flag == 0)
	{
		i = 0;
		while (i < size && flag == 0)
		{
			flag = find_bound_cells_vrt(opt_tbl, work_buf, i, size, block_size, pred_id, stats);
			i++;
		}
	}

	if (flag == 0)
	{
		i = 0;
		while (i < size && flag == 0)
		{
			j = 0;
			while (j < size && flag == 0)
			{
				flag = find_bound_cells_blk(opt_tbl, work_buf, i, j, size, block_size, pred_id, stats);
				j = j + block_size;
			}
			i = i + block_size;
		}
	}

	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_bound_cells_hrz(Grid3D& opt_tbl,  Grid2D& work_buf, int x, int size, int block_size, int pred_id, int stats[])
{
	int i, j, k, c = 0, flag = 0;
	

	for (i = 0;i < size + 1;i++) { work_buf(i,0) = 0; work_buf(i,1) = 0; work_buf(i,2) = 0; work_buf(i,3) = 0;work_buf(i,4) = 0; }

	for (j = 0;j < size;j++)
	{

		if (opt_tbl(x,j,0) > 0)
		{
			c++;
			if (opt_tbl(x,j,size + 1) == pred_id) { flag = 1; }
			for (k = 1;k < size + 1;k++) { if (opt_tbl(x,j,k) > 0) { work_buf(k,0)++; work_buf(k,1) = k; } }
		}
	}

	if (c > 4 && flag==1)
	{
		sort_symbol_count_dsc(work_buf, size);
		flag = find_cell_to_symbol_tally_hrz(opt_tbl, work_buf, x, size, pred_id, stats);
	}
	else { flag = 0; }

	return flag;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_cell_to_symbol_tally_hrz(Grid3D& opt_tbl,  Grid2D& work_buf, int x, int size, int pred_id, int stats[])
{
	int i = 0, j, k = 0, end = 0, c = 0, flag = 0, s;


	while (work_buf(end,0) > 0 && end < size + 1) { end++; }

	while (i < end - 1 && flag == 0)
	{
		c = 0;
		s = work_buf(i,1);
		for (j = 0;j < size + 1;j++) { work_buf(j,2) = 0; work_buf(j,3) = 0; work_buf(j,4) = 0; }

		for (j = 0;j < size;j++)
		{
			if (opt_tbl(x,j,0) > 0)
			{
				if (opt_tbl(x,j,s) > 0)
				{
					for (k = 1;k < size + 1;k++) { if (opt_tbl(x,j,k) > 0) { work_buf(k,2) = k; } }
				}
				else
				{
					for (k = 1;k < size + 1;k++) { if (opt_tbl(x,j,k) > 0) { work_buf(k,3) = k; } }
				}
			}
		}

		for (j = 1;j < size + 1;j++)
		{
			if (work_buf(j,2) > 0 && work_buf(j,3) == 0) { c++; work_buf(j,4) = j; }
		}

		if (c == work_buf(i,0))
		{
			flag = remove_invalid_symbols_hrz(opt_tbl,  work_buf, s, x, size, pred_id, stats);
		}

		i++;

	}
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int remove_invalid_symbols_hrz(Grid3D& opt_tbl,  Grid2D& work_buf, int s, int x, int size, int pred_id, int stats[])
{
	int j, k, flag = 0;

	for (j = 0;j < size;j++)
	{
		if (opt_tbl(x,j,s) > 0)
		{
			for (k = 1;k < size + 1;k++)
			{
				if (opt_tbl(x,j,k) > 0 && work_buf(k,4) == 0)
				{
					opt_tbl(x,j,k) = 0;
					opt_tbl(x,j,0)--;
					opt_tbl(x,j,size + 1) = pred_id;
					stats[9]++;
					flag = 1;
				}
			}
		}
	}
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_bound_cells_vrt(Grid3D& opt_tbl,  Grid2D& work_buf, int y, int size, int block_size, int pred_id, int stats[])
{
	int i,  k, c = 0, flag = 0;
	
	for (i = 0;i < size + 1;i++) { work_buf(i,0) = 0; work_buf(i,1) = 0; work_buf(i,2) = 0; work_buf(i,3) = 0;work_buf(i,4) = 0; }

	for (i = 0;i < size;i++)
	{

		if (opt_tbl(i,y,0) > 0)
		{
			c++;
			if (opt_tbl(i,y,size + 1) == pred_id) { flag = 1; }
			for (k = 1;k < size + 1;k++) { if (opt_tbl(i,y,k) > 0) { work_buf(k,0)++; work_buf(k,1) = k; } }
		}
	}

	if (c > 4 && flag==1)
	{
		sort_symbol_count_dsc(work_buf, size);
		flag = find_cell_to_symbol_tally_vrt(opt_tbl,  work_buf, y, size, pred_id, stats);
	}
	else { flag = 0; }
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_cell_to_symbol_tally_vrt(Grid3D& opt_tbl,  Grid2D& work_buf, int y, int size, int pred_id, int stats[])
{
	int i, j = 0, k = 0, end = 0, c = 0, flag = 0, s;


	while (work_buf(end,0) > 0 && end < size + 1) { end++; }

	while (j < end - 1 && flag == 0)
	{
		c = 0;
		s = work_buf(j,1);
		for (i = 0;i < size + 1;i++) { work_buf(i,2) = 0; work_buf(i,3) = 0; work_buf(i,4) = 0; }

		for (i = 0;i < size;i++)
		{
			if (opt_tbl(i,y,0) > 0)
			{
				if (opt_tbl(i,y,s) > 0)
				{
					for (k = 1;k < size + 1;k++) { if (opt_tbl(i,y,k) > 0) { work_buf(k,2) = k; } }
				}
				else
				{
					for (k = 1;k < size + 1;k++) { if (opt_tbl(i,y,k) > 0) { work_buf(k,3) = k; } }
				}
			}
		}

		for (i = 1;i < size + 1;i++)
		{
			if (work_buf(i,2) > 0 && work_buf(i,3) == 0) { c++; work_buf(i,4) = i; }
		}

		if (c == work_buf(j,0))
		{
			flag = remove_invalid_symbols_vrt(opt_tbl, work_buf, s, y, size, pred_id, stats);
		}

		j++;

	}
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int remove_invalid_symbols_vrt(Grid3D& opt_tbl,  Grid2D& work_buf, int s, int y, int size, int pred_id, int stats[])
{
	int i, k, flag = 0;

	for (i = 0;i < size;i++)
	{
		if (opt_tbl(i,y,s) > 0)
		{
			for (k = 1;k < size + 1;k++)
			{
				if (opt_tbl(i,y,k) > 0 && work_buf(k,4) == 0)
				{
					opt_tbl(i,y,k) = 0;
					opt_tbl(i,y,0)--;
					opt_tbl(i,y,size + 1) = pred_id;
					stats[9]++;
					flag = 1;
				}
			}
		}
	}
	return flag;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_bound_cells_blk(Grid3D& opt_tbl,  Grid2D& work_buf, int bx, int by, int size, int block_size, int pred_id, int stats[])
{
	int i, j, k, c = 0, flag = 0;
	

	for (i = 0;i < size + 1;i++) { work_buf(i,0) = 0; work_buf(i,1) = 0; work_buf(i,2) = 0; work_buf(i,3) = 0;work_buf(i,4) = 0; }

	for (i = bx;i < bx + block_size;i++)
	{
		for (j = by;j < by + block_size;j++)
		{
			if (opt_tbl(i,j,0) > 0)
			{
				c++;
				if (opt_tbl(i,j,size + 1) == pred_id) { flag = 1; }
				for (k = 1;k < size + 1;k++) { if (opt_tbl(i,j,k) > 0) { work_buf(k,0)++; work_buf(k,1) = k; } }
			}
		}
	}

	if (c > 4 && flag==1)
	{
		sort_symbol_count_dsc(work_buf, size);
		flag = find_cell_to_symbol_tally_blk(opt_tbl, work_buf, bx, by, size, block_size, pred_id, stats);
	}
	else { flag = 0; }
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_cell_to_symbol_tally_blk(Grid3D& opt_tbl,  Grid2D& work_buf, int bx, int by, int size, int block_size, int pred_id, int stats[])
{
	int i, j = 0, k = 0, start = 0, end = 0, c = 0, flag = 0, s;


	while (work_buf(end,0) > 0 && end < size + 1) { end++; }

	while (start < end - 1 && flag == 0)
	{
		c = 0;
		s = work_buf(start,1);
		for (i = 0;i < size + 1;i++) { work_buf(i,2) = 0; work_buf(i,3) = 0; work_buf(i,4) = 0; }

		for (i = bx;i < bx + block_size;i++)
		{
			for (j = by;j < by + block_size;j++)
			{
				if (opt_tbl(i,j,0) > 0)
				{
					if (opt_tbl(i,j,s) > 0)
					{
						for (k = 1;k < size + 1;k++) { if (opt_tbl(i,j,k) > 0) { work_buf(k,2) = k; } }
					}
					else
					{
						for (k = 1;k < size + 1;k++) { if (opt_tbl(i,j,k) > 0) { work_buf(k,3) = k; } }
					}
				}
			}
		}

		for (i = 1;i < size + 1;i++)
		{
			if (work_buf(i,2) > 0 && work_buf(i,3) == 0) { c++; work_buf(i,4) = i; }
		}

		if (c == work_buf(start,0))
		{
			flag = remove_invalid_symbols_blk(opt_tbl,work_buf, s, bx, by, size, block_size, pred_id, stats);
		}

		start++;

	}
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int remove_invalid_symbols_blk(Grid3D& opt_tbl,  Grid2D& work_buf, int s, int bx, int by, int size, int block_size, int pred_id, int stats[])
{
	int i, j, k, flag = 0;

	for (i = bx;i < bx + block_size;i++)
	{
		for (j = by;j < by + block_size;j++)
		{
			if (opt_tbl(i,j,s) > 0)
			{
				for (k = 1;k < size + 1;k++)
				{
					if (opt_tbl(i,j,k) > 0 && work_buf(k,4) == 0)
					{
						opt_tbl(i,j,k) = 0;
						opt_tbl(i,j,0)--;
						opt_tbl(i,j,size + 1) = pred_id;
						stats[9]++;
						flag = 1;
					}
				}
			}
		}
	}
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sort_symbol_count_dsc(Grid2D& work_buf, int size)
{
	int i, j, k;

	for (i = 0;i < size;i++)
	{
		for (j = i + 1;j < size + 1;j++)
		{
			if (work_buf(i,0) < work_buf(j,0))
			{
				k = work_buf(i,0);work_buf(i,0) = work_buf(j,0);work_buf(j,0) = k;
				k = work_buf(i,1);work_buf(i,1) = work_buf(j,1);work_buf(j,1) = k;
			}
		}
	}
	return 0;
}


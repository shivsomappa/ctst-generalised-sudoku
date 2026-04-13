#include"filters.h"

using namespace std;

int sort_symbol_count_dsc(Grid2D& temp_2, int size);

int find_bound_cells_hrz(Grid3D& p_OT,  Grid2D& temp_2, int x, int size, int blk_size, int pid, int stat_data[]);
int find_cell_to_symbol_tally_hrz(Grid3D& p_OT,  Grid2D& temp_2, int x, int size, int pid, int stat_data[]);
int remove_invalid_symbols_hrz(Grid3D& p_OT,  Grid2D& temp_2, int s, int x, int size, int pid, int stat_data[]);

int find_bound_cells_vrt(Grid3D& p_OT,  Grid2D& temp_2, int y, int size, int blk_size, int pid, int stat_data[]);
int find_cell_to_symbol_tally_vrt(Grid3D& p_OT,  Grid2D& temp_2, int y, int size, int pid, int stat_data[]);
int remove_invalid_symbols_vrt(Grid3D& p_OT,  Grid2D& temp_2, int s, int y, int size, int pid, int stat_data[]);

int find_bound_cells_blk(Grid3D& p_OT,  Grid2D& temp_2, int bx, int by, int size, int blk_size, int pid, int stat_data[]);
int find_cell_to_symbol_tally_blk(Grid3D& p_OT,  Grid2D& temp_2, int bx, int by, int size, int blk_size, int pid, int stat_data[]);
int remove_invalid_symbols_blk(Grid3D& p_OT,  Grid2D& temp_2, int s, int bx, int by, int size, int blk_size, int pid, int stat_data[]);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int remove_invalid_options_from_bound_cells(Grid3D& p_OT,  Grid2D& temp_2, int size, int blk_size, int pid, int stat_data[])
{
	int i = 0, j = 0, flag = 0;

	while (i < size && flag == 0)
	{
		flag = find_bound_cells_hrz(p_OT, temp_2, i, size, blk_size, pid, stat_data);
		i++;
	}

	if (flag == 0)
	{
		i = 0;
		while (i < size && flag == 0)
		{
			flag = find_bound_cells_vrt(p_OT, temp_2, i, size, blk_size, pid, stat_data);
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
				flag = find_bound_cells_blk(p_OT, temp_2, i, j, size, blk_size, pid, stat_data);
				j = j + blk_size;
			}
			i = i + blk_size;
		}
	}

	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_bound_cells_hrz(Grid3D& p_OT,  Grid2D& temp_2, int x, int size, int blk_size, int pid, int stat_data[])
{
	int i, j, k, c = 0, flag = 0;
	

	for (i = 0;i < size + 1;i++) { temp_2(i,0) = 0; temp_2(i,1) = 0; temp_2(i,2) = 0; temp_2(i,3) = 0;temp_2(i,4) = 0; }

	for (j = 0;j < size;j++)
	{

		if (p_OT(x,j,0) > 0)
		{
			c++;
			if (p_OT(x,j,size + 1) == pid) { flag = 1; }
			for (k = 1;k < size + 1;k++) { if (p_OT(x,j,k) > 0) { temp_2(k,0)++; temp_2(k,1) = k; } }
		}
	}

	if (c > 4 && flag==1)
	{
		sort_symbol_count_dsc(temp_2, size);
		flag = find_cell_to_symbol_tally_hrz(p_OT, temp_2, x, size, pid, stat_data);
	}
	else { flag = 0; }

	return flag;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_cell_to_symbol_tally_hrz(Grid3D& p_OT,  Grid2D& temp_2, int x, int size, int pid, int stat_data[])
{
	int i = 0, j, k = 0, end = 0, c = 0, flag = 0, s;


	while (temp_2(end,0) > 0 && end < size + 1) { end++; }

	while (i < end - 1 && flag == 0)
	{
		c = 0;
		s = temp_2(i,1);
		for (j = 0;j < size + 1;j++) { temp_2(j,2) = 0; temp_2(j,3) = 0; temp_2(j,4) = 0; }

		for (j = 0;j < size;j++)
		{
			if (p_OT(x,j,0) > 0)
			{
				if (p_OT(x,j,s) > 0)
				{
					for (k = 1;k < size + 1;k++) { if (p_OT(x,j,k) > 0) { temp_2(k,2) = k; } }
				}
				else
				{
					for (k = 1;k < size + 1;k++) { if (p_OT(x,j,k) > 0) { temp_2(k,3) = k; } }
				}
			}
		}

		for (j = 1;j < size + 1;j++)
		{
			if (temp_2(j,2) > 0 && temp_2(j,3) == 0) { c++; temp_2(j,4) = j; }
		}

		if (c == temp_2(i,0))
		{
			flag = remove_invalid_symbols_hrz(p_OT,  temp_2, s, x, size, pid, stat_data);
		}

		i++;

	}
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int remove_invalid_symbols_hrz(Grid3D& p_OT,  Grid2D& temp_2, int s, int x, int size, int pid, int stat_data[])
{
	int j, k, flag = 0;

	for (j = 0;j < size;j++)
	{
		if (p_OT(x,j,s) > 0)
		{
			for (k = 1;k < size + 1;k++)
			{
				if (p_OT(x,j,k) > 0 && temp_2(k,4) == 0)
				{
					p_OT(x,j,k) = 0;
					p_OT(x,j,0)--;
					p_OT(x,j,size + 1) = pid;
					stat_data[9]++;
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
int find_bound_cells_vrt(Grid3D& p_OT,  Grid2D& temp_2, int y, int size, int blk_size, int pid, int stat_data[])
{
	int i,  k, c = 0, flag = 0;
	
	for (i = 0;i < size + 1;i++) { temp_2(i,0) = 0; temp_2(i,1) = 0; temp_2(i,2) = 0; temp_2(i,3) = 0;temp_2(i,4) = 0; }

	for (i = 0;i < size;i++)
	{

		if (p_OT(i,y,0) > 0)
		{
			c++;
			if (p_OT(i,y,size + 1) == pid) { flag = 1; }
			for (k = 1;k < size + 1;k++) { if (p_OT(i,y,k) > 0) { temp_2(k,0)++; temp_2(k,1) = k; } }
		}
	}

	if (c > 4 && flag==1)
	{
		sort_symbol_count_dsc(temp_2, size);
		flag = find_cell_to_symbol_tally_vrt(p_OT,  temp_2, y, size, pid, stat_data);
	}
	else { flag = 0; }
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_cell_to_symbol_tally_vrt(Grid3D& p_OT,  Grid2D& temp_2, int y, int size, int pid, int stat_data[])
{
	int i, j = 0, k = 0, end = 0, c = 0, flag = 0, s;


	while (temp_2(end,0) > 0 && end < size + 1) { end++; }

	while (j < end - 1 && flag == 0)
	{
		c = 0;
		s = temp_2(j,1);
		for (i = 0;i < size + 1;i++) { temp_2(i,2) = 0; temp_2(i,3) = 0; temp_2(i,4) = 0; }

		for (i = 0;i < size;i++)
		{
			if (p_OT(i,y,0) > 0)
			{
				if (p_OT(i,y,s) > 0)
				{
					for (k = 1;k < size + 1;k++) { if (p_OT(i,y,k) > 0) { temp_2(k,2) = k; } }
				}
				else
				{
					for (k = 1;k < size + 1;k++) { if (p_OT(i,y,k) > 0) { temp_2(k,3) = k; } }
				}
			}
		}

		for (i = 1;i < size + 1;i++)
		{
			if (temp_2(i,2) > 0 && temp_2(i,3) == 0) { c++; temp_2(i,4) = i; }
		}

		if (c == temp_2(j,0))
		{
			flag = remove_invalid_symbols_vrt(p_OT, temp_2, s, y, size, pid, stat_data);
		}

		j++;

	}
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int remove_invalid_symbols_vrt(Grid3D& p_OT,  Grid2D& temp_2, int s, int y, int size, int pid, int stat_data[])
{
	int i, k, flag = 0;

	for (i = 0;i < size;i++)
	{
		if (p_OT(i,y,s) > 0)
		{
			for (k = 1;k < size + 1;k++)
			{
				if (p_OT(i,y,k) > 0 && temp_2(k,4) == 0)
				{
					p_OT(i,y,k) = 0;
					p_OT(i,y,0)--;
					p_OT(i,y,size + 1) = pid;
					stat_data[9]++;
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
int find_bound_cells_blk(Grid3D& p_OT,  Grid2D& temp_2, int bx, int by, int size, int blk_size, int pid, int stat_data[])
{
	int i, j, k, c = 0, flag = 0;
	

	for (i = 0;i < size + 1;i++) { temp_2(i,0) = 0; temp_2(i,1) = 0; temp_2(i,2) = 0; temp_2(i,3) = 0;temp_2(i,4) = 0; }

	for (i = bx;i < bx + blk_size;i++)
	{
		for (j = by;j < by + blk_size;j++)
		{
			if (p_OT(i,j,0) > 0)
			{
				c++;
				if (p_OT(i,j,size + 1) == pid) { flag = 1; }
				for (k = 1;k < size + 1;k++) { if (p_OT(i,j,k) > 0) { temp_2(k,0)++; temp_2(k,1) = k; } }
			}
		}
	}

	if (c > 4 && flag==1)
	{
		sort_symbol_count_dsc(temp_2, size);
		flag = find_cell_to_symbol_tally_blk(p_OT, temp_2, bx, by, size, blk_size, pid, stat_data);
	}
	else { flag = 0; }
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_cell_to_symbol_tally_blk(Grid3D& p_OT,  Grid2D& temp_2, int bx, int by, int size, int blk_size, int pid, int stat_data[])
{
	int i, j = 0, k = 0, start = 0, end = 0, c = 0, flag = 0, s;


	while (temp_2(end,0) > 0 && end < size + 1) { end++; }

	while (start < end - 1 && flag == 0)
	{
		c = 0;
		s = temp_2(start,1);
		for (i = 0;i < size + 1;i++) { temp_2(i,2) = 0; temp_2(i,3) = 0; temp_2(i,4) = 0; }

		for (i = bx;i < bx + blk_size;i++)
		{
			for (j = by;j < by + blk_size;j++)
			{
				if (p_OT(i,j,0) > 0)
				{
					if (p_OT(i,j,s) > 0)
					{
						for (k = 1;k < size + 1;k++) { if (p_OT(i,j,k) > 0) { temp_2(k,2) = k; } }
					}
					else
					{
						for (k = 1;k < size + 1;k++) { if (p_OT(i,j,k) > 0) { temp_2(k,3) = k; } }
					}
				}
			}
		}

		for (i = 1;i < size + 1;i++)
		{
			if (temp_2(i,2) > 0 && temp_2(i,3) == 0) { c++; temp_2(i,4) = i; }
		}

		if (c == temp_2(start,0))
		{
			flag = remove_invalid_symbols_blk(p_OT,temp_2, s, bx, by, size, blk_size, pid, stat_data);
		}

		start++;

	}
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int remove_invalid_symbols_blk(Grid3D& p_OT,  Grid2D& temp_2, int s, int bx, int by, int size, int blk_size, int pid, int stat_data[])
{
	int i, j, k, flag = 0;

	for (i = bx;i < bx + blk_size;i++)
	{
		for (j = by;j < by + blk_size;j++)
		{
			if (p_OT(i,j,s) > 0)
			{
				for (k = 1;k < size + 1;k++)
				{
					if (p_OT(i,j,k) > 0 && temp_2(k,4) == 0)
					{
						p_OT(i,j,k) = 0;
						p_OT(i,j,0)--;
						p_OT(i,j,size + 1) = pid;
						stat_data[9]++;
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
int sort_symbol_count_dsc(Grid2D& temp_2, int size)
{
	int i, j, k;

	for (i = 0;i < size;i++)
	{
		for (j = i + 1;j < size + 1;j++)
		{
			if (temp_2(i,0) < temp_2(j,0))
			{
				k = temp_2(i,0);temp_2(i,0) = temp_2(j,0);temp_2(j,0) = k;
				k = temp_2(i,1);temp_2(i,1) = temp_2(j,1);temp_2(j,1) = k;
			}
		}
	}
	return 0;
}


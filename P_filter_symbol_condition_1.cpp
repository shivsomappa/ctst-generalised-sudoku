#include"filters.h"
using namespace std;


int filter_hidden_symbol_hrz(Grid3D& p_OT,  Grid2D& temp_2, int x, int size, int blk_size, int pid, int stat_data[]);
int filter_hidden_symbol_vrt(Grid3D& p_OT,  Grid2D& temp_2, int y, int size, int blk_sizeint, int pid, int stat_data[]);
int filter_hidden_symbol_blk(Grid3D& p_OT,  Grid2D& temp_2, int bx, int by, int size, int blk_size, int pid, int stat_data[]);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_hidden_symbol(Grid3D& p_OT,  Grid2D& temp_2, int size, int blk_size, int pid, int stat_data[])
{
	int   x = 0, y = 0, flag = 0;


	while (x < size && flag == 0)
	{
		flag = filter_hidden_symbol_hrz(p_OT,  temp_2, x, size, blk_size, pid, stat_data);
		x++;
	}
	//*****************************************************	
	if (flag == 0)
	{
		while (y < size && flag == 0)
		{
			flag = filter_hidden_symbol_vrt(p_OT,  temp_2,  y, size, blk_size, pid, stat_data);
			y++;
		}
	}
	//*****************************************************	
	if (flag == 0)
	{
		x = 0; 
		while (x < size && flag == 0)
		{
			y = 0;
			while (y < size && flag == 0)
			{
				flag = filter_hidden_symbol_blk(p_OT, temp_2, x, y, size, blk_size, pid, stat_data);
				y = y + blk_size;
			}
			x = x + blk_size;
		}
	}

	return flag;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_hidden_symbol_hrz(Grid3D& p_OT,  Grid2D& temp_2,  int x, int size, int blk_size, int pid, int stat_data[])
{
	int i, j = 0, k, y, flag = 0;

	for (i = 0; i < size + 1; i++) { temp_2(i,0) = 0; }

	for (i = 0; i < size; i++)
	{
		if (p_OT(x,i,0) > 0)
		{
			for (k = 1; k < size + 1; k++)
			{
				if (p_OT(x,i,k) > 0) { temp_2(k,0)++; }
			}
		}
	}
	//*************************************************
	k = 1;
	while (k < size + 1 && flag == 0)
	{
		if (temp_2(k,0) == 1)
		{
			for (i = 0; i < size; i++) { if (p_OT(x,i,k) > 0) { y = i; } }

			for (i = 1; i < size + 1; i++) { p_OT(x,y,i) = 0; }
			p_OT(x,y,size + 1) = pid;
			p_OT(x,y,k) = k;
			p_OT(x,y,0) = 1;
			stat_data[4]++;
			flag = 1;
		}
		k++;
	}
	return flag;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_hidden_symbol_vrt(Grid3D& p_OT,   Grid2D& temp_2, int y, int size, int blk_size, int pid, int stat_data[])
{
	int i = 0, k, x, flag = 0;

	for (i = 0; i < size + 1; i++) { temp_2(i,0) = 0; }

	for (i = 0; i < size; i++)
	{
		if (p_OT(i,y,0) > 0)
		{
			for (k = 1; k < size + 1; k++) { if (p_OT(i,y,k) > 0) { temp_2(k,0)++; } }
		}
	}
	//****************************************
	k = 1;
	while (k < size + 1 && flag == 0)
	{
		if (temp_2(k,0) == 1)
		{
			for (i = 0; i < size; i++) { if (p_OT(i,y,k) > 0) { x = i; } }

			for (i = 1; i < size + 1; i++) { p_OT(x,y,i) = 0; }
			p_OT(x,y,size + 1) = pid;
			p_OT(x,y,k) = k;
			p_OT(x,y,0) = 1;
			stat_data[4]++;
			flag = 1;
		}
		k++;
	}
	return flag;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_hidden_symbol_blk(Grid3D& p_OT,  Grid2D& temp_2, int bx, int by, int size, int blk_size, int pid, int stat_data[])
{
	int i = 0, j, k, x, y, flag = 0;

	for (i = 0; i < size + 1; i++) { temp_2(i,0) = 0; }

	for (i = bx; i < bx + blk_size; i++)
	{
		for (j = by; j < by + blk_size; j++)
		{
			if (p_OT(i,j,0) > 0)
			{
				for (k = 1; k < size + 1; k++) { if (p_OT(i,j,k) > 0) { temp_2(k,0)++; } }
			}
		}
	}

	//*******************************************************************
	k = 1;
	while (k < size + 1 && flag == 0)
	{
		if (temp_2(k,0) == 1)
		{
			for (i = bx; i < bx + blk_size; i++)
			{
				for (j = by; j < by + blk_size; j++)
				{
					if (p_OT(i,j,k) > 0) { x = i; y = j; }

				}
			}

			for (i = 1; i < size + 1; i++) { p_OT(x,y,i) = 0; }
			p_OT(x,y,size + 1) = pid;
			p_OT(x,y,k) = k;
			p_OT(x,y,0) = 1;
			stat_data[4]++;
			flag = 1;
		}
		k++;
	}
	return flag;

}

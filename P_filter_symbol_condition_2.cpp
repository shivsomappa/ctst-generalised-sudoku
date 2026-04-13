#include"filters.h"
using namespace std;

int filter_block_hrz_symbol(Grid3D& p_OT,  Grid2D& temp_2, int bx, int by, int size, int blk_size, int pid, int stat_data[]);
int filter_block_vrt_symbol(Grid3D& p_OT,  Grid2D& temp_2, int bx, int by, int size, int blk_size, int pid, int stat_data[]);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_block_symbol(Grid3D& p_OT,  Grid2D& temp_2, int size, int blk_size, int pid, int stat_data[])
{
	int i, j, k, flag = 0, f;
	int bx=0, by;

	while(bx<size&&flag==0)
	{
		by = 0;
		while(by<size&&flag==0)
		{
			f = 0;
			for (i = 0; i < size + 1; i++) { temp_2(i,0) = 0; }

			for (i = bx; i < bx + blk_size; i++)
			{
				for (j = by; j < by + blk_size; j++)
				{
					for (k = 1; k < size + 1; k++) 
					{
						if (p_OT(i,j,k) > 0) { temp_2(k,0)++; f = 1; }
					}
				}
			}

			if (f == 1)
			{
				f = filter_block_vrt_symbol(p_OT, temp_2, bx, by, size, blk_size, pid, stat_data);
				if (f == 0)
				{
					f = filter_block_hrz_symbol(p_OT, temp_2, bx, by, size, blk_size, pid, stat_data);
				}
				if (f == 1) { flag = 1; }
			}
			by = by + blk_size;
		}
		bx = bx + blk_size;
	}

	return flag;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_block_hrz_symbol(Grid3D& p_OT,  Grid2D& temp_2, int bx, int by, int size, int blk_size, int pid, int stat_data[])
{
	int i, j, k, x, c1, c2, c3, flag = 0;

	for (k = 1; k < size + 1; k++)
	{
		if (temp_2(k,0) > 0)
		{
			c1 = 0;
			for (i = bx; i < bx + blk_size; i++)
			{
				c2 = 0; c3 = 0;
				for (j = 0; j < size; j++)
				{
					if (p_OT(i,j,k) > 0)
					{
						if ((j / blk_size) * blk_size == by) { c2++; }
						else { c3++; }
					}
				}

				if (c2 > 0)
				{
					x = i;
					if (c3 == 0)
					{
						c1 = size;
						for (i = bx; i < bx + blk_size; i++)
						{
							for (j = by; j < by + blk_size; j++)
							{
								if (i != x && p_OT(i,j,k) > 0)
								{
									p_OT(i,j,k) = 0;
									p_OT(i,j,0)--;
									p_OT(i,j,size + 1) = pid;
									stat_data[5]++;
									flag = 1;
								}
							}
						}

					}
					else { c1++; }
				}
			}

			if (c1 == 1)
			{
				for (j = 0; j < size; j++)
				{
					if ((j / blk_size) * blk_size != by && p_OT(x,j,k) > 0)
					{
						p_OT(x,j,k) = 0;
						p_OT(x,j,0)--;
						p_OT(x,j,size + 1) = pid;
						stat_data[5]++;
						flag = 1;
					}
				}
			}
		}
	}
	return flag;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_block_vrt_symbol(Grid3D& p_OT,  Grid2D& temp_2, int bx, int by, int size, int blk_size, int pid, int stat_data[])
{
	int i, j, k, y, c1, c2, c3, flag = 0;

	for (k = 1; k < size + 1; k++)
	{
		if (temp_2(k,0) > 0)
		{
			c1 = 0;
			for (j = by; j < by + blk_size; j++)
			{
				c2 = 0; c3 = 0;
				for (i = 0; i < size; i++)
				{
					if (p_OT(i,j,k) > 0)
					{
						if ((i / blk_size) * blk_size == bx) { c2++; }
						else { c3++; }
					}
				}

				if (c2 > 0)
				{
					y = j;
					if (c3 == 0)
					{
						c1 = size;
						for (j = by; j < by + blk_size; j++)
						{
							for (i = bx; i < bx + blk_size; i++)
							{
								if (j != y && p_OT(i,j,k) > 0)
								{
									p_OT(i,j,k) = 0;
									p_OT(i,j,0)--;
									p_OT(i,j,size + 1) = pid;
									stat_data[5]++;
									flag = 1;
								}
							}
						}
					}
					else { c1++; }
				}
			}

			if (c1 == 1)
			{
				for (i = 0; i < size; i++)
				{
					if ((i / blk_size) * blk_size != bx && p_OT(i,y,k) > 0)
					{
						p_OT(i,y,k) = 0;
						p_OT(i,y,0)--;
						p_OT(i,y,size + 1) = pid;
						stat_data[5]++;
						flag = 1;
					}
				}
			}
		}
	}
	return flag;
}


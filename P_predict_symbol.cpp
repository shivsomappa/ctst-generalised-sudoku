#include"filters.h"
using namespace std;

int select_set(Grid3D& p_OT, int size, int pid, int& x, int& y);
int predict_symbol_condition_1(Grid3D& p_OT, Grid2D& temp_2, int s1, int s2, int x, int y, int size, int& valid_s);
int predict_symbol_condition_2(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_3(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_4(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_5(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_6(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_7(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_8(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_9(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_10(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_11(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_12(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_13(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_14(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_15(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s);
int predict_symbol_condition_16(Grid3D& p_OT, Grid2D& s_table, int s1, int s2, int x, int y, int size, int& valid_s);
int predict_symbol_condition_17(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int& valid_s);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol(Grid3D& p_OT, Grid2D& p_table, Grid2D& s_table, Grid2D& temp_2, int size, int blk_size, int& pid, int stat_data[])
{
	int flag = 0, s1, s2, x = -1, y = -1, valid_s = 0;
	int n = 0;
	select_set(p_OT, size, pid+1,x, y);

	if (x > -1)
	{
		s1 = 1;			while (p_OT(x,y,s1) <1) { s1++; }
		s2 = s1 + 1;	while (p_OT(x,y,s2) <1) { s2++; }

		flag = predict_symbol_condition_1(p_OT, temp_2, s1, s2, x, y, size, valid_s);
		if (flag == 0) { flag = predict_symbol_condition_2(p_OT, s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_3(p_OT, s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_4(p_OT, s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_5(p_OT, s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_6(p_OT, s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_7(p_OT, s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_8(p_OT, s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_9(p_OT, s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_10(p_OT,s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_11(p_OT,s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_12(p_OT,s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_13(p_OT,s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_14(p_OT,s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_15(p_OT,s1, s2, x, y, size, blk_size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_16(p_OT, p_table, s1, s2, x, y, size, valid_s); }
		if (flag == 0) { flag = predict_symbol_condition_17(p_OT, s1, s2, x, y, size, valid_s); }
		stat_data[1]++;
		if (valid_s != s_table(x, y)) { stat_data[2]++; }
		valid_s = s_table(x,y);
		p_OT(x,y,s1) = 0;
		p_OT(x,y,s2) = 0;
		p_OT(x,y,valid_s) = valid_s;
		p_OT(x,y,0) = 1;
		p_OT(x,y,size+1) = pid;
		pid--; flag = 1;
	}
	else
	{
		cout << " Two symbols set not found, process will terminate " << endl;
		flag = 2;
	}

	return flag;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int select_set(Grid3D& p_OT, int size, int pid, int& x, int& y)
{
	int i, j, k, set3 = 0, id = 1, c;
	int s1, s2;

	x = -1;y = -1;
	for (i = 0;i < size;i++)
	{
		for (j = 0;j < size;j++)
		{
			if (p_OT(i,j,0) == 2 && p_OT(i,j,size + 1) == pid)
			{
				c = 0;
				s1 = 1;		  while (p_OT(i,j,s1) == 0) { s1++; }
				s2 = s1 + 1;  while (p_OT(i,j,s2) == 0) { s2++; }

				for (k = 0;k < size;k++)
				{
					if (p_OT(i,k,0) > 0 && p_OT(i,k,0) < 4)
					{
						if (p_OT(i,k,s1) > 0 || p_OT(i,k,s2) > 0) { c++; }
					}
					if (p_OT(k,j,0) > 0 && p_OT(k,j,0) < 4)
					{
						if (p_OT(k,j,s1) > 0 || p_OT(k,j,s2) > 0) { c++; }
					}
				}
				if (c > set3) { set3 = c; x = i;y = j; }
			}
		}
	}
	if (x == -1)
	{
		for (i = 0;i < size;i++)
		{
			for (j = 0;j < size;j++)
			{
				if (p_OT(i,j,0) == 2)
				{
					if (p_OT(i,j,size + 1) < id) { id = p_OT(i,j,size + 1); x = i;y = j; }
					else if (p_OT(i,j,size + 1) == id)
					{
						c = 0;
						s1 = 1;		  while (p_OT(i,j,s1) == 0) { s1++; }
						s2 = s1 + 1;  while (p_OT(i,j,s2) == 0) { s2++; }

						for (k = 0;k < size;k++)
						{
							if (p_OT(i,k,0) > 0 && p_OT(i,k,0) < 4)
							{
								if (p_OT(i,k,s1) > 0 || p_OT(i,k,s2) > 0) { c++; }
							}
							if (p_OT(k,j,0) > 0 && p_OT(k,j,0) < 4)
							{
								if (p_OT(k,j,s1) > 0 || p_OT(k,j,s2) > 0) { c++; }
							}
						}
						if (c > set3) { set3 = c; x = i;y = j; }
					}
				}
			}
		}

	}

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_1(Grid3D& p_OT, Grid2D& p_temp_2, int s1, int s2, int x, int y, int size, int& valid_s)
{
	int i, j;
	int hleast_count = size, vleast_count = size, hleast_s = 0, vleast_s = 0;
	int c1 = 0, c2 = 0;

	valid_s = 0;
	for (i = 0;i < size + 1;i++) { p_temp_2(i,0) = 0;p_temp_2(i,1) = 0; }

	for (i = 0;i < size;i++)
	{
		if (p_OT(x,i,0) > 0)
		{
			for (j = 1;j < size + 1;j++) { if (p_OT(x,i,j) > 0) { p_temp_2(j,0)++; } }
		}
		if (p_OT(i,y,0) > 0)
		{
			for (j = 1;j < size + 1;j++) { if (p_OT(i,y,j) > 0) { p_temp_2(j,1)++; } }
		}
	}
	//****************************************************************************************
	for (i = 1;i < size + 1;i++)
	{
		if (p_temp_2(i,0) > 0)
		{
			if (p_temp_2(i,0) < hleast_count) { hleast_count = p_temp_2(i,0);hleast_s = i;c1 = 1; }
			else if (p_temp_2(i,0) == hleast_count) { c1++; }
		}
		if (p_temp_2(i,1) > 0)
		{
			if (p_temp_2(i,1) < vleast_count) { vleast_count = p_temp_2(i,1);vleast_s = i;c2 = 1; }
			else if (p_temp_2(i,1) == vleast_count) { c2++; }
		}
	}

	if (c1 == 1 && (hleast_s == s1 || hleast_s == s2)) { valid_s = hleast_s; }
	else if (c2 == 1 && (vleast_s == s1 || vleast_s == s2)) { valid_s = vleast_s; }

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_2(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	int s1_set2 = 0, s2_set2 = 0;

	valid_s = 0;
	for (i = 0;i < size;i++)
	{
		if (i != y && p_OT(x,i,0) == 2)
		{
			if (p_OT(x,i,s1) > 0) { s1_set2++; }
			if (p_OT(x,i,s2) > 0) { s2_set2++; }
		}
		if (i != x && p_OT(i,y,0) == 2)
		{
			if (p_OT(i,y,s1) > 0) { s1_set2++; }
			if (p_OT(i,y,s2) > 0) { s2_set2++; }
		}
	}

	for (i = bx;i < bx + blk_size;i++)
	{
		for (j = by;j < by + blk_size;j++)
		{
			if (i != x && j!=y && p_OT(i,j,0) == 2)
			{
				if (p_OT(i,j,s1) > 0) { s1_set2++; }
				if (p_OT(i,j,s2) > 0) { s2_set2++; }
			}
		}
	}

	if (s1_set2 == 1 && s2_set2 > 2) { valid_s = s1; }
	else if (s2_set2 == 1 && s1_set2 > 2) { valid_s = s2; }

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_3(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j, s3, y1;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	int s1_set2 = 0, s2_set2 = 0, s3_set2=0;
	
	valid_s = 0;
	for (i = 0;i < size;i++)
	{
		if (i != y && p_OT(x,i,0) == 2)
		{
			if (p_OT(x,i,s1) > 0) { s1_set2++; y1 = i; }
			if (p_OT(x,i,s2) > 0) { s2_set2++; y1 = i; }
		}
	}

	if (s1_set2 == 0 && s2_set2 > 1) { valid_s = s1; }
	else if (s2_set2 == 0 && s1_set2 > 1) { valid_s = s2; }
	else if (s1_set2 == 0 && s2_set2 == 1)
	{
		s3 = 1; while (p_OT(x,y1,s3) == 0 || p_OT(x,y1,s3) == s2) { s3++; }
		for (i = 0;i < size;i++) { if (p_OT(x,i,0) == 2 && p_OT(x,i,s3) > 0) { s3_set2++; } }
		if (s3_set2 > 1) { valid_s = s1; }
	}
	else if (s2_set2 == 0 && s1_set2 == 1)
	{
		s3 = 1; while (p_OT(x,y1,s3) == 0 || p_OT(x,y1,s3) == s1) { s3++; }
		for (i = 0;i < size;i++) { if (p_OT(x,i,0) == 2 && p_OT(x,i,s3) > 0) { s3_set2++; } }
		if (s3_set2 > 1) { valid_s = s2; }
	}

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_4(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j, s3, x1;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	int s1_set2 = 0, s2_set2 = 0, s3_set2 = 0;
	valid_s = 0;
	for (i = 0;i < size;i++)
	{
		if (i != x && p_OT(i,y,0) == 2)
		{
			if (p_OT(i,y,s1) > 0) { s1_set2++; x1 = i; }
			if (p_OT(i,y,s2) > 0) { s2_set2++; x1 = i; }
		}
	}

	if (s1_set2 == 0 && s2_set2 > 1) { valid_s = s1; }
	else if (s2_set2 == 0 && s1_set2 > 1) { valid_s = s2; }
	else if (s1_set2 == 0 && s2_set2 == 1)
	{
		s3 = 1; while (p_OT(x1,y,s3) == 0 || p_OT(x1,y,s3) == s2) { s3++; }
		for (i = 0;i < size;i++) { if (p_OT(x,i,0) == 2 && p_OT(x,i,s3) > 0) { s3_set2++; } }
		if (s3_set2 > 1) { valid_s = s1; }
	}
	else if (s2_set2 == 0 && s1_set2 == 1)
	{
		s3 = 1; while (p_OT(x1,y,s3) == 0 || p_OT(x1,y,s3) == s1) { s3++; }
		for (i = 0;i < size;i++) { if (p_OT(x,i,0) == 2 && p_OT(x,i,s3) > 0) { s3_set2++; } }
		if (s3_set2 > 1) { valid_s = s2; }
	}

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_5(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	int s1_count = 0, s2_count = 0;// count of options of clm or row but not belongs to target block
	int s1_set2 = 0, s2_set2 = 0;
	int c1 = 0, c2 = 0;

	valid_s = 0;
	for (i = bx;i < bx + blk_size;i++)
	{
		for (j = by;j < by + blk_size;j++)
		{
			if (p_OT(i,j,0) ==2)
			{
				if (p_OT(i,j,s1) > 0) { s1_set2++; }
				if (p_OT(i,j,s2) > 0) { s2_set2++; }
			}

			if (i != x && j != y && p_OT(i,j,0) > 0)
			{
				if (p_OT(i,j,s1) > 0) { s1_count++; }
				if (p_OT(i,j,s2) > 0) { s2_count++; }
			}
		}
	}
	//**********************************************************
	if (s1_count == 0 && s2_count > 0) { valid_s = s1; }
	else if (s2_count == 0 && s1_count > 0) { valid_s = s2; }
	else
	{
		s1_count = 0;
		s2_count = 0;
		for (i = 0;i < size;i++)
		{
			if (i / blk_size != y / blk_size)
			{
				if (p_OT(x,i,s1) > 0) { s1_count++; }
				if (p_OT(x,i,s2) > 0) { s2_count++; }
			}
			else
			{
				if (p_OT(x,i,s1) > 0) { c1++; }
				if (p_OT(x,i,s2) > 0) { c2++; }

			}
		}

		if (s1_count == 0 && s2_count > 0)
		{
			if (s1_set2 == 1 || s2_set2 > 1) { valid_s = s1; }
			else { valid_s = s2; }
		}
		else if (s2_count == 0 && s1_count > 0)
		{
			if (s2_set2 == 1 || s1_set2 > 1) { valid_s = s2; }
			else { valid_s = s1; }
		}
		else if (s1_count == 0 && s2_count == 0)
		{
			if (s1_set2 > s2_set2) { valid_s = s2; }
			else if (s2_set2 > s1_set2) { valid_s = s1; }
			else if (c1 > c2) { valid_s = s1; }
			else { valid_s = s2; }
		}
		//********************************************************
		if (valid_s == 0)
		{
			c1 = 0;
			c2 = 0;
			s1_count = 0;
			s2_count = 0;
			for (i = 0;i < size;i++)
			{
				if (i / blk_size != x / blk_size)
				{
					if (p_OT(i,y,s1) > 0) { s1_count++; }
					if (p_OT(i,y,s2) > 0) { s2_count++; }
				}
				else
				{
					if (p_OT(x,i,s1) > 0) { c1++; }
					if (p_OT(x,i,s2) > 0) { c2++; }
				}
			}

			if (s1_count == 0 && s2_count > 0)
			{
				if (s1_set2 == 1 || s2_set2 > 1) { valid_s = s1; }
				else { valid_s = s2; }
			}
			else if (s2_count == 0 && s1_count > 0)
			{
				if (s2_set2 == 1 || s1_set2 > 1) { valid_s = s2; }
				else { valid_s = s1; }
			}
			else if (s1_count == 0 && s2_count == 0)
			{
				if (s1_set2 > s2_set2) { valid_s = s2; }
				else if (s2_set2 > s1_set2) { valid_s = s1; }
				else if (c1 > c2) { valid_s = s1; }
				else { valid_s = s2; }
			}
		}
	}

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_6(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j, k, end = size - blk_size;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	int s1_mcb = 0, s2_mcb = 0; // maximum options count block of row or clm
	int s1_by = 0, s2_by = 0;// max count block address
	int s1_bx = 0, s2_bx = 0;// max count block address
	int h_C1 = 0, h_C2 = 0; // count of max options count blocks
	int v_C1 = 0, v_C2 = 0; // count of max options count blocks
	int c1 = 0, c2 = 0;
	valid_s = 0;

	for (k = 0;k < size;k = k + blk_size)
	{
		c1 = 0;c2 = 0;
		for (i = bx;i < bx + blk_size;i++)
		{
			for (j = k;j < k + blk_size;j++)
			{
				if (p_OT(i,j,s1) > 0) { c1++; }
				if (p_OT(i,j,s2) > 0) { c2++; }
			}
		}

		if (c1 > s1_mcb) { s1_mcb = c1;h_C1 = 1; s1_by = k; }
		else if (c1 == s1_mcb) { h_C1++; }

		if (c2 > s2_mcb) { s2_mcb = c2;h_C2 = 1; s2_by = k; }
		else if (c2 == s2_mcb) { h_C2++; }
	}
	//********************************************************
	s1_mcb = 0;s2_mcb = 0;

	for (k = 0;k < size;k = k + blk_size)
	{
		c1 = 0;c2 = 0;
		for (i = k;i < k + blk_size;i++)
		{
			for (j = by;j < by + blk_size;j++)
			{
				if (p_OT(i,j,s1) > 0) { c1++; }
				if (p_OT(i,j,s2) > 0) { c2++; }
			}
		}

		if (c1 > s1_mcb) { s1_mcb = c1;v_C1 = 1; s1_bx = k; }
		else if (c1 == s1_mcb) { v_C1++; }

		if (c2 > s2_mcb) { s2_mcb = c2;v_C2 = 1; s2_bx = k; }
		else if (c2 == s2_mcb) { v_C2++; }
	}

	//********************************************************
	if (h_C1 == 1 && s1_by == by && v_C1==1 && s1_bx==1)
	{
		if (h_C2>1||v_C2 > 1 || s2_by != by||s2_bx!=bx) { valid_s = s2; }
	}
	else if (h_C2 == 1 && s2_by == by && v_C2 == 1 && s2_bx == 1)
	{
		if (h_C1 > 1 || v_C1 > 1 || s1_by != by || s1_bx != bx) { valid_s = s1; }
	}

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_7(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, hc1 = 0, hc2 = 0, vc1 = 0, vc2 = 0;

	valid_s = 0;
	for (i = 0;i < size;i++)
	{
		if (p_OT(x,i,s1) > 0) { hc1++; }
		if (p_OT(x,i,s2) > 0) { hc2++; }
		if (p_OT(i,y,s1) > 0) { vc1++; }
		if (p_OT(i,y,s2) > 0) { vc2++; }
	}
	if (hc1 == 2 && vc1 == 2)
	{
		if (hc2 > 2 || vc2 > 2) { valid_s = s2; }
	}
	else if (hc2 == 2 && vc2 == 2)
	{
		if (hc1 > 2 || vc1 > 2) { valid_s = s1; }
	}

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_8(Grid3D& p_OT,int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j, k;
	int s1_bbc = 0, s2_bbc = 0, c;//symbols bound to block
	int f1 = 0, f2=0, f3=0, f4=0;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	valid_s = 0;

	for (j = 0;j < size;j++)
	{
		if (p_OT(x,j,s1) > 0)
		{
			c = 0;
			for (i = 0;i < size;i++)
			{
				if (i / blk_size != x / blk_size && p_OT(i,j,s1) > 0) { c++; }
			}

			if (c == 0)
			{
				s1_bbc++;
				if (p_OT(x,j,0) == 2) { f1 = 1; }
				else
				{
					for (i = bx;i < bx + blk_size;i++)
					{
						if (p_OT(i,j,0) == 2 && p_OT(i,j,s1) > 0) { f2 = 1; }
					}
				}
			}
		}
		if (p_OT(x,j,s2) > 0)
		{
			c = 0;
			for (i = 0;i < size;i++)
			{
				if (i / blk_size != x / blk_size && p_OT(i,j,s2) > 0) { c++; }
			}
			if (c == 0) 
			{
				s2_bbc++; 
				if (p_OT(x,j,0) == 2) { f3 = 1; }
				else
				{
					for (i = bx;i < bx + blk_size;i++)
					{
						if (p_OT(i,j,0) == 2 && p_OT(i,j,s2) > 0) { f4 = 1; }
					}
				}
			}
		}
	}
	if (s1_bbc > 0 && s2_bbc == 0)
	{
		if(f2>0&&f1==0){ valid_s = s1; }
		else { valid_s = s2; }
	}
	else if (s2_bbc > 0 && s1_bbc == 0)
	{
		if (f4 > 0 && f3 == 0) { valid_s = s2; }
		else { valid_s = s1; }
	}
	else
	{
		f1 = 0;f2 = 0;f3 = 0;f4 = 0;
		s1_bbc = 0;s2_bbc = 0;
		for (i = 0;i < size;i++)
		{
			if (p_OT(i,y,s1) > 0)
			{
				c = 0;
				for (j = 0;j < size;j++)
				{
					if (j / blk_size != y / blk_size && p_OT(i,j,s1) > 0) { c++; }
				}
				if (c == 0)
				{
					s1_bbc++; 
					if (p_OT(i,y,0) == 2) { f1 = 1; }
					else
					{
						for (j = by;j < by + blk_size;j++)
						{
							if (p_OT(i,j,0) == 2 && p_OT(i,j,s1) > 0) { f2 = 1; }
						}
					}
				}
			}
			if (p_OT(i,y,s2) > 0)
			{
				c = 0;
				for (j = 0;j < size;j++)
				{
					if (j / blk_size != y / blk_size && p_OT(i,j,s2) > 0) { c++; }
				}
				if (c == 0)
				{
					s2_bbc++;
					if (p_OT(i,y,0) == 2) { f3 = 1; }
					else
					{
						for (j = by;j < by + blk_size;j++)
						{
							if (p_OT(i,j,0) == 2 && p_OT(i,j,s2) > 0) { f4 = 1; }
						}
					}
				}
			}
		}

		if (s1_bbc > 0 && s2_bbc == 0)
		{
			if (f1 == 0 && f2 > 0) { valid_s = s1; }
			else { valid_s = s2; }
		}
		else if (s2_bbc > 0 && s1_bbc == 0) 
		{
			if (f3 == 0 && f4 == 1) { valid_s = s2; }
			else { valid_s = s1; }
		}

	}

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_9(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j, k, m;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	int s1_bbc = 0, s2_bbc = 0; //options bound bloc only
	int c1, c2, c3, c4;
	valid_s = 0;
	// band block bound options
	for (k = 0;k < size;k = k + blk_size)
	{
		c1 = 0; c2 = 0;
		for (i = bx;i < bx + blk_size;i++)
		{
			c3 = 0;c4 = 0;
			for (j = k;j < k + blk_size;j++)
			{
				if (p_OT(i,j,s1) > 0) { c3++; }
				if (p_OT(i,j,s2) > 0) { c4++; }
			}
			if (c3 > 0) { c1++; }
			if (c4 > 0) { c2++; }
		}
		if (c1 == 1) { s1_bbc++; }
		else
		{
			c1 = 0; 
			for (j = k;j < k + blk_size;j++)
			{
				c3 = 0;
				for (i = bx;i < bx + blk_size;i++)
				{
					if (p_OT(i,j,s1) > 0) { c3++; }
				}
				if (c3 > 0) { c1++; }
			}
			if (c1 == 1) { s1_bbc++; }
		}

		if (c2 == 1) { s2_bbc++; }
		else
		{
			c2 = 0;
			for (j = k;j < k + blk_size;j++)
			{
				c4 = 0;
				for (i = bx;i < bx + blk_size;i++)
				{
					if (p_OT(i,j,s2) > 0) { c4++; }
				}
				if (c4 > 0) { c2++; }
			}
			if (c2 == 1) { s2_bbc++; }

		}
	}
	if (s1_bbc > 0 && s2_bbc == 0) { valid_s = s1; }
	else if (s2_bbc > 0 && s1_bbc == 0) { valid_s = s2; }

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_10(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j, k, m;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	int s1_bbc = 0, s2_bbc = 0; //options bound to single block only
	int c1, c2, c3, c4;

	valid_s = 0;
	// stack block bound options
	for (k = 0;k < size;k = k + blk_size)
	{
		c1 = 0; c2 = 0;
		for (j = by;j < by + blk_size;j++)
		{
			c3 = 0;c4 = 0;
			for (i = k;i < k + blk_size;i++)
			{
				if (p_OT(i,j,s1) > 0) { c3++; }
				if (p_OT(i,j,s2) > 0) { c4++; }
			}
			if (c3 > 0) { c1++; }
			if (c4 > 0) { c2++; }
		}

		if (c1 == 1) { s1_bbc++; }
		else
		{
			c1 = 0;
			for (i = k;i < k + blk_size;i++)
			{
				c3 = 0;
				for (j = by;j < by + blk_size;j++)
				{
					if (p_OT(i,j,s1) > 0) { c3++; }
				}
				if (c3 > 0) { c1++; }
			}
			if (c1 == 1) { s1_bbc++; }
		}

		if (c2 == 1) { s2_bbc++; }
		else
		{
			c2 = 0;
			for (i = k;i < k + blk_size;i++)
			{
				c4 = 0;
				for (j = by;j < by + blk_size;j++)
				{
					if (p_OT(i,j,s2) > 0) { c4++; }
				}
				if (c4 > 0) { c2++; }
			}
			if (c2 == 1) { s2_bbc++; }

		}
	}

	if (s1_bbc > 0 && s2_bbc == 0) { valid_s = s1; }
	else if (s2_bbc > 0 && s1_bbc == 0) { valid_s = s2; }

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_11(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j, k;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	int s1_tbrc = 0, s2_tbrc = 0, s1_tbcc = 0, s2_tbcc = 0;// target block row count and clm count
	int s1_hc = 0, s2_hc = 0, s1_vc = 0, s2_vc = 0;// count of blocks where symbol to be placed (stack and band)
	int c1 = 0, c2 = 0;
	int s1_set2 = 0, s2_set2 = 0;

	valid_s = 0;
	for (i = bx;i < bx + blk_size;i++)
	{
		c1 = 0;c2 = 0;
		for (j = by;j < by + blk_size;j++)
		{
			if (p_OT(i,j,s1) > 0) 
			{
				c1++;
				if (p_OT(i,j,0) == 2) { s1_set2++; }
			}
			if (p_OT(i,j,s2) > 0)
			{
				c2++;
				if (p_OT(i,j,0) == 2) { s2_set2++; }
			}
		}
		if (c1 > 0) { s1_tbrc++; }
		if (c2 > 0) { s2_tbrc++; }
	}

	for (j = by;j < by + blk_size;j++)
	{
		c1 = 0;c2 = 0;
		for (i = bx;i < bx + blk_size;i++)
		{
			if (p_OT(i,j,s1) > 0) { c1++; }
			if (p_OT(i,j,s2) > 0) { c2++; }
		}
		if (c1 > 0) { s1_tbcc++; }
		if (c2 > 0) { s2_tbcc++; }
	}
	//**************************************************
	for (k = 0;k < size;k = k + blk_size)
	{
		c1 = 0;c2 = 0;
		for (i = bx;i < bx + blk_size;i++)
		{
			for (j = k;j < k + blk_size;j++)
			{
				if (p_OT(i,j,s1) > 0) { c1++; }
				if (p_OT(i,j,s2) > 0) { c2++; }
			}
		}
		if (c1 > 0) { s1_hc++; }
		if (c2 > 0) { s2_hc++; }
	}
	//**************************************************
	for (k = 0;k < size;k = k + blk_size)
	{
		c1 = 0;c2 = 0;
		for (j = by;j < by + blk_size;j++)
		{
			for (i = k;i < k + blk_size;i++)
			{
				if (p_OT(i,j,s1) > 0) { c1++; }
				if (p_OT(i,j,s2) > 0) { c2++; }
			}
		}
		if (c1 > 0) { s1_vc++; }
		if (c2 > 0) { s2_vc++; }
	}

	if (s1_tbrc == 2 && s1_hc == 2 && s2_hc > 2 && s2_vc > 2) { valid_s = s1; }
	else if (s1_tbcc == 2 && s1_vc == 2 && s2_hc > 2 && s2_vc > 2) { valid_s = s1; }
	else if (s2_tbrc == 2 && s2_hc == 2 && s1_hc > 2 && s1_vc > 2) { valid_s = s2; }
	else if (s2_tbcc == 2 && s2_vc == 2 && s1_hc > 2 && s1_vc > 2) { valid_s = s2; }
	else if (s1_tbrc < s1_hc && s1_tbcc < s1_vc  && (s2_tbrc == s2_hc || s2_tbcc == s2_vc)) { valid_s = s1; }
	else if (s2_tbrc < s2_hc && s2_tbcc < s2_vc && (s1_tbrc == s1_hc || s1_tbcc == s1_vc)) { valid_s = s2; }
	else if ((s1_hc < s2_hc || s1_vc < s2_vc) && s2_set2 > 1 && s1_set2 == 1) { valid_s = s1; }
	else if ((s2_hc < s1_hc || s2_vc < s1_vc) && s1_set2 > 1 && s2_set2 == 1) { valid_s = s2; }
	else if ((s1_tbrc < s1_hc || s1_tbcc < s1_vc) && s2_tbrc == s2_hc && s2_tbcc == s2_vc) { valid_s = s1; }
	else if ((s2_tbrc < s2_hc || s2_tbcc < s2_vc) && s1_tbrc == s1_hc && s1_tbcc == s1_vc) { valid_s = s2; }


	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_12(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j, k;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	int s1_tbc = 0, s2_tbc = 0;//target blocks options_count
	int s1_max_count_blocks=0, s2_max_count_blocks = 0;//count of blocks where options count greater than target blocks options count
	int s1_least_count_blocks = 0, s2_least_count_blocks = 0;//count of blocks where options count <= target options count
	int c1, c2;

	valid_s = 0;
	for (i = bx;i < bx + blk_size;i++)
	{
		for (j = by;j < by + blk_size;j++)
		{
			if (p_OT(i,j,s1) > 0) { s1_tbc++; }
			if (p_OT(i,j,s2) > 0) { s2_tbc++; }
		}
	}
	for (k = 0;k < size;k = k + blk_size)
	{
		//band row count
		c1 = 0;c2 = 0; 
		for (i = bx;i < bx + blk_size;i++)
		{
			for (j = k;j < k + blk_size;j++)
			{
				if (p_OT(i,j,s1) > 0) { c1++; }
				if (p_OT(i,j,s2) > 0) { c2++; }
			}
		}
		if (c1>s1_tbc) { s1_max_count_blocks++; }
		else if (c1 > 0 && c1 <= s1_tbc) { s1_least_count_blocks++; }

		if (c2>s2_tbc) { s2_max_count_blocks++; }
		else if (c2 > 0 && c2 <= s2_tbc) { s2_least_count_blocks++; }
		//stack clm count
		c1 = 0;c2 = 0; 
		for (j = by;j < by + blk_size;j++)
		{
			for (i = k;i < k + blk_size;i++)
			{
				if (p_OT(i,j,s1) > 0) { c1++; }
				if (p_OT(i,j,s2) > 0) { c2++; }
			}
		}

		if (c1 > s1_tbc) { s1_max_count_blocks++; }
		else if (c1 > 0 && c1 <= s1_tbc) { s1_least_count_blocks++; }

		if (c2 > s2_tbc) { s2_max_count_blocks++; }
		else if (c2 > 0 && c2 <= s2_tbc) { s2_least_count_blocks++; }
	}

	if (s1_max_count_blocks>s1_least_count_blocks && s2_max_count_blocks<=s2_least_count_blocks) { valid_s = s1; }
	else if (s2_max_count_blocks > s2_least_count_blocks && s1_max_count_blocks <= s1_least_count_blocks) { valid_s = s2; }
	else if (s1_max_count_blocks < s1_least_count_blocks && s2_max_count_blocks >= s2_least_count_blocks) { valid_s = s2; }
	else if (s2_max_count_blocks < s2_least_count_blocks && s1_max_count_blocks >= s1_least_count_blocks) { valid_s = s1; }

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_13(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j, k;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	int hs1_bc = 0, hs2_bc = 0; //number of blocks required to be filled
	int vs1_bc = 0, vs2_bc = 0; //number of blocks required to be filled
	int d1 = 0, d2 = 0;
	int c1, c2;

	valid_s = 0;
	for (k = 0;k < size;k = k + blk_size)
	{
		//band row count
		c1 = 0;c2 = 0;
		for (i = bx;i < bx + blk_size;i++)
		{
			for (j = k;j < k + blk_size;j++)
			{
				if (p_OT(i,j,s1) > 0) { c1++; }
				if (p_OT(i,j,s2) > 0) { c2++; }
			}
		}
		if (c1 > 0) { hs1_bc++; }
		if (c2 > 0) { hs2_bc++; }
		//stack clm count
		c1 = 0;c2 = 0;
		for (j = by;j < by + blk_size;j++)
		{
			for (i = k;i < k + blk_size;i++)
			{
				if (p_OT(i,j,s1) > 0) { c1++; }
				if (p_OT(i,j,s2) > 0) { c2++; }
			}
		}
		if (c1 > 0) { vs1_bc++; }
		if (c2 > 0) { vs2_bc++; }
	}
	if (hs1_bc > vs1_bc) { d1 = hs1_bc - vs1_bc; }
	else { d1 = vs1_bc - hs1_bc; }

	if (hs2_bc > vs2_bc) { d2 = hs2_bc - vs2_bc; }
	else { d2 = vs2_bc - hs2_bc; }

	if (d1 > 0 && d2 == 0) { valid_s = s1; }
	else if (d2 > 0 && d1 == 0) { valid_s = s2; }


	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_14(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j, k;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	int s1_bc = 0, s2_bc = 0; //number of blocks required to be filled
	int c1, c2;

	valid_s = 0;
	for (k = 0;k < size;k = k + blk_size)
	{
		//band row count
		c1 = 0;c2 = 0;
		for (i = bx;i < bx + blk_size;i++)
		{
			for (j = k;j < k + blk_size;j++)
			{
				if (p_OT(i,j,s1) > 0) { c1++; }
				if (p_OT(i,j,s2) > 0) { c2++; }
			}
		}
		if (c1 > 0) { s1_bc++; }
		if (c2 > 0) { s2_bc++; }
		//stack clm count
		c1 = 0;c2 = 0;
		for (j = by;j < by + blk_size;j++)
		{
			for (i = k;i < k + blk_size;i++)
			{
				if (p_OT(i,j,s1) > 0) { c1++; }
				if (p_OT(i,j,s2) > 0) { c2++; }
			}
		}
		if (c1 > 0) { s1_bc++; }
		if (c2 > 0) { s2_bc++; }
	}

	if (s1_bc > s2_bc) { valid_s = s1; }
	else if (s2_bc > s1_bc) { valid_s = s2; }


	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_15(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int blk_size, int& valid_s)
{
	int i, j;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	int s1_count = 0, s2_count = 0; //target block options count

	valid_s = 0;
	for (i = bx;i < bx + blk_size;i++)
	{
		for (j = by;j < by + blk_size;j++)
		{
			if (p_OT(i,j,s1) > 0) { s1_count++; }
			if (p_OT(i,j,s2) > 0) { s2_count++; }
		}
	}

	if (s1_count < s2_count) { valid_s = s1; }
	else if (s2_count < s1_count) { valid_s = s2; }

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_16(Grid3D& p_OT, Grid2D& p_table, int s1, int s2, int x, int y, int size, int& valid_s)
{
	int i, j, s1_count = 0, s2_count = 0;

	valid_s = 0;
	for (i = 0;i < size;i++)
	{
		for (j = 0;j < size;j++)
		{
			if (p_table(i,j) == s1) { s1_count++; }
			else if (p_table(i,j) == s2) { s2_count++; }
		}
	}

	if (s1_count > s2_count) { valid_s = s2; }
	else if (s2_count > s1_count) { valid_s = s1; }

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int predict_symbol_condition_17(Grid3D& p_OT, int s1, int s2, int x, int y, int size, int& valid_s)
{
	int i, s1_set3 = 0, s2_set3 = 0;

	valid_s = 0;
	for (i = 0;i < size;i++)
	{
		if (p_OT(x,i,0) < 4)
		{
			if (p_OT(x,i,s1) > 0) { s1_set3++; }
			if (p_OT(x,i,s2) > 0) { s2_set3++; }
		}
		if (p_OT(i,y,0) < 4)
		{
			if (p_OT(i,y,s1) > 0) { s1_set3++; }
			if (p_OT(i,y,s2) > 0) { s2_set3++; }
		}
	}

	if (s1_set3 > s2_set3) { valid_s = s2; }
	else { valid_s = s1; }
	return 0;


}

#include "gsv.h"

using namespace std;
int m1_create_sudoku_template(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size);
int m1_randomise_sudoku_rc_shift(Grid2D& table, int size, int blk_size);
int m1_row_shift_within_blk(Grid2D& table, int size, int bx, int blk_size);
int m1_column_shift_within_blk(Grid2D& table, int size, int bx, int blk_size);
int m1_select_least_count_block(Grid2D& p_table, int& x, int& y, int size, int blk_size);

int m1_build_Options_table(Grid2D& p_table, Grid3D& options_table, int size);
int m1_implement_sudoku_property(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size);
int m1_find_least_count_set_size(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int& lss, int& set1_count, int& set2_count, int& set4_count);

int m1_create_two_symbol_set(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size, int lss);
int m1_create_three_symbol_set_step_1(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size);
int m1_create_three_symbol_set_step_2(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size);
int m1_create_three_symbol_set_step_3(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size);
int m1_create_three_symbol_set_fx1(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int s1, int s2, int x, int y, int size, int blk_size);
int m1_create_three_symbol_set_fx2(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int s1, int x, int y, int size, int blk_size);

int m1_remove_single_symbol_set(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size);
int m1_remove_two_symbol_set(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size);
int m1_remove_symbol_from_options_table(Grid3D& options_table, int s, int x, int y, int size, int blk_size);

int m1_check_nearest_neighbour_condition_hrz(Grid2D& table, int s, int x, int y1, int y2, int size, int blk_size);
int m1_check_nearest_neighbour_condition_vrt(Grid2D& table, int s, int x, int y1, int y2, int size, int blk_size);

int m1_select_second_symbol(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int s1, int& s2, int x, int y, int size, int blk_size);
int m1_select_third_symbol(Grid3D& options_table, Grid2D& s_table, Grid2D& p_table, int s1, int s2, int& s3, int x, int y, int size, int blk_size);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_GENERAT_SUDOKU_METHOD(Grid3D& options_table,Grid2D& p_table,Grid2D& s_table,int size,int blk_size)
{

	m1_create_sudoku_template(options_table,p_table,s_table,size,blk_size);
	m1_build_Options_table(p_table,options_table,size);
	cout<<"Sudoku board created implementing  sudoku properties "<<endl;
	m1_implement_sudoku_property(options_table,p_table,s_table,size,blk_size);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_create_sudoku_template(Grid3D& options_table,Grid2D& p_table,Grid2D& s_table,int size,int blk_size)
{
	int i=0,j=0,k=0,start=0,end=0,x,y;
	int count1=0,count2=size*size*10/100;
	int bx=0,by=0;

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++) { p_table(i,j) = 0; s_table(i,j) = 0; }
	}

	int *temp=NULL;
	temp=new int[size+1];

	int *temp1=NULL;
	temp1=new int[size+1];

	
	for(i=0;i<size;i++){temp[i]=i+1;}
	for(i=0;i<size;i++)
	{
		for(j=i+1;j<size;j++)
		{
			if((rand()+1)%2==0){k=temp[i];temp[i]=temp[j];temp[j]=k;}
		}
	}
	//create sudoku -spiral shift method
	for(j=0;j<size;j++)
	{
		for(i=0;i<size;i++){s_table(i,j)=temp[i];}
		if((j+1)%blk_size==0)
		{
			start=1;i=0;
			while(start<size){temp1[start]=temp[i];start++;i++;}
			start=0;while(i<size){temp1[start]=temp[i];i++;start++;}
			
		}
		else
		{
			start=blk_size;i=0;
			while(start<size){temp1[start]=temp[i];start++;i++;}
			start=0;while(i<size){temp1[start]=temp[i];i++;start++;}
		}
		for(i=0;i<size;i++){temp[i]=temp1[i];}
	}
	//rand0mise sudoku
	m1_randomise_sudoku_rc_shift(s_table, size, blk_size);
	//copy sudoku to s_table
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++){p_table(i,j)=0;}
	}

	//add symbols randmly to  puzzal board
	srand(time(0));
	while(count1<count2)
	{
		x=rand()%blk_size;
		y=rand()%blk_size;
		m1_select_least_count_block(p_table,  x,  y, size, blk_size);
		if(p_table(x,y)==0)
		{
			p_table(x,y)=s_table(x,y);
			count1++;
		}
		
	}

	delete[] temp;
	delete[] temp1;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int m1_select_least_count_block(Grid2D& p_table,int& x, int& y,int size,int blk_size)
{
	int i, j, X = x, Y = y, count = blk_size * blk_size,bx,by,c;

	for (bx = 0; bx < size; bx=bx+blk_size)
	{
		for (by = 0; by < size; by=by+blk_size)
		{
			if (p_table(bx+X,by+Y)==0)
			{
				c = 0;
				for (i = bx; i < bx + blk_size; i++)
				{
					for (j = by; j < by + blk_size; j++)
					{
						if (p_table(i,j) > 0) { c++; }
					}
					
				}
				if (c < count) { count = c; x = bx + X; y = by + Y; }
			}
		}
	}
	
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_randomise_sudoku_rc_shift(Grid2D& table,int size,int blk_size)
{
	int i=0;
	for(i=0;i<size;i=i+blk_size)
	{
		m1_row_shift_within_blk(table, size,i, blk_size);
	}

	for(i=0;i<size;i=i+blk_size)
	{
		m1_column_shift_within_blk(table, size,i, blk_size);
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_row_shift_within_blk(Grid2D& table,int size,int bx,int blk_size)
{
	int x,i,j,s;
	
	for(i=bx;i<bx+blk_size;i++)
	{
		x=rand()%blk_size+bx;
		if(x!=i)
		{
			for(j=0;j<size;j++){s=table(i,j);table(i,j)=table(x,j);table(x,j)=s;}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_column_shift_within_blk(Grid2D& table,int size,int by,int blk_size)
{
	int y,i,j,s;

	for(j=by;j<by+blk_size;j++)
	{
		y=rand()%blk_size+by;
		if(y!=j)
		{
			for(i=0;i<size;i++){s=table(i,j);table(i,j)=table(i,y);table(i,y)=s;}
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_implement_sudoku_property(Grid3D& options_table,Grid2D& p_table,Grid2D& s_table,int size,int blk_size)
{
	int flag=0,lss,set1_count=0,set2_count=0,set3_count=1;

	int t=0;
	//initialise process
	m1_find_least_count_set_size(options_table,p_table,s_table,size,lss,set1_count,set2_count,set3_count);
	if(set2_count==0)

	while(set3_count>0)
	{
		
		if(set1_count>0)
		{
			m1_create_three_symbol_set_step_1(options_table,p_table,s_table, size, blk_size);
			m1_remove_single_symbol_set(options_table,p_table,s_table, size, blk_size);
		}
		else if(set2_count>0)
		{
			m1_create_three_symbol_set_step_2(options_table,p_table,s_table, size, blk_size);
			m1_create_three_symbol_set_step_3(options_table, p_table, s_table, size, blk_size);
			m1_remove_two_symbol_set(options_table,p_table,s_table, size, blk_size);
		}
		else 
		{
			m1_create_two_symbol_set(options_table,p_table,s_table, size, blk_size, lss);
			m1_create_three_symbol_set_step_2(options_table, p_table, s_table, size, blk_size);
			m1_create_three_symbol_set_step_3(options_table, p_table, s_table, size, blk_size);
			m1_remove_two_symbol_set(options_table, p_table, s_table, size, blk_size);
		}
		m1_find_least_count_set_size(options_table, p_table, s_table, size, lss, set1_count, set2_count, set3_count);
	
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_create_three_symbol_set_step_2(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size)
{
	int i, j, s1, s2;
	int x, y, x1, y1;

	for (i = 0;i < size;i++)
	{
		for (j = 0;j < size;j++)
		{
			if (options_table(i,j,0) == 2)
			{
				s1 = s_table(i,j);
				s2 = 1;while (options_table(i,j,s2) == 0 || options_table(i,j,s2) == s1) { s2++; }

				x = 0; while (s_table(x,j) != s2) { x++; }
				y = 0; while (s_table(i,y) != s2) { y++; }

				y1 = 0;while (s_table(x,y1) != s1) { y1++; }
				x1 = 0;while (s_table(x1,y) != s1) { x1++; }

				if (p_table(x,y1) == 0 && p_table(x1,y) == 0)
				{
					if (x1 == x && y1 == y)
					{
						if (options_table(x,j,0) > 3 && options_table(i,y,0) > 3)
						{

							p_table(x1,y1) = s_table(x1,y1);
							m1_remove_symbol_from_options_table(options_table, s1, x1, y1, size, blk_size);
						}
						else if (options_table(x,j,0) > 3 && options_table(i,y,s2) == 0)
						{
							p_table(x1,y1) = s_table(x1,y1);
							m1_remove_symbol_from_options_table(options_table, s1, x1, y1, size, blk_size);
						}
						else if (options_table(i,y,0) > 3 && options_table(x,j,s1) == 0)
						{
							p_table(x1,y1) = s_table(x1,y1);
							m1_remove_symbol_from_options_table(options_table, s1, x1, y1, size, blk_size);
						}
					}
					else if (options_table(i,y,0) < 4 && options_table(i,y,s1)>0)
					{
						p_table(x,y1) = s_table(x,y1);
						m1_remove_symbol_from_options_table(options_table, s1, x, y1, size, blk_size);

					}
					else if (options_table(x,j,0) < 4 && options_table(x,j,s1)>0)
					{
						p_table(x1,y) = s_table(x1,y);
						m1_remove_symbol_from_options_table(options_table, s1, x1, y, size, blk_size);

					}
					else if (options_table(x,j,0) < options_table(i,y,0))
					{
						p_table(x,y1) = s_table(x,y1);
						m1_remove_symbol_from_options_table(options_table, s1, x, y1, size, blk_size);
					}
					else
					{
						p_table(x1,y) = s_table(x1,y);
						m1_remove_symbol_from_options_table(options_table, s1, x1, y, size, blk_size);
					}
				}

			}
		}
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_create_two_symbol_set(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size, int lss)
{
	int i = 0, j, s1 = 0, s2 = 0, s3, flag = 0;
	int x, y, x1, y1;

	while (i < size && flag == 0)
	{
		j = 0;
		while (j < size && flag == 0)
		{
			if (options_table(i,j,0) == lss)
			{
				s1 = s_table(i,j);
				m1_select_second_symbol(options_table, p_table, s_table, s1, s2, i, j, size, blk_size);
				if (s2 > 0)
				{
					flag = 1;
					x = 0;y = 0;
					while (s_table(x,j) != s2) { x++; }
					while (s_table(i,y) != s2) { y++; }
					// first remove common option of s1 cell and s2 cell
					for (s3 = 1;s3 < size + 1;s3++)
					{
						if (s3 != s1 && s3 != s2 && options_table(i,j,s3) > 0)
						{
							x1 = 0;y1 = 0;
							while (s_table(x1,j) != s3) { x1++; }
							while (s_table(i,y1) != s3) { y1++; }

							if (options_table(x,j,s3) > 0 && options_table(i,y,s3) > 0)
							{
								if (options_table(i,y,0) < options_table(x,j,0))
								{
									p_table(i,y1) = s_table(i,y1);
									m1_remove_symbol_from_options_table(options_table, s3, i, y1, size, blk_size);
								}
								else
								{
									p_table(x1,j) = s_table(x1,j);
									m1_remove_symbol_from_options_table(options_table, s3, x1, j, size, blk_size);
								}

							}
							else if (options_table(x,j,s3) > 0)
							{
								p_table(x1,j) = s_table(x1,j);
								m1_remove_symbol_from_options_table(options_table, s3, x1, j, size, blk_size);
							}
							else if (options_table(i,y,s3) > 0)
							{
								p_table(i,y1) = s_table(i,y1);
								m1_remove_symbol_from_options_table(options_table, s3, i, y1, size, blk_size);
							}
						}
					}
					// if set size greater than two
					if (options_table(i,j,0) > 2)
					{
						for (s3 = 1;s3 < size + 1;s3++)
						{
							if (s3 != s1 && s3 != s2 && options_table(i,j,s3) > 0)
							{
								x1 = 0;y1 = 0;
								while (s_table(x1,j) != s3) { x1++; }
								while (s_table(i,y1) != s3) { y1++; }

								if (options_table(x1,j,0) > options_table(i,y1,0))
								{
									p_table(x1,j) = s_table(x1,j);
									m1_remove_symbol_from_options_table(options_table, s3, x1, j, size, blk_size);
								}
								else
								{
									p_table(i,y1) = s_table(i,y1);
									m1_remove_symbol_from_options_table(options_table, s3, i, y1, size, blk_size);

								}
							}
						}
					}
				}
			}
			j++;
		}
		i++;
	}
	return 0;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_create_three_symbol_set_step_1(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size)
{
	int i, j, k, set22 = 0, set13 = 0, set23 = 0, s1, f;


	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (options_table(i,j,0) == 1)
			{
				s1 = s_table(i,j); f = 0;
				for (k = 0; k < size; k++)
				{
					if (options_table(k,j,0) == 2 && options_table(k,j,s1) > 0) { f = 1; }
					if (options_table(i,k,0) == 2 && options_table(i,k,s1) > 0) { f = 1; }
				}

				if (f == 0)
				{
					m1_create_three_symbol_set_fx2(options_table, p_table, s_table, s1, i, j, size, blk_size);
				}
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_create_three_symbol_set_step_3(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size)
{
	int i, j, s1, s2, flag = 0;

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (options_table(i,j,0) == 2)
			{
				s1 = s_table(i,j);
				s2 = 1; while (options_table(i,j,s2) == 0 || options_table(i,j,s2) == s1) { s2++; }
				options_table(i,j,size + 2) = -1;

				flag = m1_create_three_symbol_set_fx1(options_table, p_table, s_table, s1, s2, i, j, size, blk_size);
				if (flag == 0)
				{
					m1_create_three_symbol_set_fx2(options_table, p_table, s_table, s1, i, j, size, blk_size);
				}
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_create_three_symbol_set_fx1(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int s1, int s2, int x, int y, int size, int blk_size)
{
	int  s3 = 0, s4 = 0, j,  flag = 0;
	int x1, y1, x2, y2;

	x1 = 0; while (s_table(x1,y) != s2) { x1++; }
	y1 = 0; while (s_table(x,y1) != s2) { y1++; }

	if (options_table(x,y1,0) > 3 && options_table(x1,y,0) > 3)
	{
		if (options_table(x1,y,s1) > 0 && options_table(x,y1,s1) == 0) { y1 = y; }
		else if (options_table(x1,y,s1) == 0 && options_table(x,y1,s1) > 0) { x1 = x; }
		else if (options_table(x1,y,0) < options_table(x,y1,0)) { y1 = y; }
		else { x1 = x; }

		if (options_table(x1,y1,s1) > 0)
		{
			s3 = s1;
			flag = 1;
			m1_select_third_symbol(options_table, s_table, p_table, s2, s3, s4, x1, y1, size, blk_size);
		}
		else
		{
			m1_select_second_symbol(options_table, s_table, p_table, s2, s3, x1, y1, size, blk_size);
			m1_select_third_symbol(options_table, s_table, p_table, s2, s3, s4, x1, y1, size, blk_size);
		}

		//*******************************************************************
		if (s3 > 0 && s4 > 0)
		{

			for (j = 1; j < size + 1; j++)
			{
				if (options_table(x1,y1,j) > 0 && j != s2 && j != s3 && j != s4)
				{
					x2 = 0;y2 = 0;
					while (s_table(x2,y1) != j) { x2++; }
					while (s_table(x1,y2) != j) { y2++; }

					if (options_table(x1,y2,0) > options_table(x2,y1,0))
					{
						p_table(x1,y2) = s_table(x1,y2);
						m1_remove_symbol_from_options_table(options_table, j, x1, y2, size, blk_size);
					}
					else
					{
						p_table(x2,y1) = s_table(x2,y1);
						m1_remove_symbol_from_options_table(options_table, j, x2, y1, size, blk_size);
					}
				}
			}
		}
	}
	else if (options_table(x1,y,0) < 4 && options_table(x1,y,s1)>0) { flag = 1; }
	else if (options_table(x,y1,0) < 4 && options_table(x,y1,s1)>0) { flag = 1; }

	return flag;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_create_three_symbol_set_fx2(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int s1, int x, int y, int size, int blk_size)
{
	int i, j, k, c = size + 1, f1, f2;
	int x1 = -1, y1 = -1, x2, y2;
	int s2 = 0, s3 = 0;
	for (i = 0; i < size; i++)
	{
		if (i != y && options_table(x,i,s1) > 0)
		{
			k = s_table(x,i);
			if (m1_check_nearest_neighbour_condition_hrz(p_table, k, x, y, i, size, blk_size) == 0)
			{
				if (options_table(x,i,0) < c) { c = options_table(x,i,0); x1 = x; y1 = i; s2 = k; }
			}
		}
		if (i != x && options_table(i,y,s1) > 0)
		{
			k = s_table(i,y);
			if (m1_check_nearest_neighbour_condition_vrt(p_table, k, x, i, y, size, blk_size) == 0)
			{
				if (options_table(i,y,0) < c) { c = options_table(i,y,0); x1 = i; y1 = y; s2 = k; }
			}
		}
	}

	//***********************************************
	if (x1 > -1 && c > 3)
	{
		c = size + 1;
		for (k = 1; k < size + 1; k++)
		{
			if (options_table(x1,y1,k) > 0 && k != s1 && k != s2)
			{
				i = 0; while (s_table(i,y1) != k) { i++; }
				j = 0; while (s_table(x1,j) != k) { j++; }

				f1 = m1_check_nearest_neighbour_condition_vrt(p_table, k, x1, i, y1, size, blk_size);
				f2 = m1_check_nearest_neighbour_condition_hrz(p_table, k, x1, j, y1, size, blk_size);

				if (f1 == 0 && f2 == 0)
				{
					if (options_table(x1,j,0) < options_table(i,y1,0)) { c = options_table(x1,j,0); s3 = k; }
					else { c = options_table(i,y1,0); s3 = k; }
				}
				else if (f1 == 0) { c = options_table(i,y1,0); s3 = k; }
				else if (f2 == 0) { c = options_table(x1,j,0); s3 = k; }
			}
		}
		if (s3 > 0)
		{
			for (j = 1; j < size + 1; j++)
			{
				if (options_table(x1,y1,j) > 0 && j != s1 && j != s2 && j != s3)
				{
					x2 = 0;y2 = 0;
					while (s_table(x2,y1) != j) { x2++; }
					while (s_table(x1,y2) != j) { y2++; }

					if (options_table(x1,y2,0) > options_table(x2,y1,0))
					{
						p_table(x1,y2) = s_table(x1,y2);
						m1_remove_symbol_from_options_table(options_table, j, x1, y2, size, blk_size);
					}
					else
					{
						p_table(x2,y1) = s_table(x2,y1);
						m1_remove_symbol_from_options_table(options_table, j, x2, y1, size, blk_size);
					}
				}
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_build_Options_table(Grid2D& p_table, Grid3D& options_table, int size)
{
	int i = 0, j = 0, k = 0, l = 0, s = 0, bx = 0, by = 0, bl = int(sqrt(size));
	//initalise
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			for (k = 0; k < size + 1; k++)
			{
				if (p_table(i,j) == 0) { options_table(i,j,k) = k; options_table(i,j,0) = size; }
				else { options_table(i,j,k) = 0; }
			}
			options_table(i,j,size + 1) = 0;
			options_table(i,j,size + 2) = 0;
		}
	}
	//***********************************************
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			//horizotal maping
			for (k = 0; k < size; k++)
			{
				s = p_table(i,k);
				if (s > 0 && options_table(i,j,0) > 0)
				{
					options_table(i,j,s) = 0; options_table(i,j,0)--;
				}
			}
			//verticle maping
			for (k = 0; k < size; k++)
			{
				s = p_table(k,j);
				if (s > 0 && options_table(i,j,s) > 0 && options_table(i,j,0) > 0)
				{
					options_table(i,j,s) = 0; options_table(i,j,0)--;
				}
			}
			//block maping
			bx = (i / bl) * bl; by = (j / bl) * bl;
			for (k = bx; k < bx + bl; k++)
			{
				for (l = by; l < by + bl; l++)
				{
					s = p_table(k,l);
					if (s > 0 && options_table(i,j,s) > 0 && options_table(i,j,0) > 0)
					{
						options_table(i,j,s) = 0; options_table(i,j,0)--;
					}
				}
			}
		}
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_remove_single_symbol_set(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size)
{
	int i = 0, j = 0, k = 0, s1, flag = 1;

	while (flag == 1)
	{
		flag = 0;
		for (i = 0; i < size; i++)
		{
			for (j = 0; j < size; j++)
			{
				if (options_table(i,j,0) == 1)
				{
					flag = 1; s1 = s_table(i,j);
					m1_remove_symbol_from_options_table(options_table, s1, i, j, size, blk_size);
				}
			}
		}
	}

	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_remove_two_symbol_set(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int blk_size)
{
	int i = 0, j = 0, k = 0, s;

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (options_table(i,j,0) == 2 && options_table(i,j,size + 2) == -1)
			{
				s = s_table(i,j);
				for (k = 0; k < size + 1; k++) { options_table(i,j,k) = 0; }
				m1_remove_symbol_from_options_table(options_table, s, i, j, size, blk_size);
			}
		}
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_remove_symbol_from_options_table(Grid3D& options_table, int s, int x, int y, int size, int blk_size)
{
	int i, j;
	int bx = (x / blk_size) * blk_size, by = (y / blk_size) * blk_size;
	//hrz
	for (i = 0; i < size; i++)
	{
		if (options_table(x,i,s) > 0)
		{
			options_table(x,i,s) = 0; options_table(x,i,0)--;
		}
	}

	//vrt
	for (i = 0; i < size; i++)
	{

		if (options_table(i,y,s) > 0)
		{
			options_table(i,y,s) = 0; options_table(i,y,0)--;
		}
	}
	//block
	for (i = bx; i < bx + blk_size; i++)
	{
		for (j = by; j < by + blk_size; j++)
		{
			if (options_table(i,j,s) > 0)
			{
				options_table(i,j,s) = 0; options_table(i,j,0)--;

			}
		}
	}
	for (i = 0; i < size + 1; i++) { options_table(x,y,i) = 0; }
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_find_least_count_set_size(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int size, int& lss, int& set1_count, int& set2_count, int& set3_count)
{
	int i, j, c = 0, c1 = 0;

	set1_count = 0; lss = size; set2_count = 0; set3_count = 0;

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (options_table(i,j,0) > 0)
			{
				if (options_table(i,j,0) == 1) { set1_count++; }
				else if (options_table(i,j,0) == 2) { set2_count++; }
				else { c1++; }

				if (options_table(i,j,0) != c) { c = options_table(i,j,0); set3_count++; }
				if (options_table(i,j,0) < lss) { lss = options_table(i,j,0); }
			}
		}
	}
	cout << "Number of sets to be removed from options table " << c1 << "\r";
	cout.flush();

	if (set3_count == 1)
	{
		for (i = 0; i < size; i++)
		{
			for (j = 0; j < size; j++)
			{
				if (options_table(i,j,0) > 0)
				{
					p_table(i,j) = s_table(i,j);
					options_table(i,j,0) = 0;
				}
			}
		}
		set3_count = 0;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_check_nearest_neighbour_condition_hrz(Grid2D& table, int s, int x, int y1, int y2, int size, int blk_size)
{
	int i, j, k, f = 0;
	int bx = (x / blk_size) * blk_size;
	int by1 = (y1 / blk_size) * blk_size, by2 = (y2 / blk_size) * blk_size;


	if (by1 < by2 && by2 - by1 > blk_size)
	{
		for (k = by1; k < by2; k = k + blk_size)
		{
			for (i = bx; i < bx + blk_size; i++)
			{
				for (j = k; j < k + blk_size; j++) { if (table(i,j) == s) { f = 1; } }
			}
		}
	}
	else if (by2 < by1 && by1 - by2 > blk_size)
	{
		for (k = by2; k < by1; k = k + blk_size)
		{
			for (i = bx; i < bx + blk_size; i++)
			{
				for (j = k; j < k + blk_size; j++) { if (table(i,j) == s) { f = 1; } }
			}
		}

	}
	return f;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_check_nearest_neighbour_condition_vrt(Grid2D& table, int s, int x1, int x2, int y, int size, int blk_size)
{
	int i, j, k, f = 0;
	int by = (y / blk_size) * blk_size;
	int bx1 = (x1 / blk_size) * blk_size, bx2 = (x2 / blk_size) * blk_size;

	if (bx1 < bx2 && bx2 - bx1 > blk_size)
	{
		for (k = bx1; k < bx2; k = k + blk_size)
		{
			for (i = k; i < k + blk_size; i++)
			{
				for (j = by; j < by + blk_size; j++) { if (table(i,j) == s) { f = 1; } }
			}
		}
	}
	else if (bx2 < bx1 && bx1 - bx2 > blk_size)
	{
		for (k = bx2; k < bx1; k = k + blk_size)
		{
			for (i = k; i < k + blk_size; i++)
			{
				for (j = by; j < by + blk_size; j++) { if (table(i,j) == s) { f = 1; } }
			}
		}
	}
	return f;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_select_second_symbol(Grid3D& options_table, Grid2D& p_table, Grid2D& s_table, int s1, int& s2, int x, int y, int size, int blk_size)
{
	int i = 0, k, c = size + 1, f;

	s2 = 0;
	for (k = 1; k < size + 1; k++)
	{
		if (options_table(x,y,k) > 0 && k != s1)
		{
			for (i = 0; i < size; i++)
			{
				if (s_table(i,y) == k && options_table(i,y,0) < c && options_table(i,y,0)>options_table(x,y,0))
				{
					f = m1_check_nearest_neighbour_condition_vrt(s_table, k, i, x, y, size, blk_size);
					if (f == 0) { s2 = k; c = options_table(i,y,0); }
				}

				if (s_table(x,i) == k && options_table(x,i,0) < c)
				{
					f = m1_check_nearest_neighbour_condition_hrz(p_table, k, x, i, y, size, blk_size);
					if (f == 0) { s2 = k; c = options_table(x,i,0); }
				}
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int m1_select_third_symbol(Grid3D& options_table, Grid2D& s_table, Grid2D& p_table, int s1, int s2, int& s3, int x, int y, int size, int blk_size)
{
	int i, j, k, c = size + 1, f1, f2;

	s3 = 0;
	for (k = 1; k < size + 1; k++)
	{
		if (k != s1 && k != s2 && options_table(x,y,k) > 0)
		{
			i = 0; while (s_table(i,y) != k) { i++; }
			j = 0; while (s_table(x,j) != k) { j++; }

			f1 = m1_check_nearest_neighbour_condition_vrt(p_table, k, x, i, y, size, blk_size);
			f2 = m1_check_nearest_neighbour_condition_hrz(p_table, k, x, j, y, size, blk_size);

			if (f1 == 0 && f2 == 0)
			{
				if (options_table(x,j,0) < c) { c = options_table(x,j,0); s3 = k; }
				if (options_table(i,y,0) < c) { c = options_table(i,y,0); s3 = k; }
			}
			else if (f1 == 0 && options_table(i,y,0) < c) { c = options_table(i,y,0); s3 = k; }
			else if (f2 == 0 && options_table(x,j,0) < c) { c = options_table(x,j,0); s3 = k; }
		}
	}

	return 0;
}

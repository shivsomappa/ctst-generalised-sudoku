#include"filters.h"

using namespace std;



int find_CTS_sgm(Grid2D& temp_1,Grid2D& temp_2,int list_end,int size,int &c_count,int max_set_size);
int sort_temp_1_data_sgm(Grid2D& temp_1,int &size,int max_set_size);
int find_single_count_symbols(Grid2D& temp_1, Grid2D& temp_2, int max_set_size, int size);

int filter_symbol_hrz_sgm(Grid3D& p_OT,  Grid2D& temp_1,Grid2D& temp_2,int x,int size,int max_set_size, int pid, int stat_data[]);
int filter_symbol_vrt_sgm(Grid3D& p_OT,  Grid2D& temp_1,Grid2D& temp_2,int x,int size,int max_set_size, int pid, int stat_data[]);
int filter_symbol_blk_sgm(Grid3D& p_OT,  Grid2D& temp_1,Grid2D& temp_2,int bx,int by,int blk_size,int size,int max_set_size, int pid, int stat_data[]);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_symbol_sets_group_method(Grid3D& p_OT,  Grid2D& temp_1, Grid2D& temp_2,int size,int blk_size,int max_set_size, int pid, int stat_data[])
{
	int i=0,j=0, flag=0;


	while(i<size&&flag==0)
	{
		flag=filter_symbol_hrz_sgm(p_OT, temp_1,temp_2,i,size,max_set_size, pid, stat_data);
		i++;
	}

	if (flag == 0)
	{
		i = 0;
		while(i<size&&flag==0)
		{
			flag = filter_symbol_vrt_sgm(p_OT, temp_1, temp_2, i, size, max_set_size, pid, stat_data);
			i++;
		}
	}

	if (flag == 0)
	{
		i = 0;
		while(i<size&&flag==0)
		{
			j = 0;
			while(j<size&&flag==0)
			{
				flag = filter_symbol_blk_sgm(p_OT, temp_1, temp_2, i, j, blk_size, size, max_set_size, pid, stat_data);
				j = j + blk_size;
			}
			i = i + blk_size;
		}
	}

	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_symbol_hrz_sgm(Grid3D& p_OT,  Grid2D& temp_1,Grid2D& temp_2,int x,int size,int max_set_size, int pid, int stat_data[])
{
	int i=0,j=0,k=0, flag =0, list_end=0,cc=0,c_count,set_1=0;
	int x1,y1;

	for(i=0;i<size;i++)
	{
		if (p_OT(x,i,0) > 0) {cc++;}
		if (p_OT(x,i,size + 1) == pid) { flag = 1; }
	}

	//***********************************************************************
	if(cc > 4 && flag==1)
	{
		flag = 0;
		for(j = 0; j < size; j++)
		{
			if(p_OT(x,j,0) > 0)
			{
				i = 0;
				for (k = 0; k < size + 1; k++)
				{
					if (p_OT(x,j,k) > 0)
					{
						temp_1(list_end,i) = p_OT(x,j,k); i++;
					}
				}
				while (i < max_set_size) { temp_1(list_end,i) = 0; i++; }
				temp_1(list_end,max_set_size) = x;
				temp_1(list_end,max_set_size + 1) = j;
				list_end++;
			}
		}
		//***********************************************************************
		sort_temp_1_data_sgm(temp_1, list_end, max_set_size);

		if (list_end > 3)
		{
			flag = find_CTS_sgm(temp_1, temp_2, list_end, size, c_count, max_set_size);

			if (flag == 1)
			{
				flag = 0;
				if (find_single_count_symbols(temp_1, temp_2, max_set_size, size) == 1)
				{
					for (i = c_count; i < list_end; i++)
					{
						x1 = temp_1(i,max_set_size);
						y1 = temp_1(i,max_set_size + 1);
						for (k = 1; k < size + 1; k++)
						{
							if (p_OT(x1,y1,k) > 0 && temp_2(k,0) > 0)
							{
								p_OT(x1,y1,k) = 0;
								p_OT(x1,y1,0)--;			
								p_OT(x1,y1,size + 1) = pid;
								stat_data[8]++;
								flag = 1;
							}
						}
					}
				}
			}
		}
	}
	else { flag = 0; }

	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_symbol_vrt_sgm(Grid3D& p_OT,  Grid2D& temp_1,Grid2D& temp_2,int y,int size,int max_set_size, int pid, int stat_data[])
{
	int i=0,j=0,k=0, flag=0, list_end=0,cc=0,c_count,set_1=0;
	int x1,y1;


	for(i=0;i<size;i++)
	{
		if (p_OT(i,y,0) > 0) { cc++; }
		if (p_OT(i,y,size + 1) == pid) { flag = 1; }
	}
	//***********************************************************************
	if (cc > 4 && flag==1)
	{
		flag = 0;
		for(i=0;i<size;i++)
		{
			if(p_OT(i,y,0)>0)
			{
				j = 0;
				for (k = 0; k < size + 1; k++)
				{
					if (p_OT(i,y,k) > 0) { temp_1(list_end,j) = p_OT(i,y,k); j++; }
				}
				while (j < max_set_size) { temp_1(list_end,j) = 0; j++; }
				temp_1(list_end,max_set_size) = i;
				temp_1(list_end,max_set_size + 1) = y;
				list_end++;
			}
		}
		//***********************************************************************
		sort_temp_1_data_sgm(temp_1,list_end,max_set_size);

		if (list_end > 3)
		{
			flag = find_CTS_sgm(temp_1, temp_2, list_end, size, c_count, max_set_size);

			if (flag == 1)
			{
				flag = 0;
				if (find_single_count_symbols(temp_1, temp_2, max_set_size, size) == 1)
				{
					for (i = c_count;i < list_end;i++)
					{
						x1 = temp_1(i,max_set_size);y1 = temp_1(i,max_set_size + 1);
						for (k = 1;k < size + 1;k++)
						{
							if (p_OT(x1,y1,k) > 0 && temp_2(k,0) > 0)
							{
								p_OT(x1,y1,k) = 0;
								p_OT(x1,y1,0)--;		
								p_OT(x1,y1,size + 1) = pid;
								stat_data[8]++;
								flag = 1;
							}
						}
					}
				}
			}
		}
	}
	else { flag = 0; }

	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_symbol_blk_sgm(Grid3D& p_OT,  Grid2D& temp_1,Grid2D& temp_2,int bx,int by,int blk_size,int size,int max_set_size, int pid, int stat_data[])
{
	int i=0,j=0,k=0, flag=0, list_end=0,cc=0,c_count,set_1=0;
	int x1, y1;


	for(i=bx;i<bx+blk_size;i++)
	{
		for(j=by;j<by+blk_size;j++)
		{
			if (p_OT(i,j,0) > 0) { cc++; }
			if (p_OT(i,j,size + 1) == pid) { flag = 1; }
		}
	}
	//***********************************************************************
	if(cc>4 && flag==1)
	{
		flag = 0;
		for(i=bx;i<bx+blk_size;i++)
		{
			for(j=by;j<by+blk_size;j++)
			{
				if(p_OT(i,j,0)>0)
				{
					cc = 0;
					for (k = 0; k < size + 1; k++)
					{
						if (p_OT(i,j,k) > 0) { temp_1(list_end,cc) = p_OT(i,j,k); cc++; }
					}
					while (cc < max_set_size) { temp_1(list_end,cc) = 0; cc++; }
					temp_1(list_end,max_set_size) = i;
					temp_1(list_end,max_set_size + 1) = j;
					list_end++;
				}
			}
		}
		//***********************************************************************
		sort_temp_1_data_sgm(temp_1,list_end,max_set_size);
		if (list_end > 3)
		{
			flag = find_CTS_sgm(temp_1, temp_2, list_end, size, c_count, max_set_size);

			if (flag == 1)
			{
				flag = 0;
				if (find_single_count_symbols(temp_1, temp_2, max_set_size, size) == 1)
				{
					for (i = c_count;i < list_end;i++)
					{
						x1 = temp_1(i,max_set_size);y1 = temp_1(i,max_set_size + 1);
						for (k = 1;k < size + 1;k++)
						{
							if (p_OT(x1,y1,k) > 0 && temp_2(k,0) > 0)
							{
								p_OT(x1,y1,k) = 0;
								p_OT(x1,y1,0)--;			
								p_OT(x1,y1,size + 1) = pid;
								stat_data[8]++;
								flag = 1;
							}
						}
					}
				}
			}
		}
	}
	else { flag = 0; }
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_CTS_sgm(Grid2D& temp_1,Grid2D& temp_2,int list_end,int size,int &c_count,int max_set_size)
{
	int s,i,j,k,flag=0,start=1,end=list_end,shift_count=0;
	int s_count=0,c1,c2;


	while(flag==0)//end>2&&s_count!=c_count)
	{
		s_count=temp_1(0,0);c_count=1;start=1;
		for(i=0;i<size+1;i++){temp_2(i,0) = 0; }
		for(i=1;i<max_set_size;i++){if(temp_1(0,i)>0){s=temp_1(0,i);temp_2(s,0) = 1; } }
		//**************************************************
		while(start<end-1&&flag==0)
		{
			c2=max_set_size;
			for(i=start;i<list_end;i++)
			{
				c1=0;
				for(k=1;k<max_set_size;k++)
				{
					if(temp_1(i,k)>0){s=temp_1(i,k);if(temp_2(s,0) == 0) { c1++; } }
				}
				if(c1<c2){j=i;c2=c1;}
				else if (c1 == c2 && temp_1(j,0) < temp_1(i,0)) { j = i; c2 = c1; }
			}

			//****************************
			if(c2<2)
			{
				c_count++;s_count=s_count+c2;
				for(k=0;k<max_set_size+3;k++)
				{
					s=temp_1(j,k);temp_1(j,k)=temp_1(start,k);temp_1(start,k)=s;
					if(k>0&&k<max_set_size&&s>0){temp_2(s,0)++; }
				}
			}
			if (c_count == s_count-1 || shift_count > end) { flag = 1; }
			start++;
		}
		//**************************************************
		
		if(flag==0)
		{
			k = 0;
			while (k < c_count)
			{
				for (j = 0; j < max_set_size + 3; j++) { temp_1(end,j) = temp_1(0,j); }

				for (i = 0; i < end; i++)
				{
					for (j = 0; j < max_set_size + 3; j++) { temp_1(i,j) = temp_1(i + 1,j); }
				}
				k++;
			}
			shift_count++;
		}
	}

	if(c_count==s_count-1 && c_count>1){return 1;}
	else{return 0;}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sort_temp_1_data_sgm(Grid2D& temp_1,int &size,int max_set_size)
{
	int i,j,k,s, cc=size;

	for (i = 0; i < size; i++)
	{
		if (temp_1(i,0) == 2)
		{
			for (j = i + 1; j < size; j++)
			{
				if (temp_1(j,0) == 2 && temp_1(j,1) == temp_1(i,1) && temp_1(j,2) == temp_1(i,2))
				{
					temp_1(i,0) = 0; temp_1(i,1) = 0; temp_1(i,2) = 0;
					temp_1(j,0) = 0; temp_1(j,1) = 0; temp_1(j,2) = 0;
					cc = cc - 2;
				}
			}
		}
	}
	for(i=0;i<size-1;i++)
	{
		for(j=i+1;j<size;j++)
		{
			if(temp_1(i,0)<temp_1(j,0))
			{
				for(k=0;k<max_set_size+3;k++)
				{
					s=temp_1(i,k);temp_1(i,k)=temp_1(j,k);temp_1(j,k)=s;
				}
			}
		}
	}
	size = cc;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_single_count_symbols(Grid2D& temp_1, Grid2D& temp_2, int max_set_size, int size)
{
	int i, s, c = 0, flag = 0;

	for (i = 1;i < max_set_size;i++)
	{
		if (temp_1(0,i) > 0)
		{
			s = temp_1(0,i);
			if (temp_2(s,0) == 1) { c++; }
		}

	}

	if (c > 1)
	{
		for (i = 1;i < max_set_size;i++)
		{
			if (temp_1(0,i) > 0)
			{
				s = temp_1(0,i);
				if (temp_2(s,0) == 1) { temp_2(s,0) = 0; }
			}
		}
		return 1;		

	}
	else { return 0; }
}

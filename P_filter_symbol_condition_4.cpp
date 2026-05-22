// =============================================================================
// P_filter_symbol_condition_4.cpp
// N-symbol N-cell tally filter (descending order).
//
// Scans rows, columns, and blocks for a set of N cells whose combined candidate
// list contains exactly N distinct symbols (descending pass: starts from the
// largest candidate groups).  Any symbol in that tally group that appears in a
// cell NOT in the group can be eliminated.
// Exposed entry point: find_n_symbol_n_cell_tally_dsc()
// =============================================================================

#include"filters.h"

using namespace std;

int find_CTS_dsc(Grid2D& tally_buf,Grid2D& work_buf,int list_end,int size,int &c_count,int max_options);
int sort_temp_1_data_dsc(Grid2D& tally_buf,int &size,int max_options);


int filter_symbol_hrz_dsc(Grid3D& opt_tbl, Grid2D& tally_buf,Grid2D& work_buf,int x,int size,int max_options, int pred_id, int stats[]);
int filter_symbol_vrt_dsc(Grid3D& opt_tbl, Grid2D& tally_buf,Grid2D& work_buf,int x,int size,int max_options, int pred_id, int stats[]);
int filter_symbol_blk_dsc(Grid3D& opt_tbl, Grid2D& tally_buf,Grid2D& work_buf,int bx,int by,int block_size,int size,int max_options, int pred_id, int stats[]);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_n_symbol_n_cell_tally_dsc(Grid3D& opt_tbl, Grid2D& tally_buf, Grid2D& work_buf,int size,int block_size,int max_options, int pred_id, int stats[])
{
	int i=0,j=0, flag=0;


	while(i<size&&flag==0)
	{
		flag=filter_symbol_hrz_dsc(opt_tbl,tally_buf,work_buf,i,size,max_options, pred_id, stats);
		i++;
	}

	if (flag == 0)
	{
		i = 0;
		while(i<size&&flag==0)
		{
			flag = filter_symbol_vrt_dsc(opt_tbl,tally_buf, work_buf, i, size, max_options, pred_id, stats);
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
				flag = filter_symbol_blk_dsc(opt_tbl, tally_buf, work_buf, i, j, block_size, size, max_options, pred_id, stats);
				j = j + block_size;
			}
			i = i + block_size;
		}
	}

	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_symbol_hrz_dsc(Grid3D& opt_tbl, Grid2D& tally_buf,Grid2D& work_buf,int x,int size,int max_options, int pred_id, int stats[])
{
	int i=0,j=0,k=0, flag =0, list_end=0,cc=0,c_count,set_1=0;
	int x1,y1;

	for(i=0;i<size;i++)
	{
		if (opt_tbl(x,i,0) > 0) {cc++;}
		if (opt_tbl(x,i,size + 1) == pred_id) { flag = 1; }
	}

	//***********************************************************************
	if(cc > 4 && flag==1)
	{
		flag = 0;
		for(j = 0; j < size; j++)
		{
			if(opt_tbl(x,j,0) > 0)
			{
				i = 0;
				for (k = 0; k < size + 1; k++)
				{
					if (opt_tbl(x,j,k) > 0)
					{
						tally_buf(list_end,i) = opt_tbl(x,j,k); i++;
					}
				}
				while (i < max_options) { tally_buf(list_end,i) = 0; i++; }
				tally_buf(list_end,max_options) = x;
				tally_buf(list_end,max_options + 1) = j;				
				list_end++;
			}
		}
		//***********************************************************************
		sort_temp_1_data_dsc(tally_buf, list_end, max_options);

		if (list_end > 3)
		{
			flag = find_CTS_dsc(tally_buf, work_buf, list_end, size, c_count, max_options);

			if (flag == 1)
			{
				flag = 0;
				for (i = c_count; i < list_end; i++)
				{
					x1 = tally_buf(i,max_options);
					y1 = tally_buf(i,max_options + 1);
					for (k = 1; k < size + 1; k++)
					{
						if (opt_tbl(x1,y1,k) > 0 && work_buf(k,0) > 0)
						{
							opt_tbl(x1,y1,k) = 0;
							opt_tbl(x1,y1,0)--;		
							opt_tbl(x1,y1,size + 1) = pred_id;
							stats[7]++;
							flag = 1;
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
int filter_symbol_vrt_dsc(Grid3D& opt_tbl, Grid2D& tally_buf,Grid2D& work_buf,int y,int size,int max_options, int pred_id, int stats[])
{
	int i=0,j=0,k=0, flag=0, list_end=0,cc=0,c_count,set_1=0;
	int x1, y1;


	for(i=0;i<size;i++)
	{
		if (opt_tbl(i,y,0) > 0) { cc++; }
		if (opt_tbl(i,y,size + 1) == pred_id) { flag = 1; }
	}
	//***********************************************************************
	if (cc > 4 && flag==1)
	{
		flag = 0;
		for(i=0;i<size;i++)
		{
			if(opt_tbl(i,y,0)>0)
			{
				j = 0;
				for (k = 0; k < size + 1; k++)
				{
					if (opt_tbl(i,y,k) > 0) { tally_buf(list_end,j) = opt_tbl(i,y,k); j++; }
				}
				while (j < max_options) { tally_buf(list_end,j) = 0; j++; }
				tally_buf(list_end,max_options) = i;
				tally_buf(list_end,max_options + 1) = y;
				list_end++;
			}
		}
		//***********************************************************************
		sort_temp_1_data_dsc(tally_buf,list_end,max_options);
		if (list_end > 3)
		{
			flag = find_CTS_dsc(tally_buf, work_buf, list_end, size, c_count, max_options);

			if (flag == 1)
			{
				flag = 0;
				for (i = c_count;i < list_end;i++)
				{
					x1 = tally_buf(i,max_options);y1 = tally_buf(i,max_options + 1);
					for (k = 1;k < size + 1;k++)
					{
						if (opt_tbl(x1,y1,k) > 0 && work_buf(k,0) > 0)
						{
							opt_tbl(x1,y1,k) = 0;
							opt_tbl(x1,y1,0)--;		
							opt_tbl(x1,y1,size + 1) = pred_id;
							stats[7]++;
							flag = 1;
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
int filter_symbol_blk_dsc(Grid3D& opt_tbl, Grid2D& tally_buf,Grid2D& work_buf,int bx,int by,int block_size,int size,int max_options, int pred_id, int stats[])
{
	int i=0,j=0,k=0, flag=0, list_end=0,cc=0,c_count,set_1=0;
	int x1, y1;


	for(i=bx;i<bx+block_size;i++)
	{
		for(j=by;j<by+block_size;j++)
		{
			if (opt_tbl(i,j,0) > 0) { cc++; }
			if (opt_tbl(i,j,size + 1) == pred_id) { flag = 1; }
		}
	}
	//***********************************************************************
	if(cc>4 && flag==1)
	{
		flag = 0;
		for(i=bx;i<bx+block_size;i++)
		{
			for(j=by;j<by+block_size;j++)
			{
				if(opt_tbl(i,j,0)>0)
				{
					cc = 0;
					for (k = 0; k < size + 1; k++)
					{
						if (opt_tbl(i,j,k) > 0) { tally_buf(list_end,cc) = opt_tbl(i,j,k); cc++; }
					}
					while (cc < max_options) { tally_buf(list_end,cc) = 0; cc++; }
					tally_buf(list_end,max_options) = i;
					tally_buf(list_end,max_options + 1) = j;
					list_end++;
				}
			}
		}
		//***********************************************************************
		sort_temp_1_data_dsc(tally_buf,list_end,max_options);
		if (list_end > 3)
		{
			flag = find_CTS_dsc(tally_buf, work_buf, list_end, size, c_count, max_options);

			if (flag == 1)
			{
				flag = 0;
				for (i = c_count;i < list_end;i++)
				{
					x1 = tally_buf(i,max_options);y1 = tally_buf(i,max_options + 1);
					for (k = 1;k < size + 1;k++)
					{
						if (opt_tbl(x1,y1,k) > 0 && work_buf(k,0) > 0)
						{
							opt_tbl(x1,y1,k) = 0;
							opt_tbl(x1,y1,0)--;		
							opt_tbl(x1,y1,size + 1) = pred_id;
							stats[7]++;
							flag = 1;
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
int find_CTS_dsc(Grid2D& tally_buf,Grid2D& work_buf,int list_end,int size,int &c_count,int max_options)
{
	int s,i,j,k,flag=0,start=1,end=list_end,shift_count=0;
	int s_count=0,c1,c2;


	while(flag==0)//end>2&&s_count!=c_count)
	{
		s_count=tally_buf(0,0);c_count=1;start=1;
		for(i=0;i<size+1;i++){work_buf(i,0) = 0; }
		for(i=1;i<max_options;i++){if(tally_buf(0,i)>0){s=tally_buf(0,i);work_buf(s,0) = 1; } }
		//**************************************************
		while(start<end-1&&flag==0)
		{
			c2=max_options;
			for(i=start;i<list_end;i++)
			{
				c1=0;
				for(k=1;k<max_options;k++)
				{
					if(tally_buf(i,k)>0){s=tally_buf(i,k);if(work_buf(s,0) == 0) { c1++; } }
				}
				if(c1<c2){j=i;c2=c1;}
				else if (c1 == c2 && tally_buf(j,0) < tally_buf(i,0)) { j = i; c2 = c1; }
			}

			//****************************
			if(c2<2)
			{
				c_count++;s_count=s_count+c2;
				for(k=0;k<max_options+3;k++)
				{
					s=tally_buf(j,k);tally_buf(j,k)=tally_buf(start,k);tally_buf(start,k)=s;
					if(k>0&&k<max_options&&s>0){work_buf(s,0)++; }
				}
			}
			if (c_count == s_count || shift_count > end) { flag = 1; }
			start++;
		}
		//**************************************************
		
		if(flag==0)
		{
			k = 0;
			while (k < c_count)
			{
				for (j = 0; j < max_options + 3; j++) { tally_buf(end,j) = tally_buf(0,j); }

				for (i = 0; i < end; i++)
				{
					for (j = 0; j < max_options + 3; j++) { tally_buf(i,j) = tally_buf(i + 1,j); }
				}
				k++;
			}
			shift_count++;
		}
	}

	if(c_count==s_count&&c_count<list_end){return 1;}
	else{return 0;}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sort_temp_1_data_dsc(Grid2D& tally_buf,int &size,int max_options)
{
	int i,j,k,s, cc=size;

	for (i = 0; i < size; i++)
	{
		if (tally_buf(i,0) == 2)
		{
			for (j = i + 1; j < size; j++)
			{
				if (tally_buf(j,0) == 2 && tally_buf(j,1) == tally_buf(i,1) && tally_buf(j,2) == tally_buf(i,2))
				{
					tally_buf(i,0) = 0; tally_buf(i,1) = 0; tally_buf(i,2) = 0;
					tally_buf(j,0) = 0; tally_buf(j,1) = 0; tally_buf(j,2) = 0;
					cc = cc - 2;
				}
			}
		}
	}
	for(i=0;i<size-1;i++)
	{
		for(j=i+1;j<size;j++)
		{
			if(tally_buf(i,0)<tally_buf(j,0))
			{
				for(k=0;k<max_options+3;k++)
				{
					s=tally_buf(i,k);tally_buf(i,k)=tally_buf(j,k);tally_buf(j,k)=s;
				}
			}
		}
	}
	size = cc;
	return 0;
}

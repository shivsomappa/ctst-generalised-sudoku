#include"filters.h"
using namespace std;

int filter_twin_sets_hrz(Grid3D& p_OT,int s1,int s2,int x,int y,int size,int blk_size, int pid, int stat_data[]);
int filter_twin_sets_vrt(Grid3D& p_OT,  int s1,int s2,int x,int y,int size,int blk_size, int pid, int stat_data[]);
int filter_twin_sets_blk(Grid3D& p_OT,  int s1,int s2,int x,int y,int size,int blk_size, int pid, int stat_data[]);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int find_twin_set(Grid3D& p_OT,  int size,int blk_size, int pid, int stat_data[])
{
	int i=0,j,s1,s2,flag=0;

	while(i<size&&flag==0)
	{
		j = 0;
		while(j<size&&flag==0)
		{
			//if set size is two
			if(p_OT(i,j,0)==2)
			{
				//search for another two member set of same  symbols
				//if found remove these symbols from rest of cells options set.
				s1=1;	while(p_OT(i,j,s1)<1){s1++;}
				s2=s1+1;while(p_OT(i,j,s2)<1){s2++;}

				flag=filter_twin_sets_hrz(p_OT, s1,s2,i,j,size,blk_size, pid, stat_data);

				if (flag == 0)
				{
					flag = filter_twin_sets_vrt(p_OT, s1, s2, i, j, size, blk_size, pid, stat_data);
				}
				if (flag == 0)
				{
					flag = filter_twin_sets_blk(p_OT, s1, s2, i, j, size, blk_size, pid, stat_data);
				}
			}
			j++;
		}
		i++;
	}
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_twin_sets_hrz(Grid3D& p_OT,  int s1,int s2,int x,int y,int size,int blk_size, int pid, int stat_data[])
{
	int i, flag = 0, y1, c = 0;
	
	
	for(i=0;i<size;i++)
	{
		if(i!=y&&p_OT(x,i,0)==2&&p_OT(x,i,s1)>0&&p_OT(x,i,s2)>0) { flag = 1;y1 = i; }
	}
	//**********************************
	
	if(flag==1 &&(p_OT(x,y,size+1)==pid||p_OT(x,y1,size+1)==pid))
	{
		flag = 0;
		for(i=0;i<size;i++)
		{
			if(i!=y&&i!=y1&&p_OT(x,i,0)>0)
			{
				if(p_OT(x,i,s1)>0)
				{
					p_OT(x,i,s1)=0;
					p_OT(x,i,0)--;
					p_OT(x,i,size + 1) = pid;
					stat_data[6]++;
					flag = 1;
				}
				if(p_OT(x,i,s2)>0)
				{
					p_OT(x,i,s2)=0;
					p_OT(x,i,0)--;		
					p_OT(x,i,size + 1) = pid;
					stat_data[6]++;
					flag = 1;
				}
			}
		}

	}
	else { flag = 0; }
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_twin_sets_vrt(Grid3D& p_OT,  int s1,int s2,int x,int y,int size,int blk_size, int pid, int stat_data[])
{
	int i,flag=0,c=0,x1;
	
	//vrt map
	for(i=0;i<size;i++)
	{
		if(i!=x&&p_OT(i,y,0)==2&&p_OT(i,y,s1)>0&&p_OT(i,y,s2)>0) { flag = 1;x1 = i; }
	}
	//**********************************
	if(flag==1 &&(p_OT(x,y,size+1)==pid||p_OT(x1,y,size+1)==pid))
	{
		flag = 0;
		for(i=0;i<size;i++)
		{
			if(i!=x&&i!=x1&&p_OT(i,y,0)>0)
			{
				if(p_OT(i,y,s1)>0)
				{
					p_OT(i,y,s1)=0;
					p_OT(i,y,0)--;	
					p_OT(i,y,size + 1) = pid;
					stat_data[6]++;
					flag = 1;
				}
				if(p_OT(i,y,s2)>0)
				{
					p_OT(i,y,s2)=0;
					p_OT(i,y,0)--;		
					p_OT(i,y,size + 1) = pid;
					stat_data[6]++;
					flag = 1;
				}
			}
		}
	}
	else { flag = 0; }
	return flag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int filter_twin_sets_blk(Grid3D& p_OT,  int s1,int s2,int x,int y,int size,int blk_size, int pid, int stat_data[])
{
	int i,j,flag=0,x1,y1;
	int bx=(x/blk_size)*blk_size,by=(y/blk_size)*blk_size;
	
	for(i=bx;i<bx+blk_size;i++)
	{
		for(j=by;j<by+blk_size;j++)
		{
			if((i!=x||j!=y)&&p_OT(i,j,0)==2&&p_OT(i,j,s1)>0&&p_OT(i,j,s2)>0){flag=1;x1=i;y1=j;}
		}
	}

	//**********************************
	if(flag==1 && (p_OT(x,y,size + 1) == pid || p_OT(x1,y1,size + 1) == pid))
	{
		flag=0;
		for(i=bx;i<bx+blk_size;i++)
		{
			for(j=by;j<by+blk_size;j++)
			{
				if((i!=x||j!=y)&&(i!=x1||j!=y1)&&p_OT(i,j,0)>0)
				{
					if(p_OT(i,j,s1)>0)
					{
						p_OT(i,j,s1)=0;
						p_OT(i,j,0)--;		
						p_OT(i,j,size + 1) = pid;
						stat_data[6]++;
						flag = 1;
					}
					if(p_OT(i,j,s2)>0)
					{
						p_OT(i,j,s2)=0;
						p_OT(i,j,0)--;	
						p_OT(i,j,size + 1) = pid;
						stat_data[6]++;
						flag = 1;
					}
				}
			}
		}
	}
	else { flag = 0; }
	return flag;
}

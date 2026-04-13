#include"gsv.h"
#include"filters.h"
#include<locale.h>

using namespace std;

int char2int(int num);
int int2char(int num);

int vr(Grid2D& p_table, Grid2D& s_table, int size);

int Read_file(char file_name[200], Grid2D& table, int size);


int elapsed_time(time_t start,time_t end);

int Find_output_file_size(Grid2D& table,int size);
int Write_table_to_text_buffer(Grid2D& table, char *out,int size);
void write_file(Grid2D& table,int size,char *filename);
int check_process_halt(Grid3D& p_OT, Grid2D& p_table, Grid2D& s_table, Grid2D& t_table, int size, int& set2, int& set3);

int print_message_1(int& i);
int print_message_2(const Grid3D& p_OT, const Grid2D& t_table, const Grid2D& s_table, int size, int set2, int stat_data[], time_t start, time_t end);
int display_table(Grid2D& table, int size);
int display_p_OT(const Grid3D& p_OT, int size,int add);
int display_multiple_solution_list(const Grid3D& p_OT, int size);

int SOLVE_SUDOKU(Grid3D& p_OT,Grid2D& p_table, Grid2D& s_table, Grid2D& t_table, int size,int blk_size,int key_cells_option);
int Set_info(Grid3D& p_OT,int size, int blk_size, int &max_set,int &empty_cells,int &set2,int &set3);
int remove_excess_key_cells(Grid3D& p_OT, Grid2D& p_table, Grid2D& s_table, int size, int key_cells_option, int key_cells_count, int pid);

int verify_result(const Grid3D& p_OT, const Grid2D& p_table, const Grid2D& s_table, int size);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	cout<< "Sudoku(GSV)-Generate  Solve and Verify" << endl;
	cout<< "shiv.somappa@gmail.com "<<endl;
	cout << "___________________________________________________________________________________" << endl;
	cout << endl;
	int flag = 0;
	int i,j,blk_size=0,size,set2=0,set3=0;
	int key_cells_option=0;

	
	Grid2D p_table;//puzzle board
	Grid2D s_table;//solution board
	Grid2D t_table;//temp table to collect statistics
	Grid3D p_OT;

	cout<<"Enter rank of sudoku "<<endl;
	cout<<"starting with number 3"<<endl;
	cout<<"rank 3 generates 9x9 grid sudoku"<<endl;
	cout<<"rank 4 generates 16x16 grid sudoku and so on "<<endl;

	cin>>blk_size;
	if(blk_size>2)
	{
		size=blk_size*blk_size;
		//*****************************************************
		//memory allocation
				p_table.resize(size, size);
				s_table.resize(size, size);
				t_table.resize(size, size);
		p_OT.resize(size, size, size + 5);
		//********************************************************************************

		print_message_1(key_cells_option);

		if (key_cells_option == 1)
		{
			m1_GENERAT_SUDOKU_METHOD(p_OT, p_table, s_table, size, blk_size);
		}
		else if (key_cells_option == 2)
		{
			m2_GENERAT_SUDOKU_METHOD(p_OT, p_table, s_table, size, blk_size);
		}
		else if (key_cells_option == 3)
		{
			m3_GENERAT_SUDOKU_METHOD(p_OT, p_table, s_table, size, blk_size);
		}
		else
		{
			m4_GENERAT_SUDOKU_METHOD(p_OT, p_table, s_table, size, blk_size);
		}

		for (i = 0;i < size;i++)
		{
			for (j = 0;j < size;j++) { t_table(i,j) = p_table(i,j); }
		}

		cout<<endl;
		cout<<"Puzzle generate complete , solve sudoku process is running to collect statistics"<<endl;
		cout << "___________________________________________________________________________________" << endl;
		
		//******************************************************************************************
		//solve sudoku to collect approximate data
		build_options_table(p_OT, p_table, size, blk_size);
		if (blk_size ==3)
		{
			cout << "PUZZLE BOARD" << endl;
			display_table(p_table, size);
		}
		cout << "Solving puzzle to collect statistics " << endl;
		SOLVE_SUDOKU(p_OT, p_table, s_table, t_table, size, blk_size, key_cells_option);
		if (blk_size ==3)
		{
			cout << "SOLUTION BOARD" << endl;
			display_table(p_table, size);
		}

		char puzzle[11] = "puzzle.txt";
		char solution[13] = "solution.txt";
		write_file(p_table, size, puzzle);
		write_file(s_table, size, solution);

		cout << "_____________________________________________________________________________________" << endl;
		cout << "Puzzle saved as 'puzzle.txt' in GSV folder" << endl;
		cout << "Ready solution saved as 'solution.txt' in GSV folder" << endl;
		cout << "If the size does not exceed the number of columns in the sheet, the content can be copied to the spreadsheet." << endl;
		
	

		//******************************************************

		cout<<"Enter any charecter  to close the window "<<endl;
		char exit;
		cin>>exit;
	}
	return 0;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SOLVE_SUDOKU(Grid3D& p_OT,Grid2D& p_table, Grid2D& s_table, Grid2D& t_table, int size,int blk_size, int key_cells_option)
{
	time_t t_start, t_end;
	int i,j,set_3,set_2,flag = 1;
	int err_count=0;
	int max_set=0, empty_cells_count=0, kcc=0, cfc=0;// kcc-key cells count, cfc-cells filled count
	int pid=-1;
	int stat_data[10] = {0 };


	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++) 
		{
			p_OT(i,j,size + 1) = 0; 
			if (p_OT(i,j,0) > 0)
			{
				empty_cells_count++;
				if (p_OT(i,j,0) > max_set) { max_set = p_OT(i,j,0); }
			}
		}
	}
	stat_data[0] = empty_cells_count;
	//**********************************************************************
	Grid2D p_temp_1;
		p_temp_1.resize(size + 1, max_set + 3);
	Grid2D p_temp_2;
		p_temp_2.resize(size + 1, 5);
	//**********************************************************************
	time(&t_start);
	filter_symbol_cts_method(p_OT, t_table, p_temp_1, p_temp_2, size, blk_size, max_set, 0, stat_data);
	flag = check_process_halt(p_OT, p_table, s_table, t_table, size, set_2, set_3);
	cout << endl;
	cout << "Solving sudoku wihth symbol prediction method " << endl;
	cout << "Cells filled " << "  " << "key cells prediction" << "  " << "prediction errors" << endl;
	while(flag ==1)
	{
		flag = predict_symbol(p_OT, p_table, s_table, p_temp_2, size, blk_size, pid, stat_data);

		if (flag >0)
		{
			filter_symbol_cts_method(p_OT, t_table, p_temp_1, p_temp_2, size, blk_size, max_set, pid-1, stat_data);
		}
		flag = check_process_halt(p_OT, p_table, s_table, t_table, size, set_2, set_3);
		cout << stat_data[3] << "             " << stat_data[1] << "                         " <<stat_data[2]<<"  "<< '\r';
		cout.flush();

	}

	time(&t_end);
	cout << endl;
	cout << endl;
	//**********************************************************************
	if (verify_result(p_OT,  t_table,  s_table,  size) == 1)
	{
		print_message_2(p_OT, t_table, s_table, size, set_2, stat_data, t_start, t_end);
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int check_process_halt(Grid3D& p_OT,Grid2D& p_table, Grid2D& s_table, Grid2D& t_table, int size,int &set2,int &set3)
{
	int i,j,c=0;

	set3=0;set2=0;

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if (p_OT(i,j,0) > 0)
			{
				if (p_OT(i,j,0) > 0)
				{
					if (p_OT(i,j,0) == 2) { set2++; }
					if (p_OT(i,j,0) != c) { c = p_OT(i,j,0); set3++; }
				}
			}
		}
	}
	//*************************************************************************************
	if (set3 > 1)
	{
		if (set2 > set3)
		{
			for (i = 0;i < size;i++)
			{
				for (j = 0;j < size;j++)
				{
					if (p_OT(i,j,0) > 2)
					{
						p_table(i,j) = s_table(i,j);
						t_table(i,j) = s_table(i,j);
						p_OT(i,j,0) = 0;
					}
				}
			}
			set3 = 0;
			return 0;
		}
		else { return 1; }
	}
	else {	return 2; }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int print_message_1(int& i)
{
	cout << endl;
	cout << "Enter :1   For Number of key cells- Zero " << endl;
	cout << endl;
	cout << "Enter :2   For Number of key cells- Less than N" << endl;
	cout << endl;
	cout << "Enter :3   For Number of key cells- greater than N" << endl;
	cout << "           minimum initial symbols and partila implementation " << endl;
	cout << "           of sudoku properties" << endl;
	cout << endl;
	cout << "Enter :4   For completly random initial symbol placement " << endl;
	cout << endl;

	cout << "Note: there may be variation in number of key cells " << endl;
	cin >> i;
	return 0;
}
int print_message_2(const Grid3D& p_OT,
	const Grid2D& t_table,
	const Grid2D& s_table,
	int size,
	int set2,
	int stat_data[],
	time_t start,
	time_t end)
{
	// Display option table (only for 9x9)
	if (size == 9)
	{
		cout << "OPTION TABLE\n";
		display_p_OT(p_OT, size, 0);
	}

	// Capture end time
	time(&end);

	cout << "\n\nSTATISTICS\n";

	if (set2 > 0)
		cout << "Puzzle has multiple solution\n";
	else
		cout << "Puzzle has unique solution\n";

	cout << "\n";
	cout << "Number of empty cells to be filled - " << stat_data[0] << "\n\n";
	cout << "Number of key cells                - " << stat_data[1] << "\n\n";
	cout << "Number of symbol prediction errors - " << stat_data[2] << "\n\n";

	cout << "Number of single naked             - "
		<< stat_data[3] - stat_data[1] - stat_data[4] << "\n\n";

	cout << "Number of hidden singles           - " << stat_data[4] << "\n\n";

	cout << "______________________________________________________________________________\n";
	cout << "Deterministic filters data:\n";

	cout << "Symbols filtered by Pointing Pairs and Box-Line Reduction : " << stat_data[5] << "\n";
	cout << "Symbols filtered by naked pair (twin sets)                : " << stat_data[6] << "\n";
	cout << "Symbols filtered by two-pass tally (s-c = 0)              : " << stat_data[7] << "\n";
	cout << "Symbols filtered by near tight tally (s-c = n)            : " << stat_data[8] << "\n";
	cout << "Symbols filtered by bound set (symbol first)              : " << stat_data[9] << "\n";

	// Time taken
	elapsed_time(start, end);

	cout << "\n\nNote:\n";
	cout << "Except error rectification procedure, all solving procedures are used for statistics.\n";
	cout << "Errors are fixed by cross-checking predicted symbols with the ready solution.\n\n";

	// Display multiple solutions if exist
	if (set2 > 0 && size < 82)
	{
		display_multiple_solution_list(p_OT, size);
		cout << "\n";
	}

	return 0;
}///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int display_table(Grid2D& table, int size)
{
	int i = 0, j = 0, bh, bv, blk;
	blk = int(sqrt(size)); bh = blk - 1; bv = blk - 1;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if(size<10)
			{
				if (table(i,j) == 0) { cout << "  " << "-"; }
				else { cout << "  " << table(i,j); }
				if (j == bh) { cout << "  "; bh = bh + blk; }
			}
			else
			{
				if (table(i,j) == 0) { cout << "  " << "--"; }
				else if(table(i,j)<10){ cout << "   " << table(i,j); }
				else { cout << "  " << table(i,j); }
				if (j == bh) { cout << "  "; bh = bh + blk; }
			}

		}
		bh = blk - 1;
		if (i == bv) { cout << endl; bv = bv + blk; }
		cout << endl;
	}
	return 0;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int verify_result(const Grid3D& p_OT, const Grid2D& p_table, const Grid2D& s_table, int size)
{
	int i, j, s, flag = 0;

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (p_table(i,j) > 0)
			{
				if (p_table(i,j) != s_table(i,j)) { flag = 1; }
			}
			else
			{
				s = s_table(i,j);
				if (p_OT(i,j,s) == 0) { flag = 1; }
			}
		}
	}

	if (flag == 1) { cout << "Verified result Error found" << endl; return 0; }
	else { cout << "Verified result Error not found" << endl; return 1; }

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int display_p_OT(const Grid3D& p_OT, int size,int add)
{
	int i = 0, j = 0, k = 0, bh, bv, blk;
	blk = int(sqrt(size)); bh = blk - 1; bv = blk - 1;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			for (k = 0; k < size + 1+add; k++)
			{
				if (p_OT(i,j,k)==0) { cout << ","; }
				else if (p_OT(i,j,k)<10) { cout <<p_OT(i,j,k); }
				else { cout << p_OT(i,j,k); }
			}
			if (j == bh) { cout << "  "; bh = bh + blk; }
			cout << " ";
		}
		bh = blk - 1;
		if (i == bv) { cout << endl; bv = bv + blk; }
		cout << endl;
	}
	return 0;

}

int display_multiple_solution_list(const Grid3D& p_OT, int size)
{
	int i = 0, j, s1, s2, c = 0, c1 = 0;

	// Detect multiple solution condition
	while (i < size && c == 0)
	{
		j = 0;
		while (j < size && c == 0)
		{
			if (p_OT(i, j, 0) > 0)
			{
				c = p_OT(i, j, 0);
				c1++;
			}
			j++;
		}
		i++;
	}

	if (c == 2 && c1 < 50)
	{
		cout << "Multiple solutions - Cell options\n";
		cout << "=====================================================\n";

		int col_count = 0;

		for (i = 0; i < size; i++)
		{
			for (j = 0; j < size; j++)
			{
				if (p_OT(i, j, 0) > 0)
				{
					// Find first option
					s1 = 1;
					while (p_OT(i, j, s1) == 0) { s1++; }

					// Find second option
					s2 = s1 + 1;
					while (p_OT(i, j, s2) == 0) { s2++; }

					// Compact + aligned print (supports 2-digit values)
					cout << "[";
					cout << setw(2) << i << ",";
					cout << setw(2) << j << "]";
					cout << "{";
					cout << setw(2) << s1 << ",";
					cout << setw(2) << s2 << "} ";

					col_count++;

					// 10 columns per row
					if (col_count == 10)
					{
						cout << "\n";
						col_count = 0;
					}
				}
			}
		}

		if (col_count != 0) cout << "\n";
	}

	cout << "\n\n";
	cout << "If we pre-fill the puzzle board with these symbols, "
		<< "the board will become a unique solution board.\n";

	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Find_output_file_size(Grid2D& table,int size)
{
	int i,j,out_size=size;
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if(table(i,j)<10){out_size=out_size+2;}
			else if(table(i,j)<100){out_size=out_size+3;}
			else if(table(i,j)<1000){out_size=out_size+4;}
			else if(table(i,j)<10000){out_size=out_size+5;}
			else {out_size=out_size+6;}
		}
	}
	return out_size;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Write_table_to_text_buffer(Grid2D& table, char *out,int size)
{
	int i,j,k=0,v;
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if(table(i,j)<10)
			{
				v=table(i,j);
				out[k+1]=9;
				out[k]=int2char(v);
				k=k+2;
			}
			else if(table(i,j)<100)
			{
				v=table(i,j);
				out[k+2]=9;
				out[k+1]=int2char(v%10);v=v/10;
				out[k]=int2char(v);
				k=k+3;
			}
			else if(table(i,j)<1000)
			{
				v=table(i,j);
				out[k+3]=9;
				out[k+2]=int2char(v%10);v=v/10;
				out[k+1]=int2char(v%10);v=v/10;
				out[k]=int2char(v);
				k=k+4;
			}
			else if(table(i,j)<10000)
			{
				v=table(i,j);
				out[k+4]=9;
				out[k+3]=int2char(v%10);v=v/10;
				out[k+2]=int2char(v%10);v=v/10;
				out[k+1]=int2char(v%10);v=v/10;
				out[k]=int2char(v);
				k=k+5;
			}
			else if(table(i,j)<100000)
			{
				v=table(i,j);
				out[k+5]=9;
				out[k+4]=int2char(v%10);v=v/10;
				out[k+3]=int2char(v%10);v=v/10;
				out[k+2]=int2char(v%10);v=v/10;
				out[k+1]=int2char(v%10);v=v/10;
				out[k]=int2char(v);
				k=k+6;
			}
		}
		out[k-1]=13;out[k]=10;k++;
	}
	return k;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void write_file(Grid2D& table, int size, char* filename)
{
	int out_size;
	char* out = NULL;
	out_size = Find_output_file_size(table, size);
	out = new  char[out_size];
	fstream output;

	Write_table_to_text_buffer(table, out, size);
	output.open(filename, ios::out | ios::binary);
	output.write((char*)out, sizeof(char) * out_size);
	output.close();

	delete[] out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int char2int(int num)
{
	switch (num)
	{
		case 48:return 0; break;
		case 49:return 1; break;
		case 50:return 2; break;
		case 51:return 3; break;
		case 52:return 4; break;
		case 53:return 5; break;
		case 54:return 6; break;
		case 55:return 7; break;
		case 56:return 8; break;
		case 57:return 9; break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int int2char(int num)
{
	switch (num)
	{
		case 0:return 48; break;
		case 1:return 49; break;
		case 2:return 50; break;
		case 3:return 51; break;
		case 4:return 52; break;
		case 5:return 53; break;
		case 6:return 54; break;
		case 7:return 55; break;
		case 8:return 56; break;
		case 9:return 57; break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dis_format(unsigned int num)
{
	if (num > 999999999) { cout << num; }
	else if (num > 99999999) { cout << " " << num; }
	else if (num > 9999999) { cout << "  " << num; }
	else if (num > 999999) { cout << "   " << num; }
	else if (num > 99999) { cout << "    " << num; }
	else if (num > 9999) { cout << "     " << num; }
	else if (num > 999) { cout << "      " << num; }
	else if (num > 99) { cout << "       " << num; }
	else if (num > 9) { cout << "        " << num; }
	else { cout << "         " << num; }


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int elapsed_time(time_t start,time_t end)
{
	int h1,h2,m1,m2,s1,s2,dif=end-start;

	s1=dif%60;dif=dif/60;s2=s1%10;s1=s1/10;
	m1=dif%60;dif=dif/60;m2=m1%10;m1=m1/10;
	h1=dif;h2=h1%10;h1=h1/10;
	cout<<endl;
	cout<<" Elapsed time : "<<h1<<h2<<" hrs - "<<m1<<m2<<" mins - "<<s1<<s2<<" sec "<<endl;
	cout<<endl;
	return 0;
}










/*	Nitin Negi	2020CSM1014
	Samiksha Verma	2020CSM1018
*/

#include<bits/stdc++.h>
#include<iostream>
#include<cstdlib>

using namespace std; 
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~STRUCTURE NODE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
struct node
{
	int cell_number;
	struct node* prev;
	struct node* next;
};
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~STRUCTURE NODE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Global Variables~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int N;
int bucket_capacity;
//int bucket_number = 1;
//int cell_number = 1;
int link_file_num = 0;
int ID;
int X;
int Y;
vector < pair<int, pair<int, int>> > data;
vector < pair<int, pair<int, pair<int, pair<int, int>>>> > cell;
vector< pair<struct node*, struct node*> > bucket;
int xmin, ymin, xmax, ymax;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Global Variables Ends~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~FUNCTIONS PROTOTYPE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void generate_data();
void insertion();
void Logical_Split(int partition_axis, int cell_number, int bucket_number, vector < pair<int, pair<int, int>> > &temp_vector, vector <int> temp_cell);
void Splitting_Buckets(int bucket_number, int median, char widest_spread, vector < pair<int, pair<int, int>> > &temp_vector);
void Physical_Split(int cell_number, int bucket_number, int median, char widest_spread, int data_index);
void Splitting_Scales(int partition_axis, int partition_val, int bucket_number);
void insertion(int data_index);
int extract_cell_index(int data_index);
int extract_bucket_index(int cell_number);
void print_bucket_vector();
void print_cell_vector();
int bucket_not_full(int bucket_number);
void insert_point(int data_index, int cell_number, int bucket_number);
void extract_cell(vector < pair<int, pair<int, int>> > &temp_vector, int cell_number, int bucket_number);
void extract_bucket(vector < pair<int, pair<int, int>> > &temp_vector, int bucket_number);
int solve_using_case1(int cell_number, int bucket_number, vector < pair<int, pair<int, int>> > &temp_vector);
bool sort_X(const pair<int, pair<int,int>> &a, const pair<int, pair<int,int>> &b);
bool sort_Y(const pair<int, pair<int,int>> &a, const pair<int, pair<int,int>> &b);
int find_median(vector < pair<int, pair<int, int>> > &temp_vector);
char find_widest_spread(vector < pair<int, pair<int, int>> > &temp_vector);
void add_cellnum_to_bucket(int cell_number, int bucket_number);
void traditional_approach(int x1, int y1, int x2, int y2);
void our_approach(int x1, int y1, int x2, int y2);
void link_file(int bucket_number, int data_index, vector < pair<int, pair<int, int>> > &temp_vector);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~FUNCTIONS PROTOTYPE END~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*######################################################################### MAIN STARTS #########################################################################*/
int main()
{
	cout<<"Enter total number of data points: ";
	cin>>N;
	if(N<1)	//check for valid input values
	{
		cout<<"\nINPUT IS NOT VALID\n";
		return 0;
	}
	generate_data();
	
	cout<<"Enter bucket capacity: ";
	cin>>bucket_capacity;
	
	fstream ifile;
	ifile.open("input.txt", ios::in);
	vector <int> vect_x, vect_y;
	int id, x, y;
	string line;
	while(!ifile.eof())
	{
		getline(ifile, line);
		stringstream ss(line);
		while(getline(ss,line,','))
		{
			id=stoi(line);
			getline(ss,line,',');
			x=stoi(line);
			getline(ss,line,',');
			y=stoi(line);
			data.push_back(make_pair(id, make_pair(x, y)));			
		}
	}
	ifile.close();
	
	cell.push_back(make_pair(0, make_pair(-1, make_pair(-1, make_pair(400, 400)))));
	string file_name;
	
	file_name = "bucket" + to_string(bucket.size()) + ".txt";
	//cout<<"\nfile name: "<<file_name;
	fstream ofile;
	ofile.open(file_name, ios::out);
	ofile.close();
	
	node *temp = new node;
	temp->cell_number = 0;
	temp->next = NULL;
	temp->prev = NULL;
	bucket.push_back(make_pair(temp, temp));
	
	print_bucket_vector();
	print_cell_vector();
	
	cout<<"\ndata\n";
	for(int i=0; i < data.size(); i++)
	{
		cout<<data[i].first<<" "<<data[i].second.first<<" "<<data[i].second.second<<endl;
	}
	
	//inserting points one by one
	for(int i=0; i < data.size(); i++)
	{
		//cout<<data[i].first<<" "<<data[i].second.first<<" "<<data[i].second.second<<endl;
		insertion(i);		//insertion is done one by one
		cout<<"\nAfter insertion"<<endl;
		print_bucket_vector();
		print_cell_vector();
	}
	
	int x1, y1, x2, y2;
	cout<<"\nEnter rectangle points (x1 y1 x2 y2): ";
	cin>>x1>>y1>>x2>>y2;
	
	traditional_approach(x1, y1, x2, y2);
	our_approach(x1, y1, x2, y2);
	return 0;
}
/*######################################################################### MAIN ENDS #########################################################################*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GENERATING INITIAL DATA SET~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void generate_data()
{
	fstream ofile;
	ofile.open("input.txt", ios::out);
	srand(time(NULL));
	
	for(ID = 1; ID <= N; ID++)
	{
		X = rand()%401;	
		Y = rand()%401;
		ofile<<ID<<","<<X<<","<<Y<<"\n";
	}
	ofile.close();
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END OF GENERATE_DATA()~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SORTING TRIPLE ACCORDING TO 'X' AND 'Y'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool sort_X(const pair<int, pair<int,int>> &a, const pair<int, pair<int,int>> &b) 
{ 
    return (a.second.first < b.second.first); 
}

bool sort_Y(const pair<int, pair<int,int>> &a, const pair<int, pair<int,int>> &b)  
{ 
    return (a.second.second < b.second.second); 
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SORTING 'X' AND 'Y' ENDS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*1,270,80
2,257,205
3,375,90
4,181,195
5,73,106
6,27,253
7,64,298
8,319,282
9,318,162
10,1,137*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~INSERTION STARTS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void insertion(int data_index)
{
	cout<<"\nInsertion\n";
	cout<<data[data_index].first<<" "<<data[data_index].second.first<<" "<<data[data_index].second.second<<endl;
	int cell_number;
	int bucket_number;
	cell_number = extract_cell_index(data_index);
	cout<<"\ncell number: "<<cell_number<<endl;
	bucket_number = extract_bucket_index(cell_number);
	cout<<"\nbucket number: "<<bucket_number;
	
	if(bucket_not_full(bucket_number))
	{
		cout<<"\nbucket not full";
		insert_point(data_index, cell_number, bucket_number);	//simply insert data to cell
	}
	else
	{	
		cout<<"\nbucket is full";
		//CASE1: try resolving it by removing any bucket sharing happening amongst cells
		//firstly copy bucket values to temp_vector
			
		vector < pair<int, pair<int, int>> > temp_vector;
		temp_vector.push_back(make_pair(data[data_index].first, make_pair(data[data_index].second.first, data[data_index].second.second)));
		
		cout<<"bucket_number: "<<bucket_number<<endl;
		extract_bucket(temp_vector, bucket_number);
		cout<<"here";
		int partition_axis;
		for(int i=0; i<temp_vector.size(); i++)
		{
			cout<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;
		}
		partition_axis = solve_using_case1(cell_number, bucket_number, temp_vector);	// partition_axis is 0 if case 1 is not able to solve the overflow
		cout<<"\npartition axis: "<<partition_axis<<endl;
		//if(cell[cell_number].first < bucket_capacity)
		if(partition_axis != 0)		//handle case1 here	
		{
			vector <int> temp_cell;
			node *temp = new node;
			temp = bucket[bucket_number].first;
			while (temp!=NULL)
			{
				temp_cell.push_back(temp->cell_number);
				temp = temp->next;
			}
		
			cout<<"\nextracted cells are\n";
			for(int i=0; i < temp_cell.size(); i++)
			{
				cout<<temp_cell[i]<<" ";
			}
			cout<<endl;	
			cell[cell_number].first += 1;
			Logical_Split(partition_axis, cell_number, bucket_number, temp_vector, temp_cell);		
			temp_vector.erase(temp_vector.begin(), temp_vector.end());
		}
		
		else 
		{
			cout<<"\nCASE2\n";
			int median;
			median = find_median(temp_vector);
			cout<<"\nmedian = "<<median;
			if(median > -1)		//CASE2
			{	
				Physical_Split(cell_number, bucket_number, median, find_widest_spread(temp_vector), data_index);
				Splitting_Buckets(bucket_number, median, find_widest_spread(temp_vector), temp_vector);
			}
			else				//CASE3
			{
				cout<<"CASE3";
				link_file(bucket_number, data_index, temp_vector);
			}
			temp_vector.erase(temp_vector.begin(), temp_vector.end());
		}
		
	}
	print_bucket_vector();
	print_cell_vector();
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~INSERTION ENDS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~EXTRACT CELL INDEX~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int  extract_cell_index(int data_index)
{
	cout<<"\nextract cell index";
	cout<<"\ncell size: "<<cell.size()<<endl;
	int xmin_for_point, ymax_for_point, cell_number;
	xmin_for_point = INT_MIN;
	ymax_for_point = INT_MAX;

	for(int i=0; i < cell.size(); i++)
	{
		//cout<<cell[i].second.first <<"<"<< data[data_index].second.first<<" && "<< cell[i].second.second.second.second<<">="<< data[data_index].second.second;
		if(cell[i].second.first < data[data_index].second.first && cell[i].second.second.second.second >= data[data_index].second.second)
		{
			if(cell[i].second.first > xmin_for_point)
			{
				xmin_for_point = cell[i].second.first;
				cell_number = i;
			}
			if(cell[i].second.first == xmin_for_point)
			{
				if(cell[i].second.second.second.second < cell[cell_number].second.second.second.second)
				{
					cell_number = i;
					ymax_for_point = cell[i].second.second.second.second;
				}
			}
		}
	}
	
	return cell_number;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~EXTRACT CELL INDEX ENDS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~EXTRACT BUCKET INDEX~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int extract_bucket_index(int cell_number)
{
	cout<<"\nextract bucket index\n";
	cout<<"bucket size: "<<bucket.size();
	node *temp = new node;
	for(int i=0; i < bucket.size(); i++)
	{
		temp = bucket[i].first;
		while(temp != NULL)
		{
			if(temp->cell_number == cell_number)
				return i;
			temp = temp->next;
		}
	}
	return INT_MAX;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~EXTRACT BUCKET INDEX ENDS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~PRINT BUCKET VECTOR~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void print_bucket_vector()
{
	cout<<"\nPrint Bucket Vector";
	for(int i=0; i < bucket.size(); i++)
	{
		cout<<"\nbucket"<<i<<": "<<endl;
		node *temp = new node;
		temp = bucket[i].first;
		cout<<"cell number: ";
		while(temp != NULL)
		{
			cout<<temp->cell_number<<", ";
			temp = temp->next;
		}
	}
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~PRINT BUCKET VECTOR ENDS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void print_cell_vector()
{
	cout<<"\nprint cell vector";
	for(int i=0; i < cell.size(); i++)
	{
		cout<<"\ncell "<<i<<": "<<cell[i].first<<","<<cell[i].second.first<<","<<cell[i].second.second.first<<","<<cell[i].second.second.second.first<<","<<cell[i].second.second.second.second<<endl;
	}
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~CHECK IF BUCKET NOT FULL~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int bucket_not_full(int bucket_number)
{
	cout<<"\ncheck if bucket not full";
	int num_of_entries = 0;
	node *temp = new node;
	temp = bucket[bucket_number].first;
	while(temp != NULL)
	{
		num_of_entries += cell[temp->cell_number].first;
		temp = temp->next;
	}
	if(num_of_entries == bucket_capacity)
		return 0;
	else
		return 1;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BUCKET NOT FULL ENDS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~INSERT POINT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void insert_point(int data_index, int cell_number, int bucket_number)
{
	cout<<"\ninsert point";
	string file_name;
	file_name = "bucket" + to_string(bucket_number) +".txt";
	cout<<"inserting in "<<file_name;
	ofstream ofile;
	ofile.open(file_name, ios::app);
	ofile<<data[data_index].first<<","<<data[data_index].second.first<<","<<data[data_index].second.second<<"\n";
	ofile.close();
	cell[cell_number].first += 1;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~INSERT POINT ENDS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~EXTRACT BUCKET~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void extract_bucket(vector< pair<int, pair<int, int>> > &temp_vector, int bucket_number)
{
	cout<<"\nextract bucket";
	string file_name;
	file_name = "bucket" + to_string(bucket_number) +".txt";
	fstream ifile;
	ifile.open(file_name, ios::in);
	string line;
	bool is_present = false;
	int id, x, y;
	while(!ifile.eof())
	{
		getline(ifile, line);
		stringstream ss(line);
		size_t found;
		found = line.find(".");
		if (found != string::npos)  //string found
		{
			break;
		}
		while(getline(ss,line,','))
		{
			id=stoi(line);
			getline(ss,line,',');
			x=stoi(line);
			getline(ss,line,',');
			y=stoi(line);
			temp_vector.push_back(make_pair(id, make_pair(x, y)));				
		}
	}
	sort(temp_vector.begin(), temp_vector.end());
	ifile.close();
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~EXTRACT BUCKET ENDS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~extract cell	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void extract_cell(vector < pair<int, pair<int, int>> > &temp_vector, int cell_number, int bucket_number)
{
	cout<<"\nextract cell";
	string file_name;
	file_name = "bucket" + to_string(bucket_number) +".txt";
	cout<<"\nbucket : "<<file_name<<endl;
	fstream ifile;
	ifile.open(file_name, ios::in);
	string line;
	int id, x, y;
	
	while(!ifile.eof())
	{
		getline(ifile, line);
		stringstream ss(line);
		size_t found;
		found = line.find(".");
		if (found != string::npos)  //string found
		{
			break;
		}
		while(getline(ss,line,','))
		{
			id=stoi(line);
			getline(ss,line,',');
			x=stoi(line);
			getline(ss,line,',');
			y=stoi(line);
			if(x > cell[cell_number].second.first && x <= cell[cell_number].second.second.second.first && y > cell[cell_number].second.second.first && y <= cell[cell_number].second.second.second.second)
			{
				temp_vector.push_back(make_pair(id, make_pair(x, y)));
			}
							
		}
	}
	sort(temp_vector.begin(), temp_vector.end());
	ifile.close();
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~extract cell ends	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Solve overflow using case1	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int solve_using_case1(int cell_number, int bucket_number, vector < pair<int, pair<int, int>> > &temp_vector)
{
	int count1, count2, count3, count4;
	count1 = count2 = count3 = count4 = 0;
	for(int i=0; i<temp_vector.size(); i++)
	{
		if(temp_vector[i].second.first > cell[cell_number].second.first)
			count1++;
		if(temp_vector[i].second.first <= cell[cell_number].second.second.second.first)
			count2++;
		if(temp_vector[i].second.second > cell[cell_number].second.second.first)
			count3++;
		if(temp_vector[i].second.second <= cell[cell_number].second.second.second.second)
			count4++;
	}
	cout<<"\ncounts"<<count1<<","<<count2<<","<<count3<<","<<count4<<endl;
	if(count1 != bucket_capacity+1)
		return 1;
	if(count2 != bucket_capacity+1)
		return 2;
	if(count3 != bucket_capacity+1)
		return 3;
	if(count4 != bucket_capacity+1)
		return 4;
	else
		return 0;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Case 1 ends~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Physical_Split~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Physical_Split(int cell_number, int bucket_number, int median, char widest_spread, int data_index)
{
	cout<<"\nPhysical_Split\n";
	cout<<"Widest spread"<<widest_spread;
	vector < pair<int, pair<int, int>> > temp_vector;
	cell[cell_number].first += 1;
	int count = 0;
	node *ptr = new node;
	ptr = NULL;
	bucket.push_back(make_pair(ptr, ptr));
	cout<<"\nbn: "<<bucket_number<<"cn: "<<cell_number<<endl;
	if(widest_spread == 'x')
	{
		for(int i=0; i < cell.size(); i++)
		{
			if(median > cell[i].second.first && median < cell[i].second.second.second.first)
			{
				if(i == cell_number)
					temp_vector.push_back(data[data_index]);
				extract_cell(temp_vector, i, extract_bucket_index(i));
				cout<<"\nextracted cell\n";
				for(int j=0; j < temp_vector.size(); j++)
				{
					cout<<temp_vector[j].first<<","<<temp_vector[j].second.first<<","<<temp_vector[j].second.second<<endl;
				}
				for(int j=0; j < temp_vector.size(); j++)
				{
					if(temp_vector[j].second.first <= median)
						count++;
				}
				cout<<"count: "<<count;
				
				cell.push_back(make_pair((cell[i].first-count), make_pair(median, make_pair(cell[i].second.second.first, make_pair(cell[i].second.second.second.first, cell[i].second.second.second.second)))));
				//
				cell[i].second.second.second.first = median;
				cell[i].first = count;
				count = 0;
				
				
				if(i == cell_number)
				{
					if(bucket[bucket.size()-1].first == NULL)
					{
						node *temp = new node;
						temp->cell_number = cell.size()-1;
						temp->next = NULL;
						temp->prev = NULL;
						bucket[bucket.size()-1].first = temp;
						bucket[bucket.size()-1].second = temp;
					}
					else
					{
						add_cellnum_to_bucket(cell.size()-1, bucket.size()-1);
					}
				}
				else
				{
					node *temp = new node;
					temp = bucket[extract_bucket_index(i)].second;
					node *newnode = new node;
					newnode->cell_number = cell.size()-1;
					newnode->next = NULL;
					newnode->prev = temp;
					temp->next = newnode;
					bucket[extract_bucket_index(i)].second = newnode;
				}
				
				print_bucket_vector();
				print_cell_vector();
				temp_vector.erase(temp_vector.begin(), temp_vector.end());
			}
		}
	}
	else
	{
		for(int i=0; i < cell.size(); i++)
		{
			if(median > cell[i].second.second.first && median < cell[i].second.second.second.second)
			{
				
				if(i == cell_number)
					temp_vector.push_back(make_pair(data[data_index].first, make_pair(data[data_index].second.first, data[data_index].second.second)));
				
				extract_cell(temp_vector, i, extract_bucket_index(i));
				cout<<"\nextracted cell\n";
				for(int j=0; j < temp_vector.size(); j++)
				{
					cout<<temp_vector[j].first<<","<<temp_vector[j].second.first<<","<<temp_vector[j].second.second<<endl;
				}
				for(int j=0; j < temp_vector.size(); j++)
				{
					if(temp_vector[j].second.second <= median)
						count++;
				}
				cout<<"count: "<<count;
				
					cell.push_back(make_pair((cell[i].first-count) , make_pair(cell[i].second.first, make_pair(median, make_pair(cell[i].second.second.second.first, cell[i].second.second.second.second)))));
					cell[i].second.second.second.second = median;
					cell[i].first = count;
					count = 0;
				
				
				if(i == cell_number)
				{
					if(bucket[bucket.size()-1].first == NULL)
					{
						node *temp = new node;
						temp->cell_number = cell.size()-1;
						temp->next = NULL;
						temp->prev = NULL;
						bucket[bucket.size()-1].first = temp;
						bucket[bucket.size()-1].second = temp;
					}
					else
					{
						add_cellnum_to_bucket(cell.size()-1, bucket.size()-1);
					}
				}
				else
				{
					node *temp = new node;
					temp = bucket[extract_bucket_index(i)].second;
					node *newnode = new node;
					newnode->cell_number = cell.size()-1;
					newnode->next = NULL;
					newnode->prev = temp;
					temp->next = newnode;
					bucket[extract_bucket_index(i)].second = newnode;
				}
				print_bucket_vector();
				print_cell_vector();
				temp_vector.erase(temp_vector.begin(), temp_vector.end());
			}
		}
	}
	//Splitting_Buckets(int bucket_number);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Physical_Split END~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Splitting_Buckets~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void add_cellnum_to_bucket(int cell_number, int bucket_number)
{
	cout<<"\nadd_cellnum_to_bucket\n";
	node *temp =new node;
	node *newnode = new node;
	newnode->cell_number = cell_number;
	newnode->next = NULL;
	newnode->prev = bucket[bucket_number].second;
	newnode->prev->next = newnode;
	bucket[bucket_number].second = newnode;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Splitting_Buckets END~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Splitting_Buckets(int bucket_number, int median, char widest_spread, vector < pair<int, pair<int, int>> > &temp_vector)
{
	string file_name;
	file_name = "bucket" + to_string(bucket_number) + ".txt";
	fstream ofile1;
	ofile1.open(file_name, ios::out);
	
	file_name = "bucket" + to_string(bucket.size()-1) + ".txt";
	fstream ofile2;
	ofile2.open(file_name, ios::out);
	
	if(widest_spread == 'x')
	{
		for(int i=0; i < temp_vector.size(); i++)
		{
			if(temp_vector[i].second.first <= median)
				ofile1<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;
			else
				ofile2<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;	
		}
	}
	else
	{
		for(int i=0; i < temp_vector.size(); i++)
		{
			if(temp_vector[i].second.second <= median)
				ofile1<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;
			else
				ofile2<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;	
		}
	}
	
	ofile1.close();
	ofile2.close();
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Logical_Split~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Logical_Split(int partition_axis, int cell_number, int bucket_number, vector < pair<int, pair<int, int>> > &temp_vector, vector <int> temp_cell)
{
	cout<<"\nLogical Split\n";
	
	string file_name;
	file_name = "bucket" + to_string(bucket_number) + ".txt";
	fstream ofile1;
	ofile1.open(file_name, ios::out);
	
	file_name = "bucket" + to_string(bucket.size()) + ".txt";
	fstream ofile2;
	ofile2.open(file_name, ios::out);
	struct node *ptr = NULL;
	//ptr = NULL;
	bucket.push_back(make_pair(ptr, ptr));
	
	int partition_val;
	
	//update buckets
	if(partition_axis == 1)
		
	{
		partition_val = cell[cell_number].second.first;
		for(int i=0; i < temp_vector.size(); i++)
		{
			if(temp_vector[i].second.first <= cell[cell_number].second.first)
			{
				ofile1<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;
			}
			else
			{
				ofile2<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;
			}
		}
	}
	if(partition_axis == 2)
	{
		partition_val = cell[cell_number].second.second.second.first;
		for(int i=0; i < temp_vector.size(); i++)
		{
			if(temp_vector[i].second.first <= cell[cell_number].second.second.second.first)
			{
				ofile1<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;
			}
			else
			{
				ofile2<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;
			}
		}
	}
	if(partition_axis == 3)
	{
		partition_val = cell[cell_number].second.second.first;
		for(int i=0; i < temp_vector.size(); i++)
		{
			if(temp_vector[i].second.second <= cell[cell_number].second.second.first)
			{
				ofile1<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;
			}
			else
			{
				ofile2<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;
			}
		}
	}
	if(partition_axis == 4)
	{
		partition_val = cell[cell_number].second.second.second.second;
		for(int i=0; i < temp_vector.size(); i++)
		{
			if(temp_vector[i].second.second <= cell[cell_number].second.second.second.second)
			{
				ofile1<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;
			}
			else
			{
				ofile2<<temp_vector[i].first<<","<<temp_vector[i].second.first<<","<<temp_vector[i].second.second<<endl;
			}
		}
	}
	
	//update bucket vector
	
	Splitting_Scales(partition_axis, partition_val, bucket_number);	//to split buckets by reallocating cells
	
	ofile1.close();
	ofile2.close();
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Logical_Split END~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Splitting_Scales STARTS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Splitting_Scales(int partition_axis, int partition_val, int bucket_number)
{
	node *temp = new node;
	temp = bucket[bucket_number].first;
	while(temp != NULL)
	{
		cout<<cell[temp->cell_number].second.second.second.first<<">"<<partition_val<<"or"<<cell[temp->cell_number].second.second.second.second<<">"<<partition_val<<endl;
		node *remnode = new node;
		if( ((partition_axis == 1 || partition_axis == 2) && cell[temp->cell_number].second.second.second.first > partition_val) || ((partition_axis == 3 || partition_axis == 4) && cell[temp->cell_number].second.second.second.second > partition_val))
		{
			cout<<"\ncell num: "<<temp->cell_number;
			node *temp1 = new node;
			cout<<"\ntemp1= "<<bucket[bucket.size()-1].second;
			//print_bucket_vector();
			//temp1 = bucket[bucket.size()-1].second;
			//cout<<"\ntemp1= "<<temp1;
			//cout<<"\nptr= "<<ptr;
			//cout<<"\ntemp1->cell_number: "<<temp1->cell_number;
			
			if(bucket[bucket.size()-1].second == NULL)		// insert first cell
			{
				cout<<"\nfirst cell";
				bucket[bucket.size()-1].first = temp1;
				bucket[bucket.size()-1].second = temp1;
				temp1->cell_number = temp->cell_number;
				temp1->prev = NULL;
				temp1->next = NULL;
			}
			else
			{
				temp1 = bucket[bucket.size()-1].second;
				node *newnode = new node;
				newnode->cell_number = temp->cell_number;
				temp1->next = newnode;
				newnode->prev = temp1;
				newnode->next = NULL;
				bucket[bucket.size()-1].second = newnode;
			}
			cout<<"\nbefore rem\n";
			print_bucket_vector();
			cout<<"\ntemp->prev: "<<temp->prev;
			if(temp->prev == NULL)
			{
				cout<<"\ntemp->cell_num"<<temp->cell_number;
				temp->next->prev = NULL;
				bucket[bucket_number].first = temp->next;
			}
			else 
			{
				if(temp->next == NULL)
				{
					temp->prev->next = NULL;
					bucket[bucket_number].second = temp->prev;
				}
			
				else
				{
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;
				}
			}
			cout<<"\nafter rem\n";
			print_bucket_vector();	
			remnode = temp;
			
		}
		temp = temp->next;
		free(remnode);
	}
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Splitting_Scales END~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*----------------------------------------------------------------------FINDING MEDIAN--------------------------------------------------------------------------*/
int find_median(vector < pair<int, pair<int, int>> > &temp_vector)
{
	cout<<"\nFind median\n";
	int median;
	int count = 0;
	char widest_spread;
	
	// case1: find median using basic formula
	widest_spread = find_widest_spread(temp_vector);
	cout<<"\nwidest_spread: "<<widest_spread;
	cout<<"\ntemp_vector.size(): "<<temp_vector.size()<<endl;
	
	if(widest_spread == 'x')
	{
		sort(temp_vector.begin(), temp_vector.end(), sort_X);
		if(temp_vector.size()%2 == 0)
			median= int ((temp_vector[(temp_vector.size()/2)-1].second.first + temp_vector[(temp_vector.size()/2)].second.first)/2);
		else
			median= int (temp_vector[(temp_vector.size()/2)].second.first);	
		for(int i= 0; i < temp_vector.size(); i++)
		{
			if(temp_vector[i].second.first <= median)		// points on left partition
				count++;					
		}
	}	
	else
	{
		sort(temp_vector.begin(), temp_vector.end(), sort_Y);
		if(temp_vector.size()%2 == 0)
			median= int ((temp_vector[(temp_vector.size()/2)-1].second.second + temp_vector[(temp_vector.size()/2)].second.second)/2);
		else
			median= int (temp_vector[(temp_vector.size()/2)].second.second);
		for(int i= 0; i < temp_vector.size(); i++)
		{
			if(temp_vector[i].second.second <= median)		// points on left partition
				count++;						
		}
	}
	cout<<"count"<<count;	
	cout<<"\nmedian using basic formula: "<<median<<endl;
	cout<<count<<" <= "<<bucket_capacity<<" && "<<count<<" != 0";
	if( count <= bucket_capacity && count != 0)
		return median;
		
	cout<<"next  median";
	// find median using average value
	int sum=0;
	int distinct=1;
	count = 0;
	int prev = temp_vector[0].second.first;
	sum = prev;
	if(widest_spread == 'x')
	{	
		for(int i=0; i<temp_vector.size(); i++)
		{
			if(temp_vector[i].second.first != prev)
			{
				sum += temp_vector[i].second.first;
				prev= temp_vector[i].second.first;
				distinct++;
			}	
		}
		median= int (sum/distinct);
		for(int i=0; i<temp_vector.size(); i++)
		{
			if(temp_vector[i].second.first <= median)
				count++;
		}
	} 
	else
	{
		int prev = temp_vector[0].second.second;	
		for(int i=0; i<temp_vector.size(); i++)
		{
			if(temp_vector[i].second.second != prev)
			{
				sum += temp_vector[i].second.second;
				prev = temp_vector[i].second.second;
				distinct++;
			}	
		}
		median= int (sum/distinct);
		for(int i=0; i<temp_vector.size(); i++)
		{
			if(temp_vector[i].second.second <= median)
				count++;
		}
	}
	cout<<"\nmedian using average value: "<<median<<endl;
	if(count <= bucket_capacity && count != 0)
		return median;
	return -1;
}
/*-------------------------------------------------------------FIND MEDIAN ENDS---------------------------------------------------------------------------------*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~find_widest_spread~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
char find_widest_spread(vector < pair<int, pair<int, int>> > &temp_vector)
{
	cout<<"\nfind_widest_spread\n";
	int xmin, xmax, ymin, ymax;
	sort(temp_vector.begin(), temp_vector.end(), sort_Y);
	ymin = temp_vector[0].second.second;
	ymax = temp_vector[temp_vector.size()-1].second.second;
	sort(temp_vector.begin(), temp_vector.end(), sort_X);
	xmin = temp_vector[0].second.first;
	xmax = temp_vector[temp_vector.size()-1].second.first;
	cout<<"xmin,xmax,ymin,ymax: "<<xmin<<","<<xmax<<","<<ymin<<","<<ymax<<",";
	if(xmax-xmin >= ymax-ymin)
		return 'x';
	else
		return 'y';
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~find_widest_spread ends~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~traditional_approach~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void traditional_approach(int x1, int y1, int x2, int y2)
{
	cout<<"\ntraditional approach\n";
	fstream ifile;
	ifile.open("input.txt", ios::in);
	string line;
	int id, x, y;
	while(!ifile.eof())
	{
		getline(ifile, line);
		stringstream ss(line);
		while(getline(ss,line,','))
		{
			id=stoi(line);
			getline(ss,line,',');
			x=stoi(line);
			getline(ss,line,',');
			y=stoi(line);
			if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
				cout<<id<<","<<x<<","<<y<<endl;				
		}
	}
	ifile.close();
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~traditional_approach ends~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~our_approach~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void our_approach(int x1, int y1, int x2, int y2)
{
	cout<<"\nour approach\n";
	vector <int> temp_vector;
	vector <int> bkt_vector;
	for(int i=0; i<cell.size(); i++)
	{
		if (cell[i].second.first<x2  && cell[i].second.second.first<y2 && cell[i].second.second.second.first>x1 && cell[i].second.second.second.second>y1)
		{
			temp_vector.push_back(i);
		}
	}
	cout<<"\ntemp_vector"<<endl;
	for(int i=0; i < temp_vector.size(); i++)
	{
		cout<<temp_vector[i]<<",";
	}
	int bkt_num;
	for(int i=0; i < temp_vector.size(); i++)
	{
		bkt_num = extract_bucket_index(temp_vector[i]);
		cout<<"bkt_num"<<bkt_num;
		int j=0;
		for(j=0; j < bkt_vector.size(); j++)
		{
			if(bkt_num == bkt_vector[j])
				break;
		}
		if(j == bkt_vector.size())
		{
			bkt_vector.push_back(bkt_num);
		}	
	}
	cout<<"\nbkt_vector\n";
	for(int i=0; i < bkt_vector.size(); i++)
	{
		cout<<bkt_vector[i]<<",";
	}
	cout<<endl;
	bool is_present = false;
	for(int i=0; i < bkt_vector.size(); i++)
	{
		//cout<<"\ni: "<<bkt_vector[i]<<endl;
		string file_name;
		file_name = "bucket" + to_string(bkt_vector[i]) + ".txt";
		fstream ifile;
		ifile.open(file_name, ios::in);
		string line;
		int id, x, y;
		
		while(!ifile.eof())
		{
			getline(ifile, line);
			size_t found;
			//cout<<"\nline: "<<line<<endl;
			found = line.find(".");
			
			if (found != string::npos)  //string found
			{
				//cout<<"\nline inside found: "<<line<<endl;
				file_name = line;
				fstream link_file;
				link_file.open(file_name, ios::in);
				//cout<<"\nfilename: "<<file_name<<endl;
				while(!link_file.eof())
				{
					getline(link_file, line);
					//cout<<"\n getline: "<<line<<endl;
					stringstream ss(line);
					while(getline(ss,line,','))
					{
						id=stoi(line);
						getline(ss,line,',');
						x=stoi(line);
						getline(ss,line,',');
						y=stoi(line);
						if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
							cout<<id<<","<<x<<","<<y<<endl;				
					}
				}
				link_file.close();
				break;
			}
			stringstream ss(line);
			
			while(getline(ss,line,','))
			{
				id=stoi(line);
				getline(ss,line,',');
				x=stoi(line);
				getline(ss,line,',');
				y=stoi(line);
				if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
					cout<<id<<","<<x<<","<<y<<endl;				
			}
		}
		ifile.close();
	}
	
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~our_approach ends~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void link_file(int bucket_number, int data_index, vector < pair<int, pair<int, int>> > &temp_vector)
{
	cout<<"\nlink_file"<<endl;
	string line;
	string file_name;
	file_name = "bucket" + to_string(bucket_number) + ".txt";
	cout<<"file: "<<file_name;
	bool is_present = false;
	size_t found;
	fstream linkfile;
	fstream ifile;
	ifile.open(file_name, ios::in);
	while(!ifile.eof())
	{
		getline(ifile, line);
		found = line.find(".");
		if (found != string::npos)  //string found
		{
			is_present = true;
		}
	}
	ifile.close();
	if (is_present == true)  //link file already present
	{
		file_name = line;
		cout<<"filename "<<file_name;
		linkfile.open(file_name, ios::app);
	}
	else		// link file not present
	{
		cout<<"if";
		ifile.close();
		ofstream ofile;
		ofile.open(file_name, ios::app);
		ofile.seekp(bucket_capacity*sizeof(temp_vector[0]));
		file_name = "link" + to_string(link_file_num) + ".txt";
		ofile<<file_name;
		ofile.close();
		link_file_num++;
		linkfile.open(file_name, ios::out);
	}
	
	linkfile<<data[data_index].first<<","<<data[data_index].second.first<<","<<data[data_index].second.second<<endl;
	
	linkfile.close();
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/











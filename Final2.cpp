#include <iostream>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <ncurses.h>
using namespace std; 

class MineWindow
{
private:
	int x;																					//size of board
	vector<vector <int>> board;																//stores the STATE of each cell of board
	vector<vector <int>> open;
	vector<vector <int>> flag;	
	int numberofmines;	
	int numberofflags=0;
	int numberofopen=0;			
	int hint=3;										
public:
	void Entry()																			//input of Size of board
	{
		system("clear");
		cout<<"Enter the Size of Board: ";
		cin>>x;
		if(x<2)
		{
			cout<<endl;
			cout<<"⚠️  Enter an Integer Greater than 1\n";
			cout.flush();
			usleep(1000*1000);
			Entry();
		}
	}
	void Initialize()																		//populating 2-d vector with intial values
	{
		for(int i=0;i<x;i++)
		{
			vector<int> v;
			for(int j=0;j<x;j++)
			{
				v.push_back(0);
			}
			board.push_back(v);
			open.push_back(v);
			flag.push_back(v);
		}
		Create();																				
		Bomb();
	}
	void Bomb()																				//populating 2-d vector with bombs
	{
		numberofmines=0;
		int i=((x*x)/4);
		while(i)
		{
			int j=(rand() % (x*x));
			if(board[j%x][j/x]!=(-1))
			{
				board[j%x][j/x]=(-1); 
			}
			else
			{
				continue;
			}
			numberofmines++;
			i--;
		}
		for(int k=0;k<board.size();k++)
		{
			for(int l=0;l<board[k].size();l++)
			{
				if(board[k][l]!=(-1))
				{
					int count=0;
					if(l>0)
					{
						if(k>0)
						{
							if(board[k-1][l-1]==(-1))
							{
								count++;
							}
						}
						if(k<(x-1))
						{
							if(board[k+1][l-1]==(-1))
							{
								count++;
							}
						}
						if(board[k][l-1]==(-1))
						{
							count++;
						}
					}
					if(l<(x-1))
					{
						if(k>0)
						{
							if(board[k-1][l+1]==(-1))
							{
								count++;
							}
						}
						if(k<(x-1))
						{
							if(board[k+1][l+1]==(-1))
							{
								count++;
							}
						}
						if(board[k][l+1]==(-1))
						{
							count++;
						}
					}
					if(k>0)
					{
						if(board[k-1][l]==(-1))
						{
							count++;
						}
					}
					if(k<(x-1))
					{
						if(board[k+1][l]==(-1))
						{
							count++;
						}
					}
					board[k][l]=count;
				}
			}
		}
		//to be erased from here
		/*cout<<endl;
		for(int k=0;k<board.size();k++)
		{
			for(int l=0;l<board[k].size();l++)
			{
				cout<<board[k][l]<<" ";
			}
			cout<<endl;
		}*/
		//till here
	}
	void Open()
	{
		if(hint>0)
		{
			hint--;
			int j=(rand() % (x*x));
				while(board[j%x][j/x]==(-1) || open[j%x][j/x]==1)
				{
					j=(rand() % (x*x));
				}
			open[j%x][j/x]=1;
			numberofopen++;
			Create();
		}
		else
		{
			cout<<"You are out of Hints 😞";
			cout.flush();
			usleep(1000*2500);
			system("clear");
			Create();
		}
	}
	void Create()																			//prints the board in terminal 
	{
		system("clear");
		for(int i=0;i<x;i++)
		{
			cout<<"  "<<(i+1);
		}
		cout<<"\n";
		for(int i=0;i<x;i++)
		{
			cout<<(i+1);
			for(int j=0;j<x;j++)
			{
					if(board[i][j]==(-2))
					{
						cout<<"|💣";
					}
					else if(open[i][j]==1)
					{
						cout<<"|"<<board[i][j]<<" ";
					}
					else if(flag[i][j]==1)
					{
						cout<<"|🚩";
					}
					else
					{
						cout<<"|  ";
					}
			}
			cout<<"|"<<endl;
		}
		cout<<"Total Number of Mines="<<((x*x)/4)<<"   "<<"Number of Flags used="<<numberofflags<<"   "<<"Number of Hints left="<<hint<<endl;
		
	}
	int update1(int row,int col)
	{
		if(row<0 || row>=x || col<0 || col>=x)
		{
			cout<<"The Entered Co-Ordinates are out of Range 😇"<<endl;
			cout<<"Please Try Again"<<endl;
			cout.flush();
        	usleep(1000*2500);
			system("clear");
			Create();
			return 0;
		}
		if(open[row][col]==1)
		{
			cout<<"The Entered Cell has already been Opened 😇";
			cout<<endl;
			cout<<"Enter a different Cell";
			cout.flush();
        	usleep(1000*2500);
			system("clear");
			Create();
			return 0;
		}
		if(flag[row][col]==1)
		{
			cout<<"The Entered Cell has already been Flagged 😇";
			cout<<endl;
			cout<<"Enter a different Cell";
			cout.flush();
        	usleep(1000*2500);
			system("clear");
			Create();
			return 0;
		}
		if(board[row][col]==(-1))
		{
			for(int i=0;i<x;i++)
			{
				for(int j=0;j<x;j++)
				{

					if(board[i][j]==(-1))
					{
						board[i][j]=(-2);
					}
				}
			}	
			Create();
			return 1;
		}
		open[row][col]=1;
		numberofopen++;
		Create();
		if(numberofopen==((x*x)-numberofmines))
		{
			return 2;
		}
		if(numberofopen+numberofflags==(x*x))
		{
			return 1;
		}
		return 0;
	}
	int update2(int row,int col)
	{
		if(row<0 || row>=x || col<0 || col>=x)
		{
			cout<<"The Entered Co-Ordinates are out of Range 😇"<<endl;
			cout<<"Please Try Again"<<endl;
			cout.flush();
        	usleep(1000*2500);
			system("clear");
			Create();
			return 0;
		}
		if(open[row][col]==1)
		{
			cout<<"The Entered Cell has already been Opened 😇";
			cout<<endl;
			cout<<"Enter a different Cell";
			cout.flush();
        	usleep(1000*2500);
			system("clear");
			Create();
			return 0;
		}
		if(flag[row][col]==1)
		{
			cout<<"The Entered Cell has already been Flagged 😇";
			cout<<endl;
			cout<<"Enter a different Cell";
			cout.flush();
        	usleep(1000*2500);
			system("clear");
			Create();
			return 0;
		}
		flag[row][col]=1;
		numberofflags++;
		Create();
		if(numberofflags==numberofmines)
		{
			return 2;
		}
		if(numberofopen+numberofflags==(x*x))
		{
			return 1;
		}
		if(numberofflags==(x*x))
		{
			return 1;
		}
		return 0;
	}
	int check1()
	{
		int count=0;
		for(int i=0;i<x;i++)
		{
			for(int j=0;j<x;j++)
			{
				if(board[i][j]!=(-1) && open[i][j]==1)
				{
					count++;
				}
			}
		}
		if(count==numberofopen)
		{
			for(int i=0;i<x;i++)
			{
				for(int j=0;j<x;j++)
				{
					if(board[i][j]==(-1))
					{
						flag[i][j]=1;
						numberofflags++;
					}
				}
			}
			system("clear");
			Create();
			return 2;
		}
		return 0;
	}
	int check2()
	{
		int count=0;
		for(int i=0;i<x;i++)
		{
			for(int j=0;j<x;j++)
			{
				if(board[i][j]==(-1) && flag[i][j]==1)
				{
					count++;
				}
			}
		}
		if(count==numberofmines)
		{
			return 2;
		}
		return 0;
	}
	int UnFlag(int row,int col)
	{
		if(open[row][col]==1)
		{
			cout<<"The Entered Cell is already Opened 😒";
			cout.flush();
			usleep(1000*1500);
			system("clear");
			Create();
			return 0;
		}
		if(flag[row][col]!=1)
		{
			cout<<"The Entered Cell isn't Flagged 😒";
			cout.flush();
			usleep(1000*1500);
			system("clear");
			Create();
			return 0;
		}
		if(flag[row][col]==1)
		{
			flag[row][col]=0;
			numberofflags--;
			system("clear");
			Create();
			return 0;
		}
		return 0;
	}
};
int main()
{
	while(1)
	{
		srand(time(0));
		system("clear");         
		cout<<"W";
		cout.flush();
		usleep(1000*50);
		cout<<"E";
		cout.flush();
		usleep(1000*50);
		cout<<"L";
		cout.flush();
		usleep(1000*50);
		cout<<"C";
		cout.flush();
		usleep(1000*50);
		cout<<"O";
		cout.flush();
		usleep(1000*50);
		cout<<"M";
		cout.flush();
		usleep(1000*50);
		cout<<"E";
		cout.flush();
		usleep(1000*50);
		cout<<" ";
		cout.flush();
		usleep(1000*50);
		cout<<"T";
		cout.flush();
		usleep(1000*50);
		cout<<"O";
		cout.flush();
		usleep(1000*50);
		cout<<" ";
		cout.flush();
		usleep(1000*50);
		cout<<"M";
		cout.flush();
		usleep(1000*50);
		cout<<"I";
		cout.flush();
		usleep(1000*50);
		cout<<"N";
		cout.flush();
		usleep(1000*50);
		cout<<"E";
		cout.flush();
		usleep(1000*50);
		cout<<"S";
		cout.flush();
		usleep(1000*50);
		cout<<"W";
		cout.flush();
		usleep(1000*50);
		cout<<"E";
		cout.flush();
		usleep(1000*50);
		cout<<"E";
		cout.flush();
		usleep(1000*50);
		cout<<"P";
		cout.flush();
		usleep(1000*50);
		cout<<"E";
		cout.flush();
		usleep(1000*50);
		cout<<"R";
		cout.flush();
		usleep(1000*50);
		cout<<"💣";
		cout.flush();
		usleep(1000*50);
		cout<<"💣";
		cout.flush();
		usleep(1000*50);
		cout<<"💣";
		cout.flush();
		usleep(1000*500);
		cout<<endl;
		cout<<"Press 1 to Play";
		cout<<endl;
		cout.flush();
		usleep(1000*500);
		cout<<"Press 2 to EXIT";
		cout<<endl;
		cout.flush();
		usleep(1000*500);
		cout<<"Enter :";
		int i;
		cin>>i;
		while(i!=1 && i!=2)
		{
			cout<<"Please select 1 (or) 2 😇";
			cout<<endl;
			cout<<"Enter :";
			cin>>i;
		}
		if(i==2)
		{
			system("exit");
			system("clear");
			return 0;
		}
			MineWindow w;
			w.Entry();
			w.Initialize();
			cout<<"A Cell opens in...";
			cout.flush();
			usleep(1000*1000);
			cout<<"1..";
			cout.flush();
			usleep(1000*1000);
			cout<<"2..";
			cout.flush();
			usleep(1000*1000);
			cout<<"3..";
			cout.flush();
			usleep(1000*500);
			w.Open();
			int flag=0;
			while(flag==0)
			{
				cout<<"Press 1 to Open a Cell";
				cout<<endl;
				cout<<"Press 2 to Flag a Cell";
				cout<<endl;
				cout<<"Press 3 to use Hint";
				cout<<endl;
				cout<<"Press 4 to UnFlag";
				cout<<endl;
				cout<<"Enter :";
				int ii;
				cin>>ii;
				if(ii!=1 && ii!=2 && ii!=3 && ii!=4)
				{
					cout<<"Please select 1 (or) 2 (or) 3 (or) 4 😇";
					cout<<endl;
					cout<<"Try Again"<<endl;
					cout.flush();
					usleep(1000*1000);
					cout<<"wait..";
					cout.flush();
					usleep(1000*1000);
					cout<<"1..";
					cout.flush();
					usleep(1000*1000);
					cout<<"2..";
					cout.flush();
					usleep(1000*1000);
					system("clear");
					w.Create();
					continue;
				}
				if(ii==1)
				{
					cout<<"Enter the row number of the Cell to be Opened :";
					int row;
					cin>>row;
					row=row-1;
					//cout<<endl;
					cout<<"Enter the column number of the Cell to be Opened :";
					int col;
					cin>>col;
					col=col-1;
					flag=w.update1(row,col);
					if(flag==1)
					{
						cout<<"G";
						cout.flush();
	        			usleep(1000*400);
						cout<<"A";
						cout.flush();
	        			usleep(1000*400);
						cout<<"M";
						cout.flush();
	        			usleep(1000*400);
						cout<<"E";
						cout.flush();
	        			usleep(1000*400);
	        			cout<<" ";
						cout.flush();
	        			usleep(1000*400);
						cout<<"O";
						cout.flush();
	        			usleep(1000*400);
						cout<<"V";
						cout.flush();
	        			usleep(1000*400);
						cout<<"E";
						cout.flush();
	        			usleep(1000*400);
						cout<<"R";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*400);
						cout<<" ☹️";
						cout.flush();
						cout<<endl;
						cout<<"T";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"r";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"y ";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"A";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"g";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"a";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"i";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"n";
						cout.flush();
	        			usleep(1000*500);
						cout<<endl;
					}
					if(flag==2)
					{
						flag=w.check1();
					}
					if(flag==2)
					{
						cout<<"W";
						cout.flush();
	        			usleep(1000*200);
						cout<<"U";
						cout.flush();
	        			usleep(1000*200);
						cout<<"H";
						cout.flush();
	        			usleep(1000*200);
						cout<<"O";
						cout.flush();
	        			usleep(1000*200);
						cout.flush();
	        			usleep(1000*200);
						cout<<"O";
						cout.flush();
	        			usleep(1000*200);
						cout<<"O";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"🎉 ";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"🎉 ";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<endl;
	        			cout<<"Y";
						cout.flush();
	        			usleep(1000*300);
						cout<<"O";
						cout.flush();
	        			usleep(1000*300);
						cout<<"U";
						cout.flush();
	        			usleep(1000*300);
						cout<<" ";
						cout.flush();
	        			usleep(1000*300);
						cout.flush();
	        			usleep(1000*300);
						cout<<"W";
						cout.flush();
	        			usleep(1000*300);
						cout<<"O";
						cout.flush();
	        			usleep(1000*300);
	        			cout<<"N";
						cout.flush();
	        			usleep(1000*300);
						cout<<".";
						cout.flush();
	        			usleep(1000*300);
						cout<<".";
						cout.flush();
	        			usleep(1000*400);
	        			cout<<"🥳";
	        			cout.flush();
	        			usleep(1000*2500);
					}
				}
				if(ii==2)
				{
					cout<<"Enter the row number of the Cell to be Flagged :";
					int row;
					cin>>row;
					row=row-1;
					//cout<<endl;
					cout<<"Enter the column number of the Cell to be Flagged :";
					int col;
					cin>>col;
					col=col-1;
					flag=w.update2(row,col);
					if(flag==2)
					{
						flag=w.check2();
					}
					if(flag==1)
					{
						cout<<"G";
						cout.flush();
	        			usleep(1000*400);
						cout<<"A";
						cout.flush();
	        			usleep(1000*400);
						cout<<"M";
						cout.flush();
	        			usleep(1000*400);
						cout<<"E";
						cout.flush();
	        			usleep(1000*400);
	        			cout<<" ";
						cout.flush();
	        			usleep(1000*400);
						cout<<"O";
						cout.flush();
	        			usleep(1000*400);
						cout<<"V";
						cout.flush();
	        			usleep(1000*400);
						cout<<"E";
						cout.flush();
	        			usleep(1000*400);
						cout<<"R";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*400);
						cout<<" ☹️";
						cout.flush();
						cout<<endl;
						cout<<"T";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"r";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"y ";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"A";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"g";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"a";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"i";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"n";
						cout.flush();
	        			usleep(1000*500);
						cout<<endl;
					}
					if(flag==2)
					{
						cout<<"W";
						cout.flush();
	        			usleep(1000*200);
						cout<<"U";
						cout.flush();
	        			usleep(1000*200);
						cout<<"H";
						cout.flush();
	        			usleep(1000*200);
						cout<<"O";
						cout.flush();
	        			usleep(1000*200);
						cout.flush();
	        			usleep(1000*200);
						cout<<"O";
						cout.flush();
	        			usleep(1000*200);
						cout<<"O";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*200);
						cout<<".";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"🎉 ";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<"🎉 ";
						cout.flush();
	        			usleep(1000*200);
	        			cout<<endl;
	        			cout<<"Y";
						cout.flush();
	        			usleep(1000*300);
						cout<<"O";
						cout.flush();
	        			usleep(1000*300);
						cout<<"U";
						cout.flush();
	        			usleep(1000*300);
						cout<<" ";
						cout.flush();
	        			usleep(1000*300);
						cout.flush();
	        			usleep(1000*300);
						cout<<"W";
						cout.flush();
	        			usleep(1000*300);
						cout<<"O";
						cout.flush();
	        			usleep(1000*300);
	        			cout<<"N";
						cout.flush();
	        			usleep(1000*300);
						cout<<".";
						cout.flush();
	        			usleep(1000*300);
						cout<<".";
						cout.flush();
	        			usleep(1000*400);
	        			cout<<"🥳";
	        			cout.flush();
	        			usleep(1000*2500);
					}
				}
				if(ii==3)
				{
					w.Open();
				}
				if(ii==4)
				{
					cout<<"Enter the row number of the Cell to be UnFlagged :";
					int row;
					cin>>row;
					row=row-1;
					//cout<<endl;
					cout<<"Enter the column number of the Cell to be UnFlagged :";
					int col;
					cin>>col;
					col=col-1;
					flag=w.UnFlag(row,col);
				}
			}
	}
	return 0;
}
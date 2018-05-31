//Two players,weiqi and wuziqi.
#include <iostream>
#include <iomanip>
#define ROAD 19     //ROAD>=3.
#define HANDS 400
using namespace std;

void printBoard(void);
int endOfGame(int x,int y,string a[ROAD][ROAD]);
string board[ROAD][ROAD];

int main()
{
    int i,j,x,y,hand;
    const string grid[][3]=
        {"©°","©Ð","©´","©À","©à","©È","©¸","©Ø","©¼","¡ð","¡ñ"};
    const string turn[]={"White","Black"};
    char review[HANDS][2];

    //Board initializing
    for(i=0;i<ROAD;i++)
        for(j=0;j<ROAD;j++)
            board[i][j]=grid[(i+ROAD-3)/(ROAD-2)][(j+ROAD-3)/(ROAD-2)];
            //0->0,1~17->1,18->2.
     board[3][3]		=board[3][ROAD/2]		=board[3][ROAD-4]
    =board[ROAD/2][3]	=board[ROAD/2][ROAD/2]	=board[ROAD/2][ROAD-4]
    =board[ROAD-4][3]	=board[ROAD-4][ROAD/2]	=board[ROAD-4][ROAD-4]="©I";
    printBoard();

    //Luo4zi3
    for(hand=1;hand<=HANDS;hand++)
    {
        cout<<"Hand "<<hand<<':'<<
            turn[hand%2]<<"'s turn.Please enter the coordinates.\n";
        do cin>>x>>y;
        while(x<1||x>ROAD||y<1||y>ROAD
           ||grid[3][0]==board[x-1][y-1]
           ||grid[3][1]==board[x-1][y-1]);
        board[x-1][y-1]=grid[3][hand%2];
        review[hand-1][0]=x+64;         //A~S stand for 1~19.
        review[hand-1][1]=y+64;
        printBoard();
        if(endOfGame(x-1,y-1,board)>=5)
        {
			cout<<"5 Pieces in a line,"<<turn[hand%2]<<" wins!";
        	break;
        }
    }
	return 0;
}

void printBoard(void)
{
    int i,j;
    cout<<"  ";
    for(i=1;i<=ROAD;i++)cout<<setw(2)<<i;
    cout<<'\n';
    for(i=0;i<ROAD;i++)
    {
        cout<<setw(2)<<i+1;
        for(j=0;j<ROAD;j++)
        {
            cout<<board[i][j];
            if(j%ROAD==ROAD-1)cout<<'\n';
        }
    }
}

int endOfGame(int x,int y,string a[ROAD][ROAD])
{
    int direction[4],i,sign=1,lian=4;//direction including shu,heng,pie and na.
    for(i=0;i<4;i++)direction[i]=1;
//shu
    for(i=1;x+i<ROAD;i++)
        if(a[x+i][y]==a[x][y])direction[0]++;else break;
    for(i=1;x-i>=0;i++)
        if(a[x-i][y]==a[x][y])direction[0]++;else break;
//heng
    for(i=1;y+i<ROAD;i++)
        if(a[x][y+i]==a[x][y])direction[1]++;else break;
    for(i=1;y-i>=0;i++)
        if(a[x][y-i]==a[x][y])direction[1]++;else break;
//pie
    for(i=1;x+i<ROAD && y-i>=0;i++)
        if(a[x+i][y-i]==a[x][y])direction[2]++;else break;
    for(i=1;x-i>=0 && y+i<ROAD;i++)
        if(a[x-i][y+i]==a[x][y])direction[2]++;else break;
//na
    for(i=1;x+i<ROAD && y-i>=0;i++)
        if(a[x+i][y+i]==a[x][y])direction[3]++;else break;
    for(i=1;x-i>=0 && y+i<ROAD;i++)
        if(a[x-i][y-i]==a[x][y])direction[3]++;else break;
    for(i=0;i<4;i++)
        if(direction[i]>lian)lian=direction[i];
    return lian;
}

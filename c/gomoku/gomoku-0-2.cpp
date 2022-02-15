/*
    Name:       WuZiQi_2.0
    Author:     zmx0142857
    Version:    2.0
    Copyright:
    Description:A program for two players playing WuZiQi.
*/
#include <iostream>
#include <iomanip>

#define ROAD    19      // Recommended: 9<=ROAD<=32
#define HANDS   400
#define WHITE   grid[3][0]
#define BLACK   grid[3][1]
#define W_OR_B  (0==hand%2?"White":"Black")

using namespace std;

void printBoard(void);
void resetBoard(void);
int line(char x,char y);

static const string grid[4][3]={"©°","©Ð","©´","©À","©à","©È","©¸","©Ø","©¼","¡ð","¡ñ"};
static string board[ROAD][ROAD];
static char review[HANDS][2];
static int hand=1;

int main()
{
    string input,rec;
    char x,y;
newgame:////////////////////////////////////////////////////////////////////////
    resetBoard();
    for(hand=1; hand<HANDS; hand++)
    {
handout:////////////////////////////////////////////////////////////////////////
        cout<<"\nHand "<<hand<<": "<<W_OR_B
            <<"'s turn. Please enter the coordinates.\n"
            <<"        Type 'help' for a detailed instruction.\n";
inputting://////////////////////////////////////////////////////////////////////
        cout<<">>> ";
        cin>>input;
        if(input=="help")
        {
            cout<<"\nTo enter the coordinates, try typing 'jj' or 'JJ';\n"
                <<"Here's some tips to follow:\n"
                <<"    'review' - See history \n"
                <<"    'undo' - Undo, you know...\n"
                <<"    'resign' - Are you sure?\n"
                <<"    'draw' - Talk with your opponent before typing this!\n"
                <<endl;
                goto inputting;
        }
        else if(input=="review")
        {
            cout<<endl;
            for(int i=1; i<hand; i++)
                cout<<grid[3][i%2]<<setw(4)<<i<<". "
                        <<review[i][0]<<','<<review[i][1]<<endl;
            cout<<endl;
            goto inputting;
        }
        else if(input=="undo" && hand>1)
        {
            hand--;
            board[x][y]=rec;
            printBoard();
            goto handout;
        }
        else if(input=="resign")
        {
            cout<<'\n'<<W_OR_B<<" resigned!";
            goto ending;
        }
        else if(input=="draw")
        {
            cout<<"\nDraw!";
            goto ending;
        }
        else
        {
            x=(input[0]-65)%32;
            y=(input[1]-65)%32;
            if(x<0||x>=ROAD || y<0||y>=ROAD || WHITE==board[x][y] || BLACK==board[x][y])
            // out of board                 // position already taken
            {
                cout<<"\nUnavailable command. Please try another.\n";
                goto inputting;
            }
            rec=board[x][y];
            board[x][y]=grid[3][hand%2];
            review[hand][0]=input[0];
            review[hand][1]=input[1];
        }
        cout<<endl;
        printBoard();

        if(line(x,y)>=5)
        {
			cout<<"\nFinally, 5 Pieces in a line, "<<W_OR_B<<" wins!";
ending://///////////////////////////////////////////////////////////////////////
            cout<<"\nPlay again[Y/N]?\n>>> ";
			cin>>input;
			if(input=="Y" || input=="y")
            {
                cout<<endl;
                goto newgame;
            }
            else if(input=="review")
            {
                cout<<endl;
                for(int i=1; i<hand; i++)
                    cout<<grid[3][i%2]<<setw(4)<<i<<". "
                        <<review[i][0]<<','<<review[i][1]<<endl;
                cout<<endl;
                goto ending;
            }
            break;
        }
    }
	return 0;
}

void printBoard(void)
{
    char i,j;

    cout<<"  ";
    for(i=0; i<ROAD; i++)
        cout<<' '<<char(i+65);
    cout<<endl;

    for(i=0; i<ROAD; i++)
    {
        cout<<' '<<char(i+65);
        for(j=0; j<ROAD; j++)
            cout<<board[i][j];
        cout<<endl;
    }
}

void resetBoard(void)
{
    int i,j;

    //Draw grids
    for(i=0; i<ROAD; i++)
        for(j=0; j<ROAD; j++)
            board[i][j]=grid[(i+ROAD-3)/(ROAD-2)][(j+ROAD-3)/(ROAD-2)];
            //For arguments i,j: 0->0, 1~(ROAD-2)->1, (ROAD-1)->2.

    //Mark special points
     board[3][3]		=board[3][ROAD/2]		=board[3][ROAD-4]
    =board[ROAD/2][3]	=board[ROAD/2][ROAD/2]	=board[ROAD/2][ROAD-4]
    =board[ROAD-4][3]	=board[ROAD-4][ROAD/2]	=board[ROAD-4][ROAD-4]="©I";
    printBoard();

    //Initialize parameters
    hand=1;
    for(i=0; i<HANDS; i++)
        for(j=0; j<2; j++)
            review[i][j]='\0';
}

int line(char x,char y)
{
    enum directions{heng,shu,pie,na};
    int lenth[4]={1,1,1,1};
    int i=1,maxlen=1;
//heng
    for(i=1; y+i<ROAD; i++)
        if(board[x][y+i]==board[x][y])lenth[heng]++;else break;
    for(i=1; y-i>=0; i++)
        if(board[x][y-i]==board[x][y])lenth[heng]++;else break;
//shu
    for(i=1; x+i<ROAD; i++)
        if(board[x+i][y]==board[x][y])lenth[shu]++;else break;
    for(i=1; x-i>=0; i++)
        if(board[x-i][y]==board[x][y])lenth[shu]++;else break;
//pie
    for(i=1; x+i<ROAD && y-i>=0; i++)
        if(board[x+i][y-i]==board[x][y])lenth[pie]++;else break;
    for(i=1; x-i>=0 && y+i<ROAD; i++)
        if(board[x-i][y+i]==board[x][y])lenth[pie]++;else break;
//na
    for(i=1; x+i<ROAD && y-i>=0; i++)
        if(board[x+i][y+i]==board[x][y])lenth[na]++;else break;
    for(i=1; x-i>=0 && y+i<ROAD; i++)
        if(board[x-i][y-i]==board[x][y])lenth[na]++;else break;

    for(i=0; i<4; i++)
        if(lenth[i]>maxlen)maxlen=lenth[i];
    return maxlen;
}

#undef ROAD
#undef HANDS
#undef WHITE
#undef BLACK
#undef W_OR_B

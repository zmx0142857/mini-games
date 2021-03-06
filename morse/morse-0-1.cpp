//Convert Morse signal into comprehensible text, vise versa.

/*
    char    MorseCode   binary      decimal
    =======================================
    a       .-          1111 1101   253
    b       -...        0000 1000   8
    c       -.-.        0000 1010   10
    d       -..         0000 0100   4
    e       .           1111 1110   254
    f       ..-.        1111 0010   242
    g       --.         0000 0110   6
    h       ....        1111 0000   240
    i       ..          1111 1100   252
    j       .---        1111 0111   247
    k       -.-         0000 0101   5
    l       .-..        1111 0100   244
    m       --          0000 0011   3
    n       -.          0000 0010   2
    o       ---         0000 0111   7
    p       .--.        1111 0110   246
    q       --.-        0000 1101   13
    r       .-.         1111 1010   250
    s       ...         1111 1000   248
    t       -           0000 0001   1
    u       ..-         1111 1001   249
    v       ...-        1111 0001   241
    w       .--         1111 1011   251
    x       -..-        0000 1001   9
    y       -.--        0000 1011   11
    z       --..        0000 1100   12
*/
/*
    1       .----       1110 1111   239
    2       ..---       1110 0111   231
    3       ...--       1110 0011   227
    4       ....-       1110 0001   225
    5       .....       1110 0000   224
    6       -....       0001 0000   16
    7       --...       0001 1000   24
    8       ---..       0001 1100   28
    9       ----.       0001 1110   30
    0       -----       0001 1111   31
*/
/*
    ?       ..--..      1100 1100   204
    _       ..--.-      1100 1101   205
    "       .-..-.      1101 0010   210
    +       .-.-.       1110 1010   234
    .       .-.-.-      1101 0101   213
    @       .--.-.      1101 1010   218
    '       .----.      1101 1110   222
    =       -...-       0001 0001   17
    -       -....-      0010 0001   33
    /       -..-.       0001 0010   18
    ;       -.-.-.      0010 1010   42
    !       -.-.--      0010 1011   43
    ()      -.--.-      0010 1101   45
    ,       --..--      0011 0011   51
    :       ---...      0011 1000   56
*/
/*
    Ŝ       ...-.       1110 0010   226
    É       ..-..       1110 0100   228
    Ü       ..--        1111 0011   243
    Ð       ..--.       1110 0110   230
    È       .-..-       1110 1001   233
    Ä       .-.-        1111 0101   245
    Þ       .--..       1110 1100   236
    À       .--.-       1110 1101   237
    Ĵ       .---.       1110 1110   238
    Ç       -.-..       0001 0100   20
    Ĥ       -.--.       0001 0110   22
    Ĝ       --.-.       0001 1010   26
    Ñ       --.--       0001 1011   27
    Ö       ---.        0000 1110   14
    CH      ----        0000 1111   15
*/

#include<iostream>
using namespace std;

class Morse
{
public:
    static string decode(string);
    static string encode(string);
protected:
    static string arrDecode[256];
    static string arrEncode[128];
    static unsigned decodeUnit(string);    //Convert a Morse unit into a number (0-255)
};

string Morse::arrDecode[256]
    ={" ","t","n","m","d","k","g","o",  "b","x","c","y","z","q","","CH",// 0- 15
      "6","" ,"/","" ,"" ,"" ,"" ,"" ,  "7","" ,"" ,"" ,"8","" ,"9","0",// 16- 31
      "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,  "" ,"-",";","!","(",")","" ,"" ,// 32- 47
      "" ,"" ,"" ,",","" ,"" ,"" ,"" ,  ":","" ,"" ,"" ,"" ,"" ,"" ,"" ,// 48- 63
      "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,  "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,// 64- 79
      "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,  "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,// 80- 95
      "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,  "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,// 96-111
      "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,  "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,//112-127
      "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,  "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,//128-143
      "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,  "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,//144-159
      "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,  "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,//160-175
      "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,  "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,//176-191
      "" ,"" ,"" ,"" ,"" ,"" ,"" ,"" ,  "" ,"" ,"" ,"" ,"?","_","" ,"" ,//192-207
      "" ,"" ,"\"","","" ,".","" ,"" ,  "" ,"" ,"@","" ,"" ,"" ,"\'","",//208-223
      "5","4","" ,"3","" ,"" ,"" ,"2",  "" ,"" ,"+","" ,"" ,"" ,"" ,"1",//224-239
      "h","v","f","" ,"l","" ,"p","j",  "s","u","r","w","i","a","e","", //240-255
     };

string Morse::decode(string str)
{
    string s;
    char c='\0';
    unsigned spacecount=1, i=0;
    for(; (c=str[i])!='\0'; i++)
    {
        if((c=='-' || c=='.') && spacecount>0)
        {
            s=s+arrDecode[decodeUnit(&str[i])];
                // 将str的第i位作为新的str的开头传递给unit函数
            spacecount=0;
        }
        else if(c!='-' && c!='.')
            spacecount++;

        if(c=='/' ||c=='\\' ||c=='|' ||spacecount==3)
            s=s+' ';
    }
    return s;
}

unsigned Morse::decodeUnit(string str)
{
    char n='\0';
    if(str[0]=='.')
        n='\xff';
    char c=0;
    for(unsigned i=0; (c=str[i])!='\0'; i++)
    {
        n<<=1;          //低位补0
        if(c=='-')
            n^=0x1;
        else if(c=='.')
            n&=0xfe;
        else
        {
            n>>=1;      //原高位是什么，高位就补什么
            break;
        }
    }
    return unsigned(n & 0xff);
}

string Morse::arrEncode[128]
    ={""      ,""      ,""      ,""      ,""      ,""      ,""      ,""      ,//  0-  7
      ""      ,""      ,""      ,""      ,""      ,""      ,""      ,""      ,//  8- 15
      ""      ,""      ,""      ,""      ,""      ,""      ,""      ,""      ,// 16- 23
      ""      ,""      ,""      ,""      ,""      ,""      ,""      ,""      ,// 24- 31
      "  "    ,"-.-.--",".-..-.",""      ,""      ,""      ,""      ,".----.",// 32- 39
      "-.--..","-.--.-",""      ,".-.-." ,"--..--","-....-",".-.-.-","-..-." ,// 40- 47
      "-----" ,".----" ,"..---" ,"...--" ,"....-" ,"....." ,"-...." ,"--..." ,// 48- 55
      "---.." ,"----." ,"---...","-.-.-.",""      ,"-...-" ,""      ,"--..--",// 56- 63
      ".--.-.",".-"    ,"-..."  ,"-.-."  ,"-.."   ,"."     ,"..-."  ,"--."   ,// 64- 71
      "...."  ,".."    ,".---"  ,"-.-"   ,".-.."  ,"--"    ,"-."    ,"---"   ,// 72- 79
      ".--."  ,"--.-"  ,".-."   ,"..."   ,"-"     ,"..-"   ,"...-"  ,".--"   ,// 80- 87
      "-..-"  ,"-.--"  ,"--.."  ,""      ,""      ,""      ,""      ,"..--.-",// 88- 95
      ""      ,".-"    ,"-..."  ,"-.-."  ,"-.."   ,"."     ,"..-."  ,"--."   ,// 96-103
      "...."  ,".."    ,".---"  ,"-.-"   ,".-.."  ,"--"    ,"-."    ,"---"   ,//104-111
      ".--."  ,"--.-"  ,".-."   ,"..."   ,"-"     ,"..-"   ,"...-"  ,".--"   ,//112-119
      "-..-"  ,"-.--"  ,"--.."  ,""      ,""      ,""      ,""      ,""      ,//120-127
      };

string Morse::encode(string str)
{
    string s="";
    for(unsigned i=0; str[i]!='\0'; i++)
        s=s+arrEncode[str[i]]+' ';
    return s;
}


int main()
{
    cout<<"******** Welcome to the Morse Code Translator 1.0 by CLAYCRAFT ********\n"
    <<"    You can type either Morse code or English text below.\n"
    <<"    Tips:\n"
    <<"      * Split the Morse units with either a space' ', or a comma','.\n"
    <<"      * A '/','\\' or '|' will separate the words.\n"
    <<"      * For example, try typing:\n"
    <<"          .... . .-.. .-.. --- --..-- /.-- --- .-. .-.. -.. -.-.--\n"
    <<"      * Then you can type the words you see to check if this translation is\n"
    <<"        reversible."<<endl<<endl<<">>> ";
    string s;
    char c;
    while(1)
    {
        cin>>s;
        c=cin.get();
        if(s[0]=='-' || s[0]=='.' || s[0]=='/' || s[0]=='\\' || s[0]=='|')
        {
            cout<<Morse::decode(s);
        }
        else
        {
            cout<<Morse::encode(s);
            if(c==' ')
                cout<<'/';
        }
        if(c=='\n')
            cout<<endl<<endl<<">>> ";
    }
    return 0;
}

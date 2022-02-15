/*
 * Name:       CubeFont-2.0
 * Author:     zmx0142857
 * Description:Input 2D graphics, output 3D.
 * Examples:
 *                                              _____
 *                                             /\    \
 *                                            /::\____\
 *                                           /:::/    /
 *                                          /:::/    /
 *                                         /:::/    /
 *              00      00                /:::/____/
 *              00      00               /::::\    \
 *              00      00              /::::::\    \   _____
 *              00      00             /:::/\:::\    \ /\    \
 *              0000000000      ->    /:::/  \:::\    /::\____\
 *              0000000000            \::/    \:::\  /:::/    /
 *              00      00             \/____/ \:::\/:::/    /
 *              00      00                      \::::::/    /
 *              00      00                       \::::/    /
 *              00      00                       /:::/    /
 *                                              /:::/    /
 *                                             /:::/    /
 *                                            /:::/    /
 *                                            \::/    /
 *                                             \/____/
 *                                           _____________
 *                 /000\                    /::::::::\    \
 *                /00000\                  /::::::::::\    \
 *               /0000000\                /:::/~~\:::::\____\
 *              /0/   \000               /::::\~~ \:::::|    |
 *              00     000              /::::::\   \::::|    |
 *              0000000000      ->     /:::/\:::\   |:::|    |
 *              0000000000             \::/  \:::\  |:::|    |
 *              00     000              \/____\:::\/::::|    |
 *              00     000                     \::::::::|____|
 *              00     000                      \::::::/    /
 *                                              /:::::/    /
 *                                             /:::::/    /
 *                                            /:::::/    /
 *                                            \::::/    /
 *                                             \::/    /
 *                                              \/____/
 *                                               _____          
 *                                              /\    \
 *                                             /::\    \
 *                                            /::::\    \
 *                                           /::::::\    \
 *               00000000\                  /:::/\:::\    \
 *               000000000                 /:::/__\:::\    \
 *               00    \00                /::::\   \:::\    \
 *               00    /0/               /::::::\   \:::\ ___\
 *               00000000\       ->     /:::/\:::\   |:::|    | 
 *               000000000\            /:::/__\:::\__|:::|____| 
 *               00     \00            \:::\   \:::::::::/   /  
 *               00     /00             \:::\   \:::::::/___/   
 *               000000000/              \:::\   \:::|~~   |    
 *               00000000/                \:::\   |::|     |    
 *                                         \:::\__|::|_____|    
 *                                          \::::::::/    /     
 *                                           \::::::/    /      
 *                                            \::::/    /       
 *                                             ~~~~~~~~~        
 */

#include <iostream>
#include <vector>
#include <cctype> // isspace()
using std::istream;
using std::ostream;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

typedef vector<string> Map;
struct Pixel
{
	char type;				// the char from the inbuffer
	int thickness;
	bool hastop;			// true if exist a pixel on the top
	bool hasbottom;
	bool hasleft;
	bool hasright;
	bool hasrightbottom;
	bool has_slope_on_right;
	bool has_slope_on_right_bottom;
	
	static Map img_data[5];
	enum PixelType {
		BLOCK, RIGHT_BOTTOM, LEFT_TOP, LEFT_BOTTOM, RIGHT_TOP
	};

	Pixel(const Map &, int r, int c);
	bool empty() const;		// true iff this is empty
	const Map img() const;	// return proper image for this
};

class CubeFont
{
friend istream &operator>>(istream &, CubeFont &);
friend ostream &operator<<(ostream &, const CubeFont &);

protected:
	void build();			// build outbuffer by calling build_pixel()
	void build_pixel(const Map &, int r, int c);
							// print pixel, resize outbuffer if necessary

	Map inbuffer;
	Map outbuffer;
};

istream &operator>>(istream &is, CubeFont &rhs)
{
	rhs.inbuffer.clear();
	string line;
	while (getline(is, line))
		rhs.inbuffer.push_back(line);
	rhs.build();
	return is;
}

ostream &operator<<(ostream &os, const CubeFont &rhs)
{
	for (const auto &line: rhs.outbuffer)
		os << line << '\n';
	return os;
}

void CubeFont::build()
{
	outbuffer.clear();

	// build from bottom/base, so this is in reversed order
	const int height = inbuffer.size();
	for (int r = height - 1; r >= 0; --r)
	{
		const int width = inbuffer[r].size();
		for (int c = 0; c < width; ++c)
		{
			Pixel p(inbuffer, r, c);
			if (p.empty()) continue;
			// use coordinate transformation
			build_pixel(p.img(), c+r, c-r+height-2);
		}
	}
}

void CubeFont::build_pixel(const Map &img, int row, int col)
{
	int sz = outbuffer.size();
	int newsz = row + img.size();
	if (newsz > sz)
		outbuffer.resize(newsz);
	for (int i = 0; i < img.size(); ++i)
	{
		sz = outbuffer[row+i].size();
		newsz = col + img[i].size();
		if (newsz > sz)
			outbuffer[row+i].resize(newsz, ' ');
		for (int j = 0; j < img[i].size(); ++j)
			outbuffer[row+i][col+j] = img[i][j];
	}
}

/* Visualize:
 * 
 *  BLOCK       RIGHT_BOTTOM  LEFT_TOP	   LEFT_BOTTOM RIGHT_TOP
 *
 *	:\    \     ::            _______      |:          :|    |
 *	:/    /     ~~~~~~~       ::\    \     |:          :|    |
 *
 */
Map Pixel::img_data[5] = {
	{
		":\\",
		":/"
	},
	{
		"::",
		"~~"
	},
	{
		"__",
		"::\\"
	},
	{
		"|:",
		"|:"
	},
	{
		":|",
		":|"
	}
};

Pixel::Pixel(const Map &map, int r, int c)
{
	type = map[r][c];
	thickness = 4;
	hastop = r != 0 && c < int(map[r-1].size())
			&& !isspace(map[r-1][c]);
	hasbottom = r != int(map.size())-1	&& c < int(map[r+1].size())
			&& !isspace(map[r+1][c]);
	hasleft = c != 0 && !isspace(map[r][c-1]);
	hasright = c != int(map[r].size())-1 && !isspace(map[r][c+1]);
	hasrightbottom = r != int(map.size())-1 && c < int(map[r+1].size())-1
			&& !isspace(map[r+1][c+1]);
	has_slope_on_right = c != int(map[r].size())-1 && map[r][c+1] == '\\';
	has_slope_on_right_bottom =
			r != int(map.size())-1 && c < int(map[r+1].size())-1
			&& map[r+1][c+1] == '\\';
}

bool Pixel::empty() const
{
	return type != '0' && type != '/' && type != '\\';
}

const Map Pixel::img() const
{
	int choice = 0;
	switch (type)
	{
		case '0':	choice = BLOCK;									break;
		case '/':	choice = RIGHT_BOTTOM + !(hastop && hasleft);	break;
		case '\\':	choice = LEFT_BOTTOM + !(hastop && hasright);	break;
		default:	cout << "\nerror: img() called by void Pixel\n";break;
	}
	Map &img = img_data[choice];

	char fill[2] = {' ', ' '};
	if (choice == BLOCK || choice == RIGHT_TOP)
	{
        if (!hasleft)
            img[0][0] = '/';				// left edge
        else
            img[0][0] = ':';

        if (!hasleft && !hastop)
			fill[0] = '~';					// upper line
        else if (
				choice == BLOCK
			&&
				((!hasright && !hastop) || has_slope_on_right)
			)
		{
			fill[0] = '_';					// mid line
		}
	}
	else if (choice == LEFT_TOP)
	{
		fill[0] = '_';
	}
	else if (choice == RIGHT_BOTTOM)
	{
		fill[0] = '~';
	}

    if (choice == BLOCK || choice == LEFT_TOP || choice == RIGHT_TOP)
    {
        if (!hasbottom)
            img[1][0] = '\\';			// bottom edge
        else
            img[1][0]=':';

        if (
			(
				hasrightbottom
			&&
				(choice != RIGHT_TOP || !has_slope_on_right_bottom)
			)
        ||
            (choice == BLOCK && !hasbottom)
        ||
            (choice == RIGHT_TOP && !has_slope_on_right_bottom)
        )
		{
			fill[1] = '_';					// lower line
		}

    }
	if (choice != LEFT_BOTTOM)
		return Map{
			img[0] + string(thickness, fill[0])
				+ (choice == RIGHT_BOTTOM ? ' ': img[0][1]),
			img[1] + string(thickness, fill[1]) + img[1][1]
		};
	return img;
}

int main(int argc, char **argv)
{
	cout << "Input a 2D map, using '/', '\\', '0' and ' ':" << endl;
	CubeFont cubefont;
	cin >> cubefont;
	cout << cubefont;
	return 0;
}

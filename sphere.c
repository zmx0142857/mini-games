#include <stdio.h>
#include <math.h>

#define inv_of_sqrt3 0.5773502692

int main()
{
    double x, y, z;
    unsigned index;
    for (y = 1; y >= -1; y -= 0.05)
    {
        for (x = -1; x <=1; x += 0.025)
        {
            z = sqrt(1.0 - x*x - y*y);

            // the value of x+y+z is between -sqrt3 and +sqrt3
            // 0.5 is for rounding (si4 she4 wu3 ru4)
            index = ((x + y + z) * inv_of_sqrt3 + 1.0) * 5.0 + 0.5;
            putchar(x*x + y*y > 1.0 ? 'M' : "@@%#*+=;:. "[index]);
        }
        putchar('\n');
    }
    return 0;
}

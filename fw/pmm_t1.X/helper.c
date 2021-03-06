#include <math.h>
#include <stdlib.h>
#include <stdint.h> 

//****************************************************************************
//****************************************************************************
// set of helper routines to handle the floating point numbera 
//****************************************************************************

unsigned char intToStr(int x, char str[], unsigned char d);
void reverse(char *str, unsigned char len);

// reverses a string 'str' of length 'len'
void reverse(char *str, unsigned char len)
{
    unsigned char i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}
 
 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
unsigned char intToStr(int x, char str[], unsigned char d)
{
    unsigned char i = 0;
//	unsigned char k = 0;
	for (i=0;i<50;i++)	//this is ugly, isn't it?
		{
		if (x>0)
			{
			str[i] = (x%10) + '0';
			x = x/10;
			}
		else
			{
			if (i<d)
				str[i] = '0';
			else
				break;	//meh because of previous meh
			}
		}
    reverse(str, i);
    str[i] = '\0';
    return i;
}
 

/*
 ftoa(n, res, afterpoint)
n          --> Input Number
res[]      --> Array where output string to be stored
afterpoint --> Number of digits to be considered after point.

For example ftoa(1.555, str, 2) should store "1.55" in res and
ftoa(1.555, str, 0) should store "1" in res.
 
 * 
 * http://www.geeksforgeeks.org/convert-floating-point-number-string/
 */


const unsigned int ten_powers[5] = {1,10,100,1000,10000};
// Converts a floating point number to string.
unsigned char xftoa(float n, char *res, unsigned char bpoint, unsigned char apoint)
{
    // Extract integer part
    int ipart = (int)n;
    // Extract floating part
    float fpart = n - (float)ipart;
    // convert integer part to string
    int i = intToStr(ipart, res, bpoint);
    // check for display option after point
    if (apoint != 0)
    {
        res[i] = '.';  // add dot
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        //fpart = fpart * powd(10, afterpoint);
		fpart = fpart * ten_powers[apoint];
        intToStr((int)fpart, res + i + 1, apoint);
    }
return i + 	apoint + 1;
}

/*
long powd(int a, int b)
	{
	long x;
	unsigned char i;
	x = a;
	for (i=0;i<b-1;i++)
		{
		x = x*a;
		}
	return x;
	}
*/
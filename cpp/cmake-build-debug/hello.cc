// a small C++ program
#include <iostream>



int foo(double x, int y)
{
	if (x > 1.0)
		return y;
	else
		return 0;
}

int main()
{
	int i = 10;
	double d = 5.0;
	int c = 1;
	bool t = true;
	bool f = false;

	while (i > 1)
	{
		i--;
	}

	/*bool b = t && f;

	if (b)
	{
		return 1;
	} else 
	{
		return 0;
	}*/
	return i;
}


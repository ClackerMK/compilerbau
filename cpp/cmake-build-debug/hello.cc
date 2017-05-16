// a small C++ program
#include <iostream>



int foo(double x, int y)
{
	return y + 9 ;
}

int main()
{
	int i = 1;
	double d = 0.9;
	int a,b,c;

	i = foo(d,i);

	return i;
}


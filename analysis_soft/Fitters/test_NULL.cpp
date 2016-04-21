#include <iostream>

using namespace std;

int main()
{
	double* p=0;

	if (p==NULL)
	{
		std::cout << "(p == NULL) = true" << std::endl;
	}
	else if (p==0)
	{
		std::cout << "(p == 0) = true" << std::endl;
	}

	return 0;
	
}

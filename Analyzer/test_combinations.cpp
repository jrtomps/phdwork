#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <boost/bind.hpp>
using namespace std;

vector<string> data;

unsigned int 
fill_digits(unsigned int pos, 
			string curr, 
			unsigned int max_val)
{
	if (pos>0)
	{
		unsigned int cval;
		if (curr.length()>0)	
		{
			istringstream ss(curr);
			ss.seekg(-1,ios::end);
			ss >> cval; 
		}
		else
			cval = 0;
		
		while (cval<=max_val)	
		{
			ostringstream ss0(curr, ios::out| ios::app);
//			ss0.seekp(-1,ios::end);
			ss0 << (cval);	
			fill_digits(pos-1, ss0.str(), max_val);
			cval++;
		}

	}
	else
	{
		unsigned int cval;
		if (curr.length()>0)	
		{
			stringstream ss(curr);
			ss.seekg(-1,ios::end);
			ss >> cval; 
		}
		else
			cval = 0;

		while (cval <= max_val)
		{
			ostringstream ss(curr, ios::out | ios::app);
			ss << cval;
			data.push_back(ss.str());
			cval++;	
		}
	}
	
}

unsigned int c = 0;

void print(unsigned int w, string str)
{
	cout << left 
		<< setw(4) << c 
		<< ": "
		<< right 
		<< setw(w) << str 
		<< endl;

	c++;
}

int main()
{
	unsigned int npos = 0;
	unsigned int max_val = 0;

	cout << "Enter n choices : ";
	cin >> npos;
	cout << "Enter n values  : ";
	cin >> max_val;
	
	fill_digits(npos-1,"", max_val-1);

	cout << std::right; 
	for_each(data.begin(), data.end(), boost::bind(print,npos+1,_1));

	return 0;
}


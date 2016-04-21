#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include "DataElement.h"
using namespace std;

void ReadFloatGatesFromFile(std::string fname, vector<Float_t>& vec1, vector<Float_t>& vec2)
{
  ifstream file(fname.data(),ifstream::in);
  if (! file.is_open()) 
    {
      cout << fname << " doesn't exist" << endl;
      return;
    }

  vec1.clear();
  vec2.clear();

  Float_t val1, val2;

  string aline;
  stringstream ss;
  while (1)
    {
      getline(file,aline);
      if (file.eof()) break;
      else if (file.bad())  { cout << "Bad bit set" << endl; break; }
      else if (file.fail()) { cout << "Fail bit set" << endl; break; }
      else if (aline.length()==0 || aline.find_first_not_of(" \t")==string::npos) continue;
      else
      {
          ss.clear(); ss.str("");
          ss << aline;
          ss >> val1 >> val2;
          if (ss.rdstate()==0)
          {
              vec1.push_back(val1);
              vec2.push_back(val2);
          }
      }
  }

  file.close();
}


//int
//main()
//{
//  string fname;
//  cout << "Enter filename : ";
//  cin  >> fname;
//
//  vector<Float_t> val1;
//  vector<Float_t> val2;
//
//  ReadFloatGatesFromFile(fname.data(), val1, val2);
//
//  DataElement de(val1);
//
//  return 0;
//}


#include <vector>
#include <iostream>
#include <algorithm>


template<class T>
Bool_t ContainsDifferentElements(std::vector<T>& vec)
{
   Bool_t diff = false;

   if (vec.size() != 0)
     {
       T val = vec[0];
//       std::vector<T>::iterator it;
//       for (it=vec.begin(); it!=vec.end(); it++) if (*it != val) diff = true;
       for (UInt_t i=0; i<vec.size(); i++) if (vec[i] != val) diff = true;
     }

   return diff;
}

template<class T>
void Print(T& v)
{
  std::cout << v << ' ' <<  std::endl;
}

template<class T>
void PrintVector(std::vector<T>& vec)
{
  if (ContainsDifferentElements(vec))
    { 
      for (UInt_t i=0; i<vec.size(); i++) 
        std::cout<< vec[i] << ' ' << std::endl;
    }
  else if ( vec.size() > 0 )
    std::cout << vec.size() << " identical entries, val=" << vec.at(0) << std::endl;   

}



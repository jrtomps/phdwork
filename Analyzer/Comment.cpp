// Comment.cpp
//
// Jeromy Tompkins
// 7/20/2011
//
// A class that allows the saving of comments.

#include <list>
#include <string>
#include <iostream>
#include <iomanip>
#include "TObject.h"
#include "Comment.h"

ClassImp(Comment)

Comment::Comment (void)
: std::list<std::string>()
{}

Comment::Comment (Comment const& obj)
: std::list<std::string>(obj)
{}

Comment::~Comment (void)
{}

Comment& Comment::operator=(Comment const& obj)
{
  if (this!=&obj)
    *this = obj;
}


void Comment::PrintAll(void)
{
  Int_t i=0;
  std::list<std::string>::iterator it;
  for (it=begin(); it!=end(); it++)
    {
      std::cout << std::setw(4) << std::left <<  i
		<< (*it)
		<< std::endl;
      i++;
    }
}

void Comment::AddComment(void)
{
  std::string str;
  std::cout << "Enter your comment" << std::endl;
  getline(std::cin, str);
  push_back(str);
}

void Comment::AddComment(const std::string& str)
{
  push_back(str);
}


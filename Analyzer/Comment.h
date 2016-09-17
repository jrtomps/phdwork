// Comment.h
//
// Jeromy Tompkins
// 7/20/2011
//
// A class that allows the saving of comments.

#include <list>
#include <string>
#include "TObject.h"

#ifndef COMMENT_H
#define COMMENT_H

class Comment : public std::list<std::string>
{
 public:
  Comment (void);
  Comment (Comment const& obj);
  ~Comment (void);
  Comment& operator=(Comment const& obj);

  void PrintAll(void);
  void AddComment(void);
  void AddComment(const std::string& new_comment);

 private:
  ClassDef(Comment,1);
};
#endif

#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <boost/lexical_cast.hpp>

#include <muduo/base/StringPiece.h>
#include <string>

inline std::string convert_muduo_string_to_std(muduo::string s)
{
  return std::string(s.data(), s.size());
}

inline std::string int_to_string(int a)
{
  return boost::lexical_cast<std::string>(a);
}

#endif

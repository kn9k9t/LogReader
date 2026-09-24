#include "StringViewUtils.h"
//-----------------------------------------------
#include <boost/algorithm/string.hpp>
//-----------------------------------------------
std::string_view readUntilAndCutWithTrim(std::string_view & str, char delim)
{
  const auto delimPos = str.find(delim);
  auto substr = str.substr(0, delimPos);
  str = str.substr(delimPos + 1);

  trim(substr);
  trimLeft(str);
  return substr;
}
//-----------------------------------------------
void trimLeft(std::string_view & str)
{
  while (str.starts_with(" "))
  {
    str.remove_prefix(1);
  }
}
//-----------------------------------------------
void trimRight(std::string_view & str)
{
  while(str.ends_with(" "))
  {
    str.remove_suffix(1);
  }
}
//-----------------------------------------------
void trim(std::string_view & str)
{
  trimLeft(str);
  trimRight(str);
}
//-----------------------------------------------

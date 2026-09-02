#pragma once
//-----------------------------------------------
#include <string>
#include <cstdint>
//-----------------------------------------------
struct LogRow
{
  //uint64_t    _time;
  std::string _readTime;
  std::string _level;
  std::string _objectName;
  std::string _objectId;
  std::vector<std::string> _msg;
};
typedef std::vector<LogRow> VecLogRow;
//-----------------------------------------------
// std::ostream & operator<<(std::ostream & os, const LogRow & obj)
// {
//   os << obj._readTime << " | " << obj._level << " | " << obj._objectName << " | " << obj._objectId << " | " << obj._msg;
//   return os; 
// }
//-----------------------------------------------
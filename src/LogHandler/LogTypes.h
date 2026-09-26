#pragma once
//-----------------------------------------------
#include <string>
#include <vector>
//-----------------------------------------------
struct LogRow
{
  std::string _readTime;
  std::string _level;
  std::string _objectName;
  std::string _objectId;
  std::string _rawMsg;
  std::vector<std::string> _msg;
};
typedef std::vector<LogRow> VecLogRow;
//-----------------------------------------------

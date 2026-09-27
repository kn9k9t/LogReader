#pragma once
//-----------------------------------------------
#include <string>
#include <vector>
#include <memory>
//-----------------------------------------------
struct LogRowFiltersData
{
  std::string _readTime;
  std::string _level;
  std::string _objectName;
  std::string _objectId;
};
//-----------------------------------------------
struct LogRow
{
  std::shared_ptr<LogRowFiltersData> _filterData;
  std::string                        _rawMsg;
};
typedef std::vector<LogRow> VecLogRow;
//-----------------------------------------------

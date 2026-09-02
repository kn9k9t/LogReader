#pragma once
//-----------------------------------------------
#include <fstream>
#include <vector>
#include "LogTypes.h"
#include <iostream>
#include <sstream>
#include <string_view>
#include <chrono>
#include <boost/algorithm/string.hpp>
//-----------------------------------------------
class LogHandler
{
public:
  LogHandler(const std::string & path) :
    _path(path) 
  {}

  bool readLog()
  {
    std::string path = _path;
    std::fstream logFile;
    logFile.open(path);
    if (!logFile.is_open())
    {
      std::cout << "Can't open file: " << path;
      return false;      
    }
    
    for (std::string line; std::getline(logFile, line);)
    {
      parseLine(line);
    }

    return true;
  }

  const auto & getLog() const { return _log; }

private:
  std::string         _path;
  std::vector<LogRow> _log;

  bool parseLine(const std::string & line)
  {
    if (line.empty())
      return false;
    std::string_view lineStr(line);
    trimLeft(lineStr);
    if (lineStr.starts_with("***"))
      return false;

    if (lineStr.find("|") == std::string_view::npos || lineStr.starts_with("In") || lineStr.starts_with("Out"))
    {
      if (_log.empty())
        return false;

      auto & lastRow = _log.back();
      lastRow._msg.push_back(line);
      //lastRow._msg.append("\n" + std::string(lineStr));
      return true;
    }

    LogRow logRow;
    auto dateTime = readUntilAndCutWithTrim(lineStr, '|');
    logRow._readTime = dateTime;
    if (lineStr.starts_with("***"))
      return false;
    //logRow._time = decodeTimestamp(dateTime);
    logRow._level = readUntilAndCutWithTrim(lineStr, '|');
    boost::to_upper(logRow._level);
    logRow._objectName = readUntilAndCutWithTrim(lineStr, '|');
    logRow._objectId = readUntilAndCutWithTrim(lineStr, '|');
    logRow._msg.push_back(std::string(readUntilAndCutWithTrim(lineStr, '|')));

    _log.emplace_back(std::move(logRow));

    return true;
  }

  // uint64_t decodeTimestamp(std::string_view dateTime)
  // {
  //   std::chrono::sys_time<std::chrono::milliseconds> mSec;
  //   std::string dateTimeStr(dateTime);
  //   std::istringstream stream{dateTimeStr};
  //   stream >> std::chrono::parse("%y%m%d %T", mSec);
  //   if (stream.fail())
  //     return 0;

  //   auto mSecVal = dateTime.substr(dateTime.rfind(':') + 1);
  //   mSec += std::chrono::milliseconds{std::atoi(mSecVal.data())};
  //   return mSec.time_since_epoch().count();
  // }

  std::string_view readUntilAndCutWithTrim(std::string_view & str, char delim)
  {
    const auto delimPos = str.find(delim);
    auto substr = str.substr(0, delimPos);
    str = str.substr(delimPos + 1);

    trimStringView(substr);
    trimLeft(str);
    return substr;
  }

  void trimLeft(std::string_view & str)
  {
    while (str.starts_with(" "))
    {
      str.remove_prefix(1);
    }
  }

  void trimRight(std::string_view & str)
  {
    while(str.ends_with(" "))
    {
      str.remove_suffix(1);
    }
  }
  void trimStringView(std::string_view & str)
  {
    trimLeft(str);
    trimRight(str);
  }
};
//-----------------------------------------------
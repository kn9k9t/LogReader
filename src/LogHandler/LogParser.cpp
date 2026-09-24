#include "LogParser.h"
//-----------------------------------------------
#include "StringViewUtils.h"
//-----------------------------------------------
#include <boost/algorithm/string.hpp>
//-----------------------------------------------
LogParser::LogParser(ILogParserUser & user) : 
  _user(user) 
{}
//-----------------------------------------------
LogParser::~LogParser()
{
  _user.onSomeLogWasRead(std::move(_log));
}
//-----------------------------------------------
bool LogParser::parseLine(const std::string & line)
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
    return true;
  }
  LogRow row;

  auto dateTime = readUntilAndCutWithTrim(lineStr, '|');
  row._readTime = dateTime;

  if (lineStr.starts_with("***"))
    return false;
  
  row._level = readUntilAndCutWithTrim(lineStr, '|');
  boost::to_upper(row._level);
  row._objectName = readUntilAndCutWithTrim(lineStr, '|');
  row._objectId = readUntilAndCutWithTrim(lineStr, '|');
  row._msg.push_back(std::string(readUntilAndCutWithTrim(lineStr, '|')));

  _log.push_back(row);
  //if (_log.empty() == false && _log.size() % BunchSize == 0)
  if (_log.size() >= BunchSize)
  {
    _user.onSomeLogWasRead(std::move(_log));
    _log.clear();
  }

  return true;
}
//-----------------------------------------------
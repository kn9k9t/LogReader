#pragma once
//-----------------------------------------------
#include "LogTypes.h"
//-----------------------------------------------
class ILogParserUser
{
public:
  virtual ~ILogParserUser() = default;

  virtual void onSomeLogWasRead(VecLogRow logPart) = 0;
};
//-----------------------------------------------
class LogParser
{
public:
  LogParser(ILogParserUser & user);
  ~LogParser();

  bool parseLine(const std::string & line);

private:
  enum
  {
    BunchSize = 100
  };
  ILogParserUser & _user;
  VecLogRow _log;
};
//-----------------------------------------------

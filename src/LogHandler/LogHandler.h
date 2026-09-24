#pragma once
//-----------------------------------------------
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string_view>
#include <chrono>
#include <thread>
#include <mutex>
#include <filesystem>
#include <boost/algorithm/string.hpp>
// #include <boost/asio/io_context.hpp>
//-----------------------------------------------
#include "LogTypes.h"
#include "LogParser.h"
//-----------------------------------------------
class ILogHandlerUser
{
public:
  virtual ~ILogHandlerUser() = default;

  virtual void onSomeLogWasRead(VecLogRow logPart) = 0;
};
//-----------------------------------------------
class LogHandler : private ILogParserUser
{
public:
  LogHandler(const std::string & path, ILogHandlerUser & user);
  ~LogHandler();

  void run();
  void stop();

private:
  std::thread         _thread;
  bool                _running;
  mutable std::mutex  _lock;

  std::string                     _path;
  std::filesystem::file_time_type _lastWriteTime;

  ILogHandlerUser & _user;

  void watchLogChangesAndReadIfNeeded();

  bool isLogChanged() const;

  bool readLog();

  virtual void onSomeLogWasRead(VecLogRow logPart) override;
};
//-----------------------------------------------

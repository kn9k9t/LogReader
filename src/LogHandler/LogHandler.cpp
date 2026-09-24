#include "LogHandler.h"
//-----------------------------------------------
LogHandler::LogHandler(const std::string & path, ILogHandlerUser & user) :
  _running(false),
  _path(path),
  _user(user)
{}
//-----------------------------------------------
LogHandler::~LogHandler()
{
  stop();
}
//-----------------------------------------------
void LogHandler::run()
{
  _running = true;
  //_thread = std::thread(&LogHandler::watchLogChangesAndReadIfNeeded, this);
  _thread = std::thread([&]{ watchLogChangesAndReadIfNeeded(); });
}
//-----------------------------------------------
void LogHandler::stop()
{
  _running = false;
  if (_thread.joinable())
    _thread.join();
}
//-----------------------------------------------
void LogHandler::watchLogChangesAndReadIfNeeded()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  while (_running)
  {
    if (isLogChanged())
    {
      readLog();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
  }
}
//-----------------------------------------------
bool LogHandler::isLogChanged() const
{
  auto lastWriteTime = std::filesystem::last_write_time(_path);
  return lastWriteTime != _lastWriteTime;
}
//-----------------------------------------------
bool LogHandler::readLog()
{
  _lastWriteTime = std::filesystem::last_write_time(_path);
  
  std::fstream logFile;
  logFile.open(_path);
  if (!logFile.is_open())
  {
    std::cout << "Can't open file: " << _path;
    return false;      
  }

  LogParser parser(*this);
  for (std::string line; std::getline(logFile, line);)
  {
    parser.parseLine(line);
  }

  return true;
}
//-----------------------------------------------
void LogHandler::onSomeLogWasRead(VecLogRow logPart)
{
  _user.onSomeLogWasRead(std::move(logPart));
}
//-----------------------------------------------
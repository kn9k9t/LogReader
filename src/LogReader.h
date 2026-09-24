#pragma once
//-----------------------------------------------
#include "LogHandler/LogHandler.h"
#include "UI/UI.h"
//-----------------------------------------------
class LogReaderApp : public ILogHandlerUser
{
public:
  LogReaderApp(const std::string & logPath) :
    _logHandler(logPath, *this)
  {
    
  }

  void run()
  {
    _logHandler.run();
    
    _ui.init();
  }

  virtual void onSomeLogWasRead(VecLogRow logPart) override;

private:
  LogHandler _logHandler;
  UI         _ui;
};
//-----------------------------------------------
#include "LogReader.h"
//-----------------------------------------------
void LogReaderApp::onSomeLogWasRead(VecLogRow logPart)
{
  _ui.appendLog(std::move(logPart));
}
//-----------------------------------------------
#pragma once
//-----------------------------------------------
#include <map>
#include "ftxui/ftxui.hpp"
#include <boost/algorithm/string.hpp>
#include "../LogHandler/LogTypes.h"
//-----------------------------------------------
#include "TableRowComp.h"
//-----------------------------------------------
class UI
{
public:
  UI();

  void init();

  void appendLog(VecLogRow log);

private:
  App _screen;

  Component _objectNamesContainer;
  Component _levelFilterContainer;
  Component _objectIdFilterContainer;
  Component _logList;

  std::map<std::string, bool> _levelFilters;
  std::map<std::string, bool> _objectNamesFilters;
  std::map<std::string, bool> _objectIdsFilters;

  VecLogRow  _logData;

  void onFilterChanged();
};
//-----------------------------------------------
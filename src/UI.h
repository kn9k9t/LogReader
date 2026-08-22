#pragma once
//-----------------------------------------------
#include <map>
#include "ftxui/ftxui.hpp"
#include <boost/algorithm/string.hpp>
#include "LogTypes.h"
//-----------------------------------------------
using namespace ftxui;
//-----------------------------------------------
class TableRowComponent : public ComponentBase {
public:
    TableRowComponent(const LogRow & row) : _row(row) {}

    Element OnRender() override 
    {
      auto colorType = Color();
      if (boost::iequals(_row._level, "ERR"))
        colorType = Color::Red;
      if (boost::iequals(_row._level, "WRN"))
        colorType = Color::Yellow;
      if (boost::iequals(_row._level, "VRB"))
        colorType = Color::Blue;

      auto element = hbox({
        text(" "), text(_row._readTime)   | color(colorType), text(" "), separator(),
        text(" "), text(_row._level)      | color(colorType), text(" "), separator(),
        text(" "), text(_row._objectName) | color(colorType) | size(WIDTH, EQUAL, 10), text(" "), separator(),
        text(" "), text(_row._objectId)   | color(colorType) | size(WIDTH, EQUAL, 10), text(" "), separator(),
        text(" "), paragraph(_row._msg)   | color(colorType)
      });

      if (Focused()) 
      {
        element = focus(element);
      }

      return element;
    }

    bool Focusable() const override { return true; }

private:
    LogRow _row;
};
//-----------------------------------------------
class UI
{
public:
  UI() :
    _screen(ScreenInteractive::Fullscreen()),
    _logList(Container::Vertical({}))
  {}

  void init(VecLogRow log)
  {
    _logData = std::move(log);

    for (const auto & entry : _logData)
    {
      _objectNamesFilters.insert(std::make_pair(entry._objectName, true));

      auto levelStr = entry._level;
      boost::to_upper(levelStr);
      _levelFilters.insert(std::make_pair(levelStr, true));

      _objectIdsFilters.insert(std::make_pair(entry._objectId, true));
    }

    refreshLog(_logData);

    auto filtersCheckboxesOptions = CheckboxOption();
    filtersCheckboxesOptions.on_change = [this](){ onFilterChanged(); };

    auto objectNamesContainer = Container::Vertical({});
    for (auto & [objectName, value] : _objectNamesFilters)
    {
      objectNamesContainer->Add(Checkbox(objectName, &value, filtersCheckboxesOptions));
    }

    auto levelFilterContainer = Container::Vertical({});
    for (auto & [level, value] : _levelFilters)
    {
      levelFilterContainer->Add(Checkbox(level, &value, filtersCheckboxesOptions));
    }

    auto objectIdFilterContainer = Container::Vertical({});
    for (auto & [objectId, value] : _objectIdsFilters)
    {
      objectIdFilterContainer->Add(Checkbox(objectId, &value, filtersCheckboxesOptions));
    }

    std::string objectIdFilterValue;
    auto inputOptions = InputOption();
    inputOptions.multiline = false;
    inputOptions.on_change = [&]()
    {
      objectIdFilterContainer->DetachAllChildren();
      for (auto & [objectId, value] : _objectIdsFilters)
      {
        if (objectIdFilterValue.empty())
        {
          objectIdFilterContainer->Add(Checkbox(objectId, &value, filtersCheckboxesOptions));
          continue;
        }
        if (std::string_view(objectId).starts_with(objectIdFilterValue))
          objectIdFilterContainer->Add(Checkbox(objectId, &value, filtersCheckboxesOptions));
      }
    };
    auto objectIdFilterInput = Input(&objectIdFilterValue, inputOptions);

    auto configMenu = Container::Horizontal(
    {
      levelFilterContainer,
      objectNamesContainer,
      Container::Vertical({objectIdFilterInput, objectIdFilterContainer}),
    });

    int activeLayer = 0;
    _logList->TakeFocus();
    auto layout = Container::Tab(
    {
      _logList,
      configMenu,
    }, &activeLayer);
  
    auto layoutEventCatcher = CatchEvent(layout, [&](Event event)
    {
      if (event == Event::Tab) {
        if (activeLayer == 0)
        {
          activeLayer = 1;
          configMenu->TakeFocus();
        }
        else
        {
          activeLayer = 0;
          _logList->TakeFocus();  
        }
        return true;
      }
      return false;
    });

    auto renderer = Renderer(layoutEventCatcher, [&]()
    {
      if (activeLayer == 1)
      {
        return vbox(
          {
            text("Press TAB to close filters"),
            separator(),
            hbox(
            {
              vbox({text("Levels"), separator(), levelFilterContainer->Render()}) | border,
              vbox({text("Objects"), separator(), objectNamesContainer->Render() | yframe | vscroll_indicator}) | border,
              vbox({hbox({text("Objects IDs"), separator(), objectIdFilterInput->Render()}), separator(), objectIdFilterContainer->Render() | yframe | vscroll_indicator}) | border
            })
        });
      }
      else
      {
        return vbox(
        {
          text("Press TAB to open filters"),
          separator(),
          _logList->Render() | yflex | vscroll_indicator | yframe
        });
      }
    });

    _screen.Loop(renderer);
  }

  void refreshLog(const VecLogRow & log)
  {
    _logList->DetachAllChildren();
    for (const auto & row : log)
    {
      _logList->Add(Make<TableRowComponent>(row));
    }
  }

private:
  App _screen;

  Component _logList;
  Component _widgetObjectNameFilters;
  Component _widgetObjectIdFilters;
  Component _widgetLevelFilters;


  VecLogRow                   _logData;
  std::map<std::string, bool> _levelFilters;
  std::map<std::string, bool> _objectNamesFilters;
  std::map<std::string, bool> _objectIdsFilters;

  void onFilterChanged()
  {
    auto tmpLogData = _logData;

    tmpLogData.erase(std::remove_if(tmpLogData.begin(), tmpLogData.end(), [&](const auto & entry)
    {
      auto it = _objectNamesFilters.find(entry._objectName);
      if (it != _objectNamesFilters.end())
      {
        if (it->second == false) return true;
      }

      it = _levelFilters.find(entry._level);
      if (it != _levelFilters.end())
      {
        if (it->second == false) return true;
      }

      it = _objectIdsFilters.find(entry._objectId);
      if (it != _objectIdsFilters.end())
      {
        if (it->second == false) return true;
      }

      return false;
    }), tmpLogData.end());
    
    refreshLog(tmpLogData);
  }
};
//-----------------------------------------------
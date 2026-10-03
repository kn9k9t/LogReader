#include "UI.h"
#include "ScrollableContainer.h"
//-----------------------------------------------
UI::UI() :
  _screen(ScreenInteractive::Fullscreen()),
  _objectNamesContainer(Container::Vertical({})),
  _levelFilterContainer(Container::Vertical({})),
  _objectIdFilterContainer(Container::Vertical({})),
  _logList(Container::Scrollable({}))
{}
//-----------------------------------------------
void UI::init()
{
  auto filtersCheckboxesOptions = CheckboxOption();
  filtersCheckboxesOptions.on_change = [this](){ onFilterChanged(); };

  for (auto & [objectName, value] : _objectNamesFilters)
  {
    _objectNamesContainer->Add(Checkbox(objectName, &value, filtersCheckboxesOptions));
  }

  for (auto & [level, value] : _levelFilters)
  {
    _levelFilterContainer->Add(Checkbox(level, &value, filtersCheckboxesOptions));
  }

  for (auto & [objectId, value] : _objectIdsFilters)
  {
    _objectIdFilterContainer->Add(Checkbox(objectId, &value, filtersCheckboxesOptions));
  }

  std::string objectIdFilterValue;
  auto inputOptions = InputOption();
  inputOptions.multiline = false;
  inputOptions.on_change = [&]()
  {
    _objectIdFilterContainer->DetachAllChildren();
    for (auto & [objectId, value] : _objectIdsFilters)
    {
      if (objectIdFilterValue.empty())
      {
        _objectIdFilterContainer->Add(Checkbox(objectId, &value, filtersCheckboxesOptions));
        continue;
      }
      if (std::string_view(objectId).starts_with(objectIdFilterValue))
      {
        _objectIdFilterContainer->Add(Checkbox(objectId, &value, filtersCheckboxesOptions));
      }
    }
  };
  auto objectIdFilterInput = Input(&objectIdFilterValue, inputOptions);

  auto inverterMaker = [this](bool & val, auto & map)
  {
    auto option = CheckboxOption();
    option.on_change = [&]()
    {
      for (auto & [name, value] : map)
      {
        value = val;
      }
      onFilterChanged();
    };
    return Checkbox("CHANGE ALL", &val, option);
  };

  bool levelInverterValue = true, objectNameInverterValue = true, objectIdInverterValue = true;
  auto levelInverter = inverterMaker(levelInverterValue, _levelFilters);
  auto objectNameInverter = inverterMaker(objectNameInverterValue, _objectNamesFilters);
  auto objectIdInverter = inverterMaker(objectIdInverterValue, _objectIdsFilters);

  _levelFilterContainer->TakeFocus();
  auto configMenu = Container::Horizontal(
  {
    Container::Vertical({levelInverter, _levelFilterContainer}),
    Container::Vertical({objectNameInverter, _objectNamesContainer}),
    Container::Vertical({objectIdFilterInput, objectIdInverter, _objectIdFilterContainer}),
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
            vbox({text("Levels"), levelInverter->Render(), separator(), _levelFilterContainer->Render()}) | border,
            vbox({text("Objects"), objectNameInverter->Render(), separator(), _objectNamesContainer->Render() | yframe | vscroll_indicator}) | border,
            vbox({hbox({text("Objects IDs"), separator(), objectIdFilterInput->Render()}), objectIdInverter->Render(), separator(), _objectIdFilterContainer->Render() | yframe | vscroll_indicator}) | border
          })
      });
    }
    else
    {
      return vbox(
      {
        text("Press TAB to open filters"),
        separator(),
        _logList->Render() | yflex | yframe
      });
    }
  });

  _screen.Loop(renderer);
}
//-----------------------------------------------
void UI::appendLog(VecLogRow log)
{
  _screen.Post([&, log]()
  {
    _logData.insert(_logData.end(), log.begin(), log.end());

    bool isLastElementFocused = false;
    if (_logList->ChildCount() > 0)
    {
      _logList->ChildAt(_logList->ChildCount() - 1)->Focused();
    }

    updateFiltersData(log);

    for (const auto & row : log)
    {
      _logList->Add(Make<TableRowComponent>(row));
    }

    if (isLastElementFocused)
    {
      _logList->ChildAt(_logList->ChildCount() - 1)->TakeFocus();
    }

    updateFiltersWidgets();
  });
  _screen.RequestAnimationFrame();
}
//-----------------------------------------------
void UI::updateFiltersWidgets()
{
  auto filtersCheckboxesOptions = CheckboxOption();
  filtersCheckboxesOptions.on_change = [this](){ onFilterChanged(); };

  _objectNamesContainer->DetachAllChildren();
  for (auto & [objectName, value] : _objectNamesFilters)
  {
    _objectNamesContainer->Add(Checkbox(objectName, &value, filtersCheckboxesOptions));
  }

  _levelFilterContainer->DetachAllChildren();
  for (auto & [level, value] : _levelFilters)
  {
    _levelFilterContainer->Add(Checkbox(level, &value, filtersCheckboxesOptions));
  }

  _objectIdFilterContainer->DetachAllChildren();
  for (auto & [objectId, value] : _objectIdsFilters)
  {
    _objectIdFilterContainer->Add(Checkbox(objectId.empty() ? "<empty>" : objectId, &value, filtersCheckboxesOptions));
  }
}
//-----------------------------------------------
void UI::updateFiltersData(const VecLogRow & appendedLog)
{
  for (const auto & row : appendedLog)
  {
    _objectNamesFilters.insert(std::make_pair(row._filterData->_objectName, true));

    auto levelStr = row._filterData->_level;
    boost::to_upper(levelStr);
    _levelFilters.insert(std::make_pair(levelStr, true));

    if (row._filterData->_objectId.empty() == false)
      _objectIdsFilters.insert(std::make_pair(row._filterData->_objectId, true));
  }
}
//-----------------------------------------------
void UI::onFilterChanged()
{
  auto shouldBeHided = [&](const auto & entry)
  {
    auto it = _objectNamesFilters.find(entry._filterData->_objectName);
    if (it != _objectNamesFilters.end())
    {
      if (it->second == false) return true;
    }

    it = _levelFilters.find(entry._filterData->_level);
    if (it != _levelFilters.end())
    {
      if (it->second == false) return true;
    }

    it = _objectIdsFilters.find(entry._filterData->_objectId);
    if (it != _objectIdsFilters.end())
    {
      if (it->second == false) return true;
    }

    return false;
  };
  
  _logList->DetachAllChildren();
  for (const auto & row : _logData)
  {
    if (shouldBeHided(row)) 
      continue;
    _logList->Add(Make<TableRowComponent>(row));
  }
  _screen.RequestAnimationFrame();
}
//-----------------------------------------------

#include "UI.h"
//-----------------------------------------------
UI::UI() :
  _screen(ScreenInteractive::Fullscreen()),
  _objectNamesContainer(Container::Vertical({})),
  _levelFilterContainer(Container::Vertical({})),
  _objectIdFilterContainer(Container::Vertical({})),
  _logList(Container::Vertical({}))
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
        _objectIdFilterContainer->Add(Checkbox(objectId, &value, filtersCheckboxesOptions));
    }
  };
  auto objectIdFilterInput = Input(&objectIdFilterValue, inputOptions);

  auto configMenu = Container::Horizontal(
  {
    _levelFilterContainer,
    _objectNamesContainer,
    Container::Vertical({objectIdFilterInput, _objectIdFilterContainer}),
  });

  int activeLayer = 0;
  _levelFilterContainer->TakeFocus();
  _objectNamesContainer->TakeFocus();
  _objectIdFilterContainer->TakeFocus();

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
            vbox({text("Levels"), separator(), _levelFilterContainer->Render()}) | border,
            vbox({text("Objects"), separator(), _objectNamesContainer->Render() | yframe | vscroll_indicator}) | border,
            vbox({hbox({text("Objects IDs"), separator(), objectIdFilterInput->Render()}), separator(), _objectIdFilterContainer->Render() | yframe | vscroll_indicator}) | border
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
//-----------------------------------------------
void UI::appendLog(VecLogRow log)
{
  _screen.Post([&, log]()
  {
    auto isLastElementFocused = _logList->ChildCount() > 0 ? _logList->ChildAt(_logList->ChildCount() - 1)->Focused() 
                                                           : false;

    for (const auto & row : log)
    {
      _objectNamesFilters.insert(std::make_pair(row._objectName, true));

      auto levelStr = row._level;
      boost::to_upper(levelStr);
      _levelFilters.insert(std::make_pair(levelStr, true));

      _objectIdsFilters.insert(std::make_pair(row._objectId, true));

      _logList->Add(Make<TableRowComponent>(row));
    }

    if (isLastElementFocused)
    {
      _logList->ChildAt(_logList->ChildCount() - 1)->TakeFocus();
    }

    _logData.insert(_logData.end(), log.begin(), log.end());

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
      _objectIdFilterContainer->Add(Checkbox(objectId, &value, filtersCheckboxesOptions));
    }
  });
  _screen.RequestAnimationFrame();
}
//-----------------------------------------------
void UI::onFilterChanged()
{
  auto shouldBeHided = [&](const auto & entry)
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
  };
  
  _logList->DetachAllChildren();
  for (const auto & row : _logData)
  {
    if (shouldBeHided(row)) 
      continue;
    _logList->Add(Make<TableRowComponent>(row));
  }
}
//-----------------------------------------------

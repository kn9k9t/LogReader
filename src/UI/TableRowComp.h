#pragma once
//-----------------------------------------------
#include <map>
#include "ftxui/ftxui.hpp"
#include <boost/algorithm/string.hpp>
#include "../LogHandler/LogTypes.h"
//-----------------------------------------------
using namespace ftxui;
//-----------------------------------------------
class TableRowComponent : public ComponentBase {
public:
    TableRowComponent(const LogRow & row);

    Element OnRender() override;
    bool Focusable() const override;
    const LogRow & getLogRow() const;
private:
    LogRow _row;
};
//-----------------------------------------------
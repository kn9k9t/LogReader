#include "TableRowComp.h"
//-----------------------------------------------
TableRowComponent::TableRowComponent(const LogRow & row) : 
  _row(row) 
{}
//-----------------------------------------------
Element TableRowComponent::OnRender() 
{
  auto colorType = Color();
  if (boost::iequals(_row._level, "ERR"))
    colorType = Color::Red;
  if (boost::iequals(_row._level, "WRN"))
    colorType = Color::Yellow;
  if (boost::iequals(_row._level, "VRB"))
    colorType = Color::Blue;

  Elements mainData
  {
    text(" "), text(_row._readTime)    | color(colorType) | size(WIDTH, EQUAL, 19), text(" "), separator(),
    text(" "), text(_row._level)       | color(colorType), text(" "), separator(),
    text(" "), text(_row._objectName)  | color(colorType) | size(WIDTH, EQUAL, 10), text(" "), separator(),
    text(" "), text(_row._objectId)    | color(colorType) | size(WIDTH, EQUAL, 10), text(" "), separator(),
    text(" "), paragraph(_row._msg.front()) | color(colorType)
  };
  Elements extraRows;
  for (size_t i = 1; i < _row._msg.size(); ++i)
  {
    extraRows.push_back(text(_row._msg[i]));
  }

  auto element = vbox({
    hbox({mainData}),
    vbox({extraRows})
  });

  if (Focused()) 
  {
    element = focus(element);
  }

  return element;
}
//-----------------------------------------------
bool TableRowComponent::Focusable() const 
{
  return true;
}
//-----------------------------------------------
const LogRow & TableRowComponent::getLogRow() const
{
  return _row;
}
//-----------------------------------------------

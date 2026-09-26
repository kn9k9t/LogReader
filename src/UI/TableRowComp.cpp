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

  auto element = text(_row._rawMsg);

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

#include "TableRowComp.h"
//-----------------------------------------------
TableRowComponent::TableRowComponent(const LogRow & row) : 
  _row(row) 
{}
//-----------------------------------------------
Element TableRowComponent::OnRender() 
{
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

#include "ScrollableContainer.h"
//-----------------------------------------------
namespace ftxui
{
namespace Container
{
Component Scrollable(Components children)
{
  return std::make_shared<ScrollableContainer>(std::move(children));
} 
}
}
//-----------------------------------------------
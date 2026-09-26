#pragma once
//-----------------------------------------------
#include "ftxui/ftxui.hpp"
//-----------------------------------------------
namespace ftxui {
class ScrollableContainer : public ComponentBase {
 public:
  ScrollableContainer(Components children) :
    content_begin_(0),
    content_end_(1)
  {
    for (Component & child : children) 
    {
      Add(std::move(child));
    }
    content_end_ = ftxui::Terminal::Size().dimy;
  }

  virtual Element OnRender() override
  {
    Elements elements;
    int reserveCount = GetComponentSize() + 1;
    if (reserveCount <= 0)
      reserveCount = 1;

    elements.reserve(reserveCount);
    for (size_t i = content_begin_; i < (size_t)content_end_ && i < children().size(); ++i) 
    {
      elements.push_back(children()[i]->Render());
    }
    if (elements.empty()) {
      return text("Empty container") | reflect(box_);
    }
    return vbox(std::move(elements)) | reflect(box_);
  }

  virtual bool OnEvent(Event event) override
  {
    if (event == Event::ArrowUp || event == Event::Character('k'))
    {
      UpdateScroll(-1);
      return true;
    }
    if (event == Event::ArrowDown || event == Event::Character('j'))
    {
      UpdateScroll(+1);
      return true;
    }
    if (event == Event::PageUp) {
      for (int i = 0; i < box_.y_max - box_.y_min; ++i) {
        UpdateScroll(-1);
      }
    }
    if (event == Event::PageDown) {
      for (int i = 0; i < box_.y_max - box_.y_min; ++i) {
        UpdateScroll(+1);
      }
    }
    return false;
  }

 private:
  void UpdateScroll(int dir)
  {
    content_begin_ += dir;
    content_end_ += dir;

    if (content_begin_ < 0)
    {
      content_begin_ = 0;
      content_end_ = GetComponentSize() + 1;
    }

    if ((size_t)content_end_ >= children().size()) 
    {
      content_end_ = children().size();
      content_begin_ = content_end_ - GetComponentSize() - 1;
    }
  }

  int GetComponentSize() const
  {
    return box_.y_max - box_.y_min;
  }

  // int container_height_ = 30;
  int content_begin_ = 0;
  int content_end_ = 0;

  Box box_;
};
namespace Container
{
Component Scrollable(Components children);
} //namespace Container
} // namespace ftxui
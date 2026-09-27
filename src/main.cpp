#include "LogReader.h"
#include "UI/ScrollableContainer.h"
//-----------------------------------------------
// int main(int, char **)
// {
//   auto app(ScreenInteractive::Fullscreen());
//   auto scrollableContainer = std::make_shared<ScrollableContainer>(Components{});

//   for (int i = 0; i < 100; ++i)
//   {
//     LogRow logRow;
//     logRow._readTime = "2023-06-01 12:00:00." + std::to_string(i);
//     logRow._level = "INFO";
//     logRow._objectName = "Object" + std::to_string(i);
//     logRow._objectId = "ID" + std::to_string(i);
//     logRow._msg = {"Some dummy message " + std::to_string(i)};
//     scrollableContainer->Add(std::make_shared<TableRowComponent>(logRow));
//   }

//   auto renderer = Renderer(scrollableContainer, [&]()
//   {
//     return scrollableContainer->Render() | yflex | yframe;
//   });

//   app.Loop(renderer);
//   return 0;
// }
//-----------------------------------------------
int main(int argc, char ** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: logreader <path>" << std::endl;
    return 1;
  }

  LogReaderApp app(argv[1]);
  app.run();

  return 0;
}
//-----------------------------------------------
#include "LogReader.h"
#include "UI/ScrollableContainer.h"
//-----------------------------------------------
int main(int argc, char ** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: logreader <path>" << std::endl;
    return 1;
  }

  std::cout << "agrv[0] : " << argv[0] << std::endl;
  std::cout << "agrv[1] : " << argv[1] << std::endl; 

  // LogReaderApp app(argv[1]);
  // app.run();

  return 0;
}
//-----------------------------------------------
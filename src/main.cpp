#include "LogReader.h"
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
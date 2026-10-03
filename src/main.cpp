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

  std::filesystem::path fullPath;
  try
  {
    fullPath = std::filesystem::canonical(argv[1]);
  }
  catch (const std::exception& ex)
  {
    std::cout << "Canonical path for " << argv[1] << " cannot be resolved:\n"
              << ex.what() << std::endl;
    exit(1);
  }

  LogReaderApp app(fullPath.generic_string());
  app.run();

  return 0;
}
//-----------------------------------------------
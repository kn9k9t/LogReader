#include "FileReader.h"
#include "UI.h"
//-----------------------------------------------
int main(int argc, char ** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: logreader <path>" << std::endl;
    return 1;
  }

  LogHandler logHandler(argv[1]);
  if (logHandler.readLog() == false)
  {
    std::cout << "Erorr log reading" << std::endl;
    return 1;
  }

  auto entries = logHandler.getLog();

  UI ui;
  ui.init(entries);
  return 0;
}
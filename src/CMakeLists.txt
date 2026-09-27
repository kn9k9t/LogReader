add_library(log_reader_lib
  StringViewUtils/StringViewUtils.h
  StringViewUtils/StringViewUtils.cpp

  LogHandler/LogTypes.h
  LogHandler/LogParser.h
  LogHandler/LogParser.cpp
  LogHandler/LogHandler.cpp
  LogHandler/LogHandler.h

  UI/ScrollableContainer.cpp
  UI/ScrollableContainer.h
  UI/TableRowComp.cpp
  UI/TableRowComp.h
  UI/UI.cpp
  UI/UI.h

  LogReader.h
  LogReader.cpp
)
target_link_libraries(log_reader_lib
  PRIVATE ftxui::ftxui
)
include_directories(log_reader_lib PRIVATE StringViewUtils)

add_executable(logreader main.cpp)
target_link_libraries(logreader
  PRIVATE ftxui::ftxui
  PRIVATE ${Boost_LIBRARIES}
  PRIVATE log_reader_lib
)

if (MSVC)
  target_compile_options(logreader PRIVATE
    /W4
  )
  target_compile_options(log_reader_lib PRIVATE
    /W4
  )
else ()
  target_compile_options(logreader PRIVATE
    -Wall -Wextra -pedantic
  )
  target_compile_options(log_reader_lib PRIVATE
    -Wall -Wextra -pedantic
  )
endif()

install(TARGETS logreader RUNTIME DESTINATION bin)

# enable_testing()

# add_test(tests tests)
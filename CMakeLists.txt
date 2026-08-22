cmake_minimum_required(VERSION 3.20)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(log_reader VERSION ${PROJECT_VERSION})



add_executable(logreader src/main.cpp)
add_library(log_reader_lib
  src/LogTypes.h
  src/FileReader.h
  src/UI.h
)

set_property(TARGET logreader PROPERTY CXX_STANDARD 23)

set_target_properties(log_reader_lib PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(log_reader_lib PRIVATE src)

include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v7.0.3
)
FetchContent_MakeAvailable(ftxui)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)

target_link_libraries(logreader 
  PRIVATE log_reader_lib
  PRIVATE ftxui::ftxui
  PRIVATE ${Boost_LIBRARIES}
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
# -Werror
install(TARGETS logreader RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT nikpavl04@gmail.com)

include(CPack)

# enable_testing()

# add_test(tests tests)
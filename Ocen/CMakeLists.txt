# CMakeList.txt: Ocen 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("Ocen")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY E:/Vincent/Language_C++/Code_At_VS/Ocen/bin)
set(BOOST_ROOT "E:/Vincent/Language_C++/Code_At_VS/Library/boost_library")

# 将源代码添加到此项目的可执行文件。
add_executable (Ocen "Ocen.cpp" "Ocen.h"  "Robot/lex/Lexical.cpp" "Robot/lex/Lexical.h" "Robot/parser/Parser.h" "Robot/parser/Parser.cpp" "Server/Server.cpp" "Server/Server.h" "Robot/interpreter/Interpreter.cpp" "Robot/interpreter/Interpreter.h" "Server/UserData.cpp" "Server/UserData.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Ocen PROPERTY CXX_STANDARD 20)
endif()

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(Ocen ${Boost_LIBRARIES})
endif()



# TODO: 如有需要，请添加测试并安装目标。

cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
option(BUILD_COVERAGE "Build coverage" OFF)

project(Go_game)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

#add_library(${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml-system
#            ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml-window
#            ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml-graphics
#            ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml-network
#)

add_library(Go_game STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/Button.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/MainMenu.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/Table.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/TableStone.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/for_mainmenu.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/math_functions.cpp
)

add_executable(start_game
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/source.cpp
)

target_include_directories(Go_game
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(start_game
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# target_include_directories(Go_game
#   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SFML-include
# )
# target_include_directories(start_game
#   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SFML-include
# )

target_link_libraries(start_game sfml-network
                                 sfml-graphics
                                 sfml-window
                                 sfml-system
                                 Go_game
)

# target_link_libraries (start_game PUBLIC Go_game)
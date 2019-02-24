cmake_minimum_required(VERSION 3.13)
project(sudok_no)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 17)

#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")
#
include(FindPkgConfig)
find_package(SFML 2 REQUIRED network audio graphics window system)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

#add_compile_options(-Wall -Wextra -pedantic -Wshadow -Weffc++ -Werror)
add_compile_options(-Wall -Wextra -pedantic -Wshadow -Werror)

add_executable(sudok_no main.cpp algorithm.cpp types.h puzzle.cpp puzzle.h algorithm.h)
target_link_libraries(sudok_no ${SFML_LIBRARIES})
target_link_libraries(sudok_no ${CONAN_LIBS})
target_link_libraries(sudok_no pthread)

file(COPY fonts DESTINATION "${CMAKE_BINARY_DIR}")
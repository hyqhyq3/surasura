project(surasura)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(Boost COMPONENTS system context)



add_subdirectory(examples)
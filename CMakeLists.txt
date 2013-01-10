project(counter)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(Boost COMPONENTS system context)

add_executable(counter main.cpp)
target_link_libraries(counter ${Boost_LIBRARIES} "-lpthread")

install(TARGETS counter RUNTIME DESTINATION bin)

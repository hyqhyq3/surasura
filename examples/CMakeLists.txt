
add_executable(test main.cpp)

target_link_libraries(test ${Boost_LIBRARIES} "-lpthread")
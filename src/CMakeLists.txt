add_subdirectory(Core)

add_subdirectory(Timer)

add_executable(test test.cpp)
target_link_libraries(test PUBLIC clock timer_manager)
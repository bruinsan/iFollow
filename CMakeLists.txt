CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

PROJECT(
	alarm_priority	
	VERSION 3.0
	LANGUAGES C CXX
	)

include_directories(.)

add_subdirectory(alarm)
add_subdirectory(tests)

add_executable(alarm_sample main.cpp)
target_link_libraries(alarm_sample alarm)



CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

PROJECT(
	alarm_priority	
	VERSION 3.0
	LANGUAGES C CXX
	)

FIND_PACKAGE ( Threads REQUIRED )
ADD_EXECUTABLE ( alarm main.cpp alarm.cpp alarmManager.cpp)
TARGET_LINK_LIBRARIES ( alarm ${CMAKE_THREAD_LIBS_INIT} )

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(unittest tests.cpp alarm.cpp alarmManager.cpp)
target_link_libraries(unittest ${GTEST_LIBRARIES} pthread)



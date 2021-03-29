CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

PROJECT(
	alarm_priority	
	VERSION 1.0
	LANGUAGES C CXX
	)

FIND_PACKAGE ( Threads REQUIRED )
ADD_EXECUTABLE ( alarm main.cpp )
TARGET_LINK_LIBRARIES ( alarm ${CMAKE_THREAD_LIBS_INIT} )

# add_executable(alarm main.cpp)



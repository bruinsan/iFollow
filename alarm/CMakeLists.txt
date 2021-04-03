set(alarm_source_files
    alarm.cpp
    alarm.hpp
    alarmManager.cpp
    alarmManager.hpp
)

add_library(alarm ${alarm_source_files})

FIND_PACKAGE ( Threads REQUIRED )
target_link_libraries(alarm ${CMAKE_THREAD_LIBS_INIT})
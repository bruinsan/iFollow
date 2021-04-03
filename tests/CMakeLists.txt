find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_alarm tests.cpp )
target_link_libraries(test_alarm alarm ${GTEST_LIBRARIES})



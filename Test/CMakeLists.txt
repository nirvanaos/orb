add_executable(test-orb
	TestORB.cpp
)
target_link_libraries(test-orb PRIVATE
	googletest-nirvana
	nirvana
	core-mock
)

#include(GoogleTest)
#gtest_discover_tests(test-orb WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test-orb
	TestORB.cpp
)
target_link_libraries(test-orb PRIVATE
	googletest-nirvana
	nirvana
	mockimport
	crtl
)

add_custom_command(TARGET test-orb POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:test-orb> $<TARGET_FILE_DIR:test-orb>
  COMMAND_EXPAND_LISTS
)

#include(GoogleTest)
#gtest_discover_tests(test-orb WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

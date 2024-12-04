add_executable(test-orb
	TestORB.cpp
)
target_link_libraries(test-orb PRIVATE
	googletest-nirvana
	nirvana
	core-mock
)

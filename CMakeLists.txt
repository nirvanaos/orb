add_subdirectory(Include/CORBA)
add_subdirectory(Source)

if(BUILD_TESTING)
	add_subdirectory(Test)
endif()

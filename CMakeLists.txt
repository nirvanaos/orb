add_subdirectory (Include/CORBA)

if (NIRVANA_BUILD)

add_subdirectory (Source)

if (BUILD_TESTING)
	add_subdirectory (Test)
endif ()

endif ()

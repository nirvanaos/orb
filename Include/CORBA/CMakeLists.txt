set(idl_files
	Components.idl
	Context.idl
	CORBA_Stream.idl
	CORBA_types.idl
	CosEventChannelAdmin.idl
	CosEventComm.idl
	CosLwLogAdministrator.idl
	CosLwLogConsumer.idl
	CosLwLogService.idl
	CosLwLogStatus.idl
	CosNaming.idl
	CosTime.idl
	CosTimerEvent.idl
	CosTypedEventChannelAdmin.idl
	CosTypedEventComm.idl
	Current.idl
	DomainManager.idl
	Dynamic.idl
	DynamicServant.idl
	InterfaceRepository.idl
	Messaging.idl
	NirvanaDGC.idl
	NVList.idl
	ObjectFactory.idl
	ORB.idl
	Policy.idl
	Pollable.idl
	PortableServer.idl
	RefCnt.idl
	Request.idl
	SFloat.idl
	TCKind.idl
	TimeBase.idl
	TypeCode.idl

	Proxy/IOReference.idl
	Proxy/ProxyFactory.idl

	ccm/CCM_Base.idl
	ccm/CCM_Cookie.idl
	ccm/CCM_Events.idl
	ccm/CCM_Exceptions.idl
	ccm/CCM_Home.idl
	ccm/CCM_Navigation.idl
	ccm/CCM_Object.idl
	ccm/CCM_Receptacle.idl
)

foreach(f IN LISTS idl_files)
	list(APPEND idl_idl ${CMAKE_CURRENT_SOURCE_DIR}/${f})
	cmake_path(REMOVE_EXTENSION f OUTPUT_VARIABLE f_no_ext)
	list(APPEND idl_h ${CMAKE_CURRENT_SOURCE_DIR}/${f_no_ext}.h)
	list(APPEND idl_h ${CMAKE_CURRENT_SOURCE_DIR}/${f_no_ext}_s.h)
	cmake_path(GET f STEM name)
	list(APPEND idl_cpp ${CMAKE_CURRENT_BINARY_DIR}/${name}.cpp)
endforeach()

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/AMI.h ${CMAKE_CURRENT_SOURCE_DIR}/AMI_s.h ${CMAKE_CURRENT_BINARY_DIR}/AMI.cpp
	COMMAND ${NIDL2CPP} ARGS ${IDL_OPTIONS} ${CMAKE_CURRENT_SOURCE_DIR}/AMI.idl -legacy -no_ami -no_proxy -out_cpp ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/AMI.idl
	COMMENT "Compile IDL..."
	VERBATIM
)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/IORequest.h ${CMAKE_CURRENT_SOURCE_DIR}/IORequest_s.h ${CMAKE_CURRENT_BINARY_DIR}/IORequest.cpp
	COMMAND ${NIDL2CPP} ARGS ${IDL_OPTIONS} ${CMAKE_CURRENT_SOURCE_DIR}/IORequest.idl -legacy -no_ami -no_proxy -out_cpp ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/IORequest.idl
	COMMENT "Compile IDL..."
	VERBATIM
)

add_custom_command(
	OUTPUT ${idl_h} ${idl_cpp}
	COMMAND ${NIDL2CPP} ARGS ${IDL_OPTIONS} ${idl_idl} -legacy -no_proxy -out_cpp ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS ${idl_idl}
	COMMENT "Compile IDL..."
	VERBATIM
)

add_custom_target(orb_generated DEPENDS ${idl_cpp} ${CMAKE_CURRENT_BINARY_DIR}/AMI.cpp ${CMAKE_CURRENT_BINARY_DIR}/IORequest.cpp)
add_dependencies(nirvana orb_generated)

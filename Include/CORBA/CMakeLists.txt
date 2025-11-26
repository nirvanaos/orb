set (idl_files
	Components.idl
	Context.idl
	CORBA_Stream.idl
	CORBA_types.idl
	CosEventChannelAdmin.idl
	CosEventComm.idl
	CosLwLogAdministrator.idl
	CosLwLogConsumer.idl
	CosLwLogProducer.idl
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
	FT.idl
	InterfaceRepository.idl
	Messaging.idl
	NirvanaDGC.idl
	NVList.idl
	ObjectFactory.idl
	ORB.idl
	Policy.idl
	Pollable.idl
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

set (idl_files_no_ami
	AMI.idl
	IORequest.idl
	PortableServer.idl
)

idl_generate (OUT_VAR src_no_ami IDL_FILES ${idl_files_no_ami} OPTIONS -no_ami)
idl_generate (OUT_VAR src IDL_FILES ${idl_files} OPTIONS -DCCM_LW -DCCM_NOEVENT)
add_custom_target (corba_idl DEPENDS ${src} ${src_no_ami})
target_sources (Nirvana PRIVATE ${src} ${src_no_ami})

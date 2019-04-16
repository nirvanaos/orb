#include "I1.h"
#include "I3.h"
#include <CORBA/OLF.h>

#ifndef TEST_NO_STATIC

using namespace CORBA;
using namespace CORBA::Nirvana;

#pragma section (OLF_BIND, read, execute)

extern "C" __declspec (allocate(OLF_BIND))
static const struct
{
	OLF::SectionHeader hdr;
	struct
	{
		struct
		{
			OLF::SectionHeader hdr;
			OLF::ObjectExport data [2];
		} s0;

		struct
		{
			OLF::SectionHeader hdr;
			OLF::ObjectLink data [2];
		} s1;

		struct
		{
			OLF::SectionHeader hdr;
#ifndef TEST_LOCAL_OBJECT
			OLF::StaticObject data [2];
#else
			OLF::StaticLocal data [2];
#endif
		} s2;

	} data;

} OLF_data = {

	{OLF::SectionType::OLF, sizeof (OLF_data.data)},
	{
		{
			{OLF::SectionType::OBJECT_EXPORT, sizeof (OLF_data.data.s0.data)},
			{
				{"TestORB::StaticI1", STATIC_BRIDGE (TestORB::StaticI1, AbstractBase)},
				{"TestORB::StaticI3", STATIC_BRIDGE (TestORB::StaticI3, AbstractBase)}
			}
		},
		{
			{OLF::SectionType::OBJECT_LINK, sizeof (OLF_data.data.s1.data)},
			{{0}, {0}}
		},
		{
#ifndef TEST_LOCAL_OBJECT
			{OLF::SectionType::STATIC_OBJECT, sizeof (OLF_data.data.s2.data)},
			{
				{STATIC_BRIDGE (TestORB::StaticI1, PortableServer::ServantBase)},
				{STATIC_BRIDGE (TestORB::StaticI3, PortableServer::ServantBase)}
			}
#else
			{OLF::SectionType::STATIC_LOCAL, sizeof (OLF_data.data.s2.data)},
			{
				{STATIC_BRIDGE (TestORB::StaticI1, AbstractBase)},
				{STATIC_BRIDGE (TestORB::StaticI3, AbstractBase)}
			}
#endif
		}
	}
};

#ifndef TEST_LOCAL_OBJECT

template <>
const PortableServer::Servant& InterfaceStatic <TestORB::StaticI1, PortableServer::ServantBase>::servant_base_ = 
reinterpret_cast <const PortableServer::Servant&> (OLF_data.data.s1.data [0].interface_ptr);

template <>
const PortableServer::Servant& InterfaceStatic <TestORB::StaticI3, PortableServer::ServantBase>::servant_base_ = 
reinterpret_cast <const PortableServer::Servant&> (OLF_data.data.s1.data [1].interface_ptr);

#else

template <>
const Object_ptr& InterfaceStatic <TestORB::StaticI1, LocalObject>::object_ =
reinterpret_cast <const Object_ptr&> (OLF_data.data.s1.data [0].interface_ptr);

template <>
const Object_ptr& InterfaceStatic <TestORB::StaticI3, LocalObject>::object_ =
reinterpret_cast <const Object_ptr&> (OLF_data.data.s1.data [1].interface_ptr);

#endif

#endif

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
			OLF::ObjectLink data [2];
		} links;

		struct
		{
			OLF::SectionHeader hdr;
			OLF::ExportInterface data [2];
		} exports;

	} data;

} OLF_data = {

	{OLF::SectionType::OLF, sizeof (OLF_data.data)},
	{
		{
			{OLF::SectionType::OBJECT_LINK, sizeof (OLF_data.data.links.data)},
			{{0}, {0}}
		},
		{
#ifndef TEST_LOCAL_OBJECT
			{OLF::SectionType::EXPORT_OBJECT, sizeof (OLF_data.data.exports.data)},
			{
				{"TestORB::StaticI1", STATIC_BRIDGE (TestORB::StaticI1, PortableServer::ServantBase)},
				{"TestORB::StaticI3", STATIC_BRIDGE (TestORB::StaticI3, PortableServer::ServantBase)}
			}
#else
			{OLF::SectionType::EXPORT_LOCAL, sizeof (OLF_data.data.exports.data)},
			{
				{"TestORB::StaticI1", STATIC_BRIDGE (TestORB::StaticI1, AbstractBase)},
				{"TestORB::StaticI3", STATIC_BRIDGE (TestORB::StaticI3, AbstractBase)}
			}
#endif
		}
	}
};

#ifndef TEST_LOCAL_OBJECT

template <>
const PortableServer::Servant& InterfaceStatic <TestORB::StaticI1, PortableServer::ServantBase>::servant_base_ = 
reinterpret_cast <const PortableServer::Servant&> (OLF_data.data.links.data [0].interface_ptr);

template <>
const PortableServer::Servant& InterfaceStatic <TestORB::StaticI3, PortableServer::ServantBase>::servant_base_ = 
reinterpret_cast <const PortableServer::Servant&> (OLF_data.data.links.data [1].interface_ptr);

#else

template <>
const Object_ptr& InterfaceStatic <TestORB::StaticI1, LocalObject>::object_ =
reinterpret_cast <const Object_ptr&> (OLF_data.data.links.data [0].interface_ptr);

template <>
const Object_ptr& InterfaceStatic <TestORB::StaticI3, LocalObject>::object_ =
reinterpret_cast <const Object_ptr&> (OLF_data.data.links.data [1].interface_ptr);

#endif

#endif
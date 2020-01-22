#ifndef NIRVANA_ORB_OLF_H_
#define NIRVANA_ORB_OLF_H_

#include <CORBA/ServantBase_c.h>
#include <CORBA/LocalObject_c.h>

#define OLF_BIND "olfbind"

namespace CORBA {
namespace Nirvana {
namespace OLF {

struct SectionHeader
{
	ULong type; //!< Section type
	ULong size;	//!< Size of the section data (not including this header).
};

enum Command
{
	OLF_IMPORT_INTERFACE = 1,
	OLF_EXPORT_INTERFACE = 2,
	OLF_EXPORT_OBJECT = 3,
	OLF_EXPORT_LOCAL = 4,
};

struct ExportInterface
{
	::Nirvana::Word command;
	const Char* name;
	Bridge <Interface>* itf;
};

struct ExportObject
{
	::Nirvana::Word command;
	const Char* name;
	BridgeMarshal <PortableServer::ServantBase>* implementation;
	Bridge <PortableServer::ServantBase>* core_object;
};

struct ExportLocal
{
	::Nirvana::Word command;
	const Char* name;
	Bridge <AbstractBase>* implementation;
	Bridge <LocalObject>* core_object;
};

struct ImportInterface
{
	::Nirvana::Word command;
	const Char* name;
	const Char* interface_id;
	Bridge <Interface>* itf;
};

}
}
}

#endif

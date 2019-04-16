#ifndef NIRVANA_ORB_OLF_H_
#define NIRVANA_ORB_OLF_H_

#include <CORBA/ServantBase_c.h>

#define OLF_BIND ".olfbind"

namespace CORBA {
namespace Nirvana {
namespace OLF {

struct SectionHeader
{
	ULong type;
	ULong size;
};

enum SectionType
{
	OLF = 'FLO', // "OLF\0"
	OBJECT_EXPORT = 1,	// Must be first section in the OLF section.
	OBJECT_LINK = 2,
	SYNC_DOMAIN = 3,
	STATIC_OBJECT = 4,
	STATIC_LOCAL = 5,
	OBJECT_IMPORT = 6
};

struct ObjectExport
{
	const Char* name;	// Names must be sorted in the OLF_OBJECT_EXPORT section.
	Bridge <Interface>* itf;
};

struct ObjectLink
{
	Bridge <Interface>* interface_ptr;
};

struct StaticObject
{
	Bridge <PortableServer::ServantBase>* servant;
};

struct StaticLocal
{
	Bridge <AbstractBase>* servant;
};

struct ObjectImport
{
	const Char* name;
	const Char* interface_id;
};

}
}
}

#endif

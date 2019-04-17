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
	OBJECT_LINK = 1,
	SYNC_DOMAIN = 2,
	EXPORT_INTERFACE = 3,
	EXPORT_OBJECT = 4,
	EXPORT_LOCAL = 5,
	IMPORT_INTERFACE = 6,
};

struct ExportInterface
{
	const Char* name;
	Bridge <Interface>* itf;
};

struct ObjectLink
{
	Bridge <Interface>* interface_ptr;
};

struct ImportInterface
{
	const Char* name;
	const Char* interface_id;
};

}
}
}

#endif

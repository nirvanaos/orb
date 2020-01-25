#ifndef NIRVANA_ORB_OLF_H_
#define NIRVANA_ORB_OLF_H_

#include <Nirvana/NirvanaBase.h>
#include "ServantBase_c.h"
#include "LocalObject_c.h"

#define OLF_BIND "olfbind"

#pragma section (OLF_BIND, read, execute)

namespace CORBA {
namespace Nirvana {

enum OLF_Command
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
	BridgeMarshal <AbstractBase>* implementation;
	Bridge <LocalObject>* core_object;
};

struct ImportInterface
{
	::Nirvana::Word command;
	const Char* name;
	const Char* interface_id;
	Bridge <Interface>* itf;
};

template <class I>
struct ImportInterfaceT
{
	ImportInterface imp;

	operator T_ptr <I> () const
	{
		return static_cast <Bridge <I>*> (imp.itf);
	}

	I* operator -> () const
	{
		return static_cast <I*> (imp.itf);
	}
};

}
}

#endif

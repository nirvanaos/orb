#ifndef NIRVANA_ORB_INTERFACE_S_H_
#define NIRVANA_ORB_INTERFACE_S_H_

#include "AbstractBase_c.h"
#include "RepositoryId.h"

namespace CORBA {
namespace Nirvana {

extern void _check_pointer (const void* p);
extern void _check_pointer (const Bridge <Interface>* obj, const Bridge <Interface>::EPV& epv);

template <class S, class I> class Skeleton;

struct InterfaceEntry
{
	const Char* interface_id;
	Bridge <Interface>* (*cast) (void* servant);

	static Bridge <Interface>* find (const InterfaceEntry* begin, const InterfaceEntry* end, void* servant, const Char* id);
};

template <class S, class ... I>
class InterfaceFinder
{
	template <class Itf>
	static Bridge <Interface>* cast (void* servant)
	{
		return &static_cast <Bridge <Itf>&> (*reinterpret_cast <S*> (servant));
	}

public:
	static Bridge <Interface>* find (S& servant, const Char* id)
	{
		static const InterfaceEntry table [] = {
			{ Bridge <I>::interface_id_, cast <I> }...,
		};

		return InterfaceEntry::find (table, table + sizeof (table) / sizeof (*table), &servant, id);
	}
};

template <class ... I>
class FindInterface
{
public:
	template <class S>
	static Bridge <Interface>* find (S& servant, const Char* id)
	{
		return InterfaceFinder <S, I...>::find (servant, id);
	}
};

}
}

#endif

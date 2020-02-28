#ifndef NIRVANA_ORB_FINDINTERFACE_H_
#define NIRVANA_ORB_FINDINTERFACE_H_

#include <Nirvana/Nirvana.h>
#include "ServantBase.h"
#include "LocalObject.h"

namespace CORBA {
namespace Nirvana {

struct InterfaceEntry
{
	const Char* interface_id;
	size_t id_len;
	Interface* (*cast) (void* servant);

	static Interface* find (const InterfaceEntry* begin, const InterfaceEntry* end, void* servant, const String& id);
};

template <class S, class Primary, class ... I>
class InterfaceFinder
{
	template <class Itf>
	static Interface* cast (void* servant)
	{
		return &static_cast <Bridge <Itf>&> (*reinterpret_cast <S*> (servant));
	}

	template <>
	static Interface* cast <::PortableServer::ServantBase> (void* servant)
	{
		return &static_cast <Bridge <Object>&> (*reinterpret_cast <S*> (servant));
	}

	template <>
	static Interface* cast <LocalObject> (void* servant)
	{
		return &static_cast <Bridge <Object>&> (*reinterpret_cast <S*> (servant));
	}

public:
	static Interface_ptr find (S& servant, const String& id)
	{
		return InterfaceEntry::find (itable_, itable_ + countof (itable_), &servant, id);
	}

private:
	static const InterfaceEntry itable_ [];
};

template <class S, class Primary, class ... I>
const InterfaceEntry InterfaceFinder <S, Primary, I...>::itable_ [] = {
	{ Bridge <Primary>::interface_id_, countof (Bridge <Primary>::interface_id_), cast <Primary> },
	{ Bridge <I>::interface_id_, countof (Bridge <I>::interface_id_), cast <I> }...
};

template <class Primary, class ... I>
class FindInterface
{
public:
	template <class S>
	static Interface_ptr find (S& servant, const String& id)
	{
		return InterfaceFinder <S, Primary, I...>::find (servant, id);
	}
};

}
}

#endif

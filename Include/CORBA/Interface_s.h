#ifndef NIRVANA_ORB_INTERFACE_S_H_
#define NIRVANA_ORB_INTERFACE_S_H_

#include <Nirvana/NirvanaBase.h>
#include "Object.h"
#include "RepositoryId.h"
#include "ServantImpl.h"
#include "Exception.h"

namespace PortableServer {
class ServantBase;
}

namespace CORBA {

class LocalObject;

namespace Nirvana {

struct InterfaceEntry
{
	const Char* interface_id;
	Bridge <Interface>* (*cast) (void* servant);

	static Bridge <Interface>* find (const InterfaceEntry* begin, const InterfaceEntry* end, void* servant, const Char* id);
};

template <class S, class Primary, class ... I>
class InterfaceFinder
{
	template <class Itf>
	static Bridge <Interface>* cast (void* servant)
	{
		return &static_cast <Bridge <Itf>&> (*reinterpret_cast <S*> (servant));
	}

	template <class Itf>
	struct InterfaceId
	{
		static constexpr const Char* id ()
		{
			return Bridge <Itf>::interface_id_;
		}
	};

	template <>
	static Bridge <Interface>* cast <PortableServer::ServantBase> (void* servant)
	{
		return &static_cast <Bridge <Object>&> (*reinterpret_cast <S*> (servant));
	}

	template <>
	struct InterfaceId <PortableServer::ServantBase>
	{
		static constexpr const Char* id ()
		{
			return Bridge <Object>::interface_id_;
		}
	};

	template <>
	static Bridge <Interface>* cast <LocalObject> (void* servant)
	{
		return &static_cast <Bridge <Object>&> (*reinterpret_cast <S*> (servant));
	}

	template <>
	struct InterfaceId <LocalObject>
	{
		static constexpr const Char* id ()
		{
			return Bridge <Object>::interface_id_;
		}
	};

public:
	static Interface_ptr find (S& servant, const Char* id)
	{
		return Interface::unmarshal (InterfaceEntry::find (itable_, itable_ + countof (itable_), &servant, id));
	}

private:
	static const InterfaceEntry itable_ [];
};

template <class S, class Primary, class ... I>
const InterfaceEntry InterfaceFinder <S, Primary, I...>::itable_ [] = {
	{ InterfaceId <Primary>::id (), cast <Primary> },
	{ InterfaceId <I>::id (), cast <I> }...
};

template <class Primary, class ... I>
class FindInterface
{
public:
	template <class S>
	static Interface_ptr find (S& servant, const Char* id)
	{
		return InterfaceFinder <S, Primary, I...>::find (servant, id);
	}
};

}
}

#endif

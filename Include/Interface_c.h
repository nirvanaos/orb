// Nirvana project
// Object Request Broker
#ifndef NIRVANA_ORB_INTERFACE_C_H_
#define NIRVANA_ORB_INTERFACE_C_H_

#include <Nirvana/Nirvana.h>
#include "Environment.h"
#include "T_ptr.h"

namespace CORBA {
namespace Nirvana {

/// All bridges should be derived from `Bridge &lt;Interface&gt`.
template <>
class Bridge <Interface>
{
	Bridge (const Bridge&) = delete;
	Bridge& operator = (const Bridge&) = delete;
public:
	struct EPV
	{
		const Char* interface_id;
		Bridge <Interface>* (*duplicate) (Bridge <Interface>*, EnvironmentBridge*);
		void (*release) (Bridge <Interface>*);
	};

	const EPV& _epv () const
	{
		return epv_ref_;
	}

protected:
	Bridge (const EPV& epv) :
		epv_ref_ (epv)
	{}

protected:
	const EPV& epv_ref_;
};

//! Interface - All client interfaces can be widened to it.
class Interface :
	public Bridge <Interface>
{
public:
	static Interface_ptr _duplicate (Interface_ptr itf);
	static void _release (Interface_ptr itf);

	static Interface_ptr _nil ()
	{
		return Interface_ptr::nil ();
	}
};

//! ClientBase - How client obtains pointer to Bridge

template <class T, class I>
class ClientBase
{
protected:
	Bridge <I>& _bridge ()
	{
		return static_cast <Bridge <I>&> (static_cast <T&> (*this));
	}
};

//! Client must derive from ClientBase
template <class T, class I> class Client;

//! ClientInterface - Base template for client interfaces

template <class I>
class ClientInterfaceBase :
	public Bridge <I>,
	public Client <I, I>
{
public:
	operator I& ()
	{
		if (
			Bridge <I>::_epv ().interface_id == Bridge <I>::interface_id_
			||
			RepositoryId::compatible (Bridge <I>::_epv ().interface_id, Bridge <I>::interface_id_)
		)
			return static_cast <I&> (*this);
		else
			throw MARSHAL ();
	}

	operator Interface& ()
	{
		return *static_cast <Interface*> (static_cast <Bridge <Interface>*> (static_cast <Bridge <I>*>(this)));
	}

	static T_ptr <I> _duplicate (T_ptr <I> obj)
	{
		return static_cast <Bridge <I>*> (static_cast <Bridge <Interface>*> (Interface::_duplicate (obj)));
	}

	static T_ptr <I> _nil ()
	{
		return T_ptr <I>::nil ();
	}
};

}

inline bool is_nil (Nirvana::Interface_ptr itf)
{
	return !static_cast <Nirvana::Bridge <Nirvana::Interface>*> (itf);
}

inline void release (Nirvana::Interface_ptr itf)
{
	Nirvana::Interface::_release (itf);
}

}

#endif

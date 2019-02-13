// Nirvana project
// Object Request Broker
#ifndef NIRVANA_ORB_INTERFACE_C_H_
#define NIRVANA_ORB_INTERFACE_C_H_

#include <Nirvana/Nirvana.h>
#include "Environment.h"
#include "T_ptr.h"

namespace CORBA {

class Object;
typedef Nirvana::T_ptr <Object> Object_ptr;
typedef Nirvana::T_var <Object> Object_var;
typedef Nirvana::T_out <Object> Object_out;

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
		Bridge <Interface>* (*duplicate) (Bridge <Interface>*, EnvironmentBridge*);
		void (*release) (Bridge <Interface>*);
	};

	const EPV& _epv () const
	{
		assert (epv_ptr_);
		return *epv_ptr_;
	}

protected:
	Bridge (const EPV& epv) :
		epv_ptr_ (&epv)
	{}

	Bridge ()
	{}

protected:
	const EPV* epv_ptr_;
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

//! ClientInterface - Base template for all client interfaces
template <class T, class I> class Client;

template <class I>
class ClientInterface :
	public Bridge <I>,
	public Client <I, I>
{
public:
	operator Interface& ()
	{
		return static_cast <Interface&> (static_cast <Bridge <Interface>&> (*this));
	}

	static T_ptr <I> _duplicate (T_ptr <I> obj)
	{
		return static_cast <Bridge <I>*> (static_cast <Bridge <Interface>*> (Interface::_duplicate (obj)));
	}

	// For the definition of this method see Object.h
	static T_ptr <I> _narrow (T_ptr <Object> obj);

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

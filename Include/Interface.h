// Nirvana project
// Object Request Broker
#ifndef NIRVANA_ORB_INTERFACE_H_
#define NIRVANA_ORB_INTERFACE_H_

#include <Nirvana.h>
#include "Environment.h"
#include "T_ptr.h"
#include "RepositoryId.h"

namespace CORBA {

class Object;
typedef Nirvana::T_ptr <Object> Object_ptr;
typedef Nirvana::T_var <Object> Object_var;
typedef Nirvana::T_out <Object> Object_out;

namespace Nirvana {

//! All interfaces derived from Interface class.
//! This class provides life-cycle management and pointer to entry-point vector (EPV).
class Interface;

//! All bridges should be derived from Bridge <Interface>.
template <>
class Bridge <Interface>
{
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

//! Interface - All client interfaces derive this
class Interface
{
protected:
	template <class I>
	friend void CORBA::release (T_ptr <I>);

	static Bridge <Interface>* __duplicate (Bridge <Interface>* p);
	static void __release (Bridge <Interface>* p);
};

//! ClientInterface - Base template for all client interfaces
template <class T, class I> class Client;

template <class I>
class ClientInterface :
	public Bridge <I>,
	public Client <I, I>,
	public Interface
{
public:
	static T_ptr <I> _duplicate (T_ptr <I> obj)
	{
		return static_cast <Bridge <I>*> (__duplicate (obj));
	}

	static T_ptr <I> _narrow (T_ptr <Object> obj);

	static T_ptr <I> _nil ()
	{
		return T_ptr <I>::nil ();
	}
};

}

template <class T>
bool is_nil (Nirvana::T_ptr <T> obj)
{
	return !obj.p_;
}

template <class T>
void release (Nirvana::T_ptr <T> obj)
{
	Nirvana::Interface::__release (obj.p_);
}

}

#endif

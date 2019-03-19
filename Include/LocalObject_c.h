#ifndef NIRVANA_ORB_LOCALOBJECT_C_H_
#define NIRVANA_ORB_LOCALOBJECT_C_H_

#include "Object_s.h"

namespace CORBA {

class LocalObject : public Object
{
public:
	void _add_ref ()
	{
		_duplicate (this);
	}

	void _remove_ref ()
	{
		release (this);
	}

	ULong _refcount_value () const
	{
		return 1; // TODO: Implement special method in AbstractBase.
	}
};

namespace Nirvana {

template <>
class T_ptr <LocalObject>
{
public:
	T_ptr ()
		// Zero init skipped for performance
	{}

	T_ptr (Bridge <Object>* p) :
		p_ (static_cast <LocalObject*> (p))
	{}

	operator Bridge <Object>* () const
	{
		return p_;
	}

	LocalObject* operator -> () const
	{
		assert (p_);
		return p_;
	}

	operator bool () const
	{
		return p_ != 0;
	}

	static T_ptr <LocalObject> nil ()
	{
		return T_ptr ((LocalObject*)nullptr);
	}

private:
	LocalObject* p_;
};

}

typedef Nirvana::T_ptr <LocalObject> LocalObject_ptr;
typedef Nirvana::T_var <LocalObject> LocalObject_var;
typedef Nirvana::T_out <LocalObject> LocalObject_out;
typedef Nirvana::T_inout <LocalObject> LocalObject_inout;

}

#endif

#ifndef NIRVANA_ORB_LOCALOBJECT_C_H_
#define NIRVANA_ORB_LOCALOBJECT_C_H_

#include "Object_c.h"

namespace CORBA {

class LocalObject;
typedef Nirvana::T_ptr <LocalObject> LocalObject_ptr;
typedef Nirvana::T_var <LocalObject> LocalObject_var;
typedef Nirvana::T_out <LocalObject> LocalObject_out;
typedef Nirvana::T_inout <LocalObject> LocalObject_inout;

namespace Nirvana {

template <>
class Bridge <LocalObject> : public Bridge <Object>
{
protected:
	Bridge (const EPV& epv) :
		Bridge <Object> (epv)
	{}
};

}

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
		return 1;
	}
};

}

#endif

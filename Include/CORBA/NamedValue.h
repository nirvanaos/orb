#ifndef NIRVANA_ORB_NAMEDVALUE_H_
#define NIRVANA_ORB_NAMEDVALUE_H_

#include "Any.h"

namespace CORBA {

class NamedValue;
typedef Nirvana::I_ptr <NamedValue> NamedValue_ptr;
typedef Nirvana::I_var <NamedValue> NamedValue_var;
typedef Nirvana::I_out <NamedValue> NamedValue_out;
typedef Nirvana::I_inout <NamedValue> NamedValue_inout;

namespace Nirvana {

BRIDGE_BEGIN (NamedValue, CORBA_REPOSITORY_ID ("NamedValue"))
ABI_VT_ret <String> (*name) (Bridge <NamedValue>*, Interface*);
ABI_VT_ret <Any> (*value) (Bridge <NamedValue>*, Interface*);
Flags (*flags) (Bridge <NamedValue>*, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, NamedValue> :
	public T
{
public:
	const String& name ()
	{
		Environment _env;
		Bridge <Object>& _b (T::_get_bridge (_env));
		T_VT_ret <String> _ret = (_b._epv ().epv.name) (&_b, &_env);
		_env.check ();
		return _ret;
	}

	const Any& value ()
	{
		Environment _env;
		Bridge <Object>& _b (T::_get_bridge (_env));
		T_VT_ret <Any> _ret = (_b._epv ().epv.value) (&_b, &_env);
		_env.check ();
		return _ret;
	}

	Flags flags ()
	{
		Environment _env;
		Bridge <Object>& _b (T::_get_bridge (_env));
		Flags _ret = (_b._epv ().epv.flags) (&_b, &_env);
		_env.check ();
		return _ret;
	}
};

}
}

#endif

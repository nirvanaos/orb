#ifndef NIRVANA_TESTORB_LOCALOBJECTSTATICCORE_H_
#define NIRVANA_TESTORB_LOCALOBJECTSTATICCORE_H_

#include <ImplementationStatic.h>

namespace CORBA {
namespace Nirvana {

template <class S, class Primary>
class LocalObjectStaticCore :
	public AbstractBaseStatic <S>,
	public InterfaceStatic <S, Object>
{
public:
	static T_ptr <Primary> _this ()
	{
		return InterfaceStatic <S, Primary>::_get_ptr ();
	}

	// Object operations

	static ImplementationDef_ptr _get_implementation ()
	{
		return ImplementationDef_ptr::nil ();
	}

	static InterfaceDef_ptr _get_interface ()
	{
		return InterfaceDef_ptr::nil ();
	}

	static Boolean _is_a (const Char* type_id)
	{
		Bridge <Interface>* itf = AbstractBaseStatic <S>::_get_ptr ()->_query_interface (type_id);
		if (itf) {
			(itf->_epv ().release) (itf);
			return true;
		} else
			return false;
	}

	static Boolean _non_existent ()
	{
		return false;
	}

	static Boolean _is_equivalent (Object_ptr other_object)
	{
		return _get_interface () == other_object;
	}

	static ULong _hash (ULong maximum)
	{
		return 0;
	}
};

}
}

#endif

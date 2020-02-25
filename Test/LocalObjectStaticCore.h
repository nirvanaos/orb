#ifndef NIRVANA_TESTORB_LOCALOBJECTSTATICCORE_H_
#define NIRVANA_TESTORB_LOCALOBJECTSTATICCORE_H_

#include <CORBA/ImplementationStatic.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class LocalObjectStaticCore :
	public InterfaceStatic <S, AbstractBase>,
	public InterfaceStaticBase <S, Object>
{
public:
	// Object operations

	static ImplementationDef_ptr _get_implementation ()
	{
		return ImplementationDef::_nil ();
	}

	static InterfaceDef_ptr _get_interface ()
	{
		return InterfaceDef::_nil ();
	}

	static Boolean _is_a (const String& type_id)
	{
		Interface* itf = S::_query_interface (type_id);
		if (itf)
			return true;
		else
			return false;
	}

	static Boolean _non_existent ()
	{
		return false;
	}

	static Boolean _is_equivalent (Object_ptr other)
	{
		return static_cast <AbstractBase*> (InterfaceStatic <S, AbstractBase>::_bridge ())->_query_interface (0) == AbstractBase_ptr (other)->_query_interface (0);
	}

	static ULong _hash (ULong maximum)
	{
		return 0;
	}
	// TODO: Other Object operations shall be here...
};

}
}

#endif

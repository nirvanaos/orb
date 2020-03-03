#ifndef NIRVANA_ORB_OBJECTSTATIC_H_
#define NIRVANA_ORB_OBJECTSTATIC_H_

#include <Nirvana/NirvanaBase.h>
#include <Nirvana/OLF.h>
#include "AbstractBaseStatic.h"
#include "Object_s.h"

namespace CORBA {
namespace Nirvana {

//! Static implementation of LocalObject
//! \tparam S Servant class.
//! \tparam Primary Primary interface.
template <class S>
class InterfaceStatic <S, Object> :
	public InterfaceStatic <S, AbstractBase>,
	public InterfaceStaticBase <S, Object>
{
public:
	static Bridge <Object>* _get_object (String_in iid)
	{
		return static_cast <Bridge <Object>*> (AbstractBase_ptr (object ())->_query_interface (iid));
	}

	// Object operations

	static ImplementationDef_var _get_implementation ()
	{
		return object ()->_get_implementation ();
	}

	static InterfaceDef_var _get_interface ()
	{
		return object ()->_get_interface ();
	}

	static Boolean _is_a (const String& type_id)
	{
		return object ()->_is_a (type_id);
	}

	static Boolean _non_existent ()
	{
		return object ()->_non_existent ();
	}

	static Boolean _is_equivalent (Object_ptr other_object)
	{
		return object ()->_is_equivalent (other_object);
	}

	static ULong _hash (ULong maximum)
	{
		return object ()->_hash (maximum);
	}
	// TODO: Other Object operations shall be here...

protected:
	static Interface* _get_proxy ()
	{
		Interface* proxy = AbstractBase_ptr (object ())->_query_interface (0);
		if (!proxy)
			::Nirvana::throw_MARSHAL ();
		return proxy;
	}

private:
	static Object_ptr object ()
	{
		return static_cast <Object*> (export_struct_.core_object);
	}

	static __declspec (allocate(OLF_BIND)) const ::Nirvana::ExportLocal export_struct_;
};

template <class S> __declspec (allocate(OLF_BIND))
const ::Nirvana::ExportLocal InterfaceStatic <S, Object>::export_struct_{ ::Nirvana::OLF_EXPORT_LOCAL, S::constant_name
, STATIC_BRIDGE (S, Object) };

}
}

#endif

#ifndef NIRVANA_ORB_LOCALOBJECTSTATIC_H_
#define NIRVANA_ORB_LOCALOBJECTSTATIC_H_

#include <Nirvana/NirvanaBase.h>
#include <Nirvana/OLF.h>
#include "AbstractBaseStatic.h"
#include "ReferenceCounterStatic.h"
#include "LocalObject_s.h"
#include "local_core.h"

namespace CORBA {
namespace Nirvana {

//! Static implementation of LocalObject
//! \tparam S Servant class.
//! \tparam Primary Primary interface.
template <class S>
class InterfaceStatic <S, LocalObject> :
	public InterfaceStatic <S, AbstractBase>,
	public InterfaceStatic <S, ReferenceCounter>,
	public InterfaceStaticBase <S, LocalObject>
{
public:
	static Bridge <Object>* _get_object (String_in iid)
	{
		return get_object_from_core (core_object (), iid);
	}

	// Object operations

	static Boolean _non_existent ()
	{
		return false;
	}

protected:
	static Interface* _get_proxy ()
	{
		return get_proxy (core_object ());
	}

private:
	static LocalObject_ptr core_object ()
	{
		return static_cast <LocalObject*> (export_struct_.core_object);
	}

	static __declspec (allocate (OLF_BIND)) const ::Nirvana::ExportLocal export_struct_;
};

template <class S> __declspec (allocate(OLF_BIND))
const ::Nirvana::ExportLocal InterfaceStatic <S, LocalObject>::export_struct_{ ::Nirvana::OLF_EXPORT_LOCAL, S::constant_name
, STATIC_BRIDGE (S, LocalObject), STATIC_BRIDGE (S, AbstractBase) };

}
}

#endif

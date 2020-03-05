#ifndef NIRVANA_ORB_LOCALOBJECT_H_
#define NIRVANA_ORB_LOCALOBJECT_H_

#include "Object.h"
#include "ReferenceCounter.h"

namespace CORBA {

class LocalObject;
typedef Nirvana::I_var <LocalObject> LocalObject_var;
typedef Nirvana::I_out <LocalObject> LocalObject_out;
typedef Nirvana::I_inout <LocalObject> LocalObject_inout;

namespace Nirvana {

BRIDGE_BEGIN (LocalObject, CORBA_REPOSITORY_ID ("LocalObject"))
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BASE_STRUCT_ENTRY (ReferenceCounter, _ReferenceCounter)
BRIDGE_EPV
BRIDGE_END ()

template <> /// We can obtain I_ptr <LocalObject> directly from servant pointer
class I_ptr <LocalObject> : public I_ptr_base <LocalObject>
{
public:
	I_ptr () NIRVANA_NOEXCEPT
	{}

	/// We can obtain I_ptr directly from servant pointer
	I_ptr (Bridge <LocalObject>* p) NIRVANA_NOEXCEPT :
		I_ptr_base <LocalObject> (reinterpret_cast <LocalObject*> (p))
	{}

	I_ptr (const I_ptr& src) NIRVANA_NOEXCEPT :
		I_ptr_base (src)
	{}

	I_ptr (const I_var <LocalObject>& var) NIRVANA_NOEXCEPT :
		I_ptr_base (var)
	{}

	/// Move constructor in case returned I_var assigned to I_ptr:
	///    Object_var func ();
	///    Object_ptr obj = func ();
	I_ptr (I_var <LocalObject>&& var) NIRVANA_NOEXCEPT
	{
		this->move_from (var);
	}

	I_ptr& operator = (const I_ptr& src) NIRVANA_NOEXCEPT
	{
		I_ptr_base::operator = (src);
		return *this;
	}

	/// When servant returns `I_ptr`, skeleton must be able to convert
	/// it to the ABI return type `Interface*`
	Bridge <LocalObject>* operator & () const NIRVANA_NOEXCEPT
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)this->p_);
		return reinterpret_cast <Bridge <LocalObject>*> (this->p_);
	}
};

}

typedef Nirvana::I_ptr <LocalObject> LocalObject_ptr;

class LocalObject : public Nirvana::ClientInterface <LocalObject, Object, Nirvana::ReferenceCounter>
{};

}

#endif

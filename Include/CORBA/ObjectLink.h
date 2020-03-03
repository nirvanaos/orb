#ifndef NIRVANA_ORB_OBJECTLINK_H_
#define NIRVANA_ORB_OBJECTLINK_H_

#include "Object.h"

namespace CORBA {
namespace Nirvana {

//! \brief Implements delegate to the core Object implementation.
class ObjectLink :
	public Bridge <Object>
{
public:
	Bridge <Object>* _get_object (String_in iid) const;

	// Object operations

	ImplementationDef_var _get_implementation () const
	{
		return object_->_get_implementation ();
	}

	InterfaceDef_var _get_interface () const
	{
		return object_->_get_interface ();
	}

	Boolean _is_a (const String& type_id) const
	{
		return object_->_is_a (type_id);
	}

	Boolean _non_existent () const
	{
		return object_->_non_existent ();
	}

	Boolean _is_equivalent (Object_ptr other_object) const
	{
		return object_->_is_equivalent (other_object);
	}

	ULong _hash (ULong maximum) const
	{
		return object_->_hash (maximum);
	}
	// TODO: Other Object operations shall be here...

protected:
	ObjectLink (const Bridge <Object>::EPV& epv);

	ObjectLink (const ObjectLink&) = delete;
	ObjectLink& operator = (const ObjectLink&)
	{
		return *this; // Do nothing
	}

	Interface* _get_proxy ();

private:
	Object_var object_;
};

}
}

#endif

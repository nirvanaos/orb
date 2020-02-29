#ifndef NIRVANA_ORB_OBJECTLINK_H_
#define NIRVANA_ORB_OBJECTLINK_H_

#include "Object.h"

namespace CORBA {
namespace Nirvana {

//! \brief Implements delegate to the core Object implementation.
class ObjectLink
{
public:
	// Object operations

	ImplementationDef_ptr _get_implementation ()
	{
		return object_->_get_implementation ();
	}

	InterfaceDef_ptr _get_interface ()
	{
		return object_->_get_interface ();
	}

	Boolean _is_a (const String& type_id)
	{
		return object_->_is_a (type_id);
	}

	Boolean _non_existent ()
	{
		return object_->_non_existent ();
	}

	Boolean _is_equivalent (Object_ptr other_object)
	{
		return object_->_is_equivalent (other_object);
	}

	ULong _hash (ULong maximum)
	{
		return object_->_hash (maximum);
	}
	// TODO: Other Object operations shall be here...

protected:
	ObjectLink () :
		object_ (Object::_nil ())
	{}

	ObjectLink (const ObjectLink&) = delete;
	ObjectLink& operator = (const ObjectLink&)
	{
		return *this; // Do nothing
	}

	void _construct (Bridge <Object>* impl);

	Interface* _get_proxy ();

private:
	Object_ptr object_;
};

}
}

#endif

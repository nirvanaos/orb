#ifndef NIRVANA_ORB_OBJECTLINK_H_
#define NIRVANA_ORB_OBJECTLINK_H_

#include "LocalObject.h"

namespace CORBA {
namespace Nirvana {

//! \brief Implements delegate to the core Object implementation.
class ObjectLink :
	public Bridge <LocalObject>
{
public:
	Bridge <Object>* _get_object (String_in iid) const;

	// Object operations

	Boolean _non_existent () const
	{
		return false;
	}

	InterfaceDef_var _get_interface ()
	{
		return object_->_get_interface ();
	}

	Boolean _is_a (const String& type_id)
	{
		return object_->_is_a (type_id);
	}

protected:
	ObjectLink (const Bridge <LocalObject>::EPV& epv, Bridge <AbstractBase>& ab);

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

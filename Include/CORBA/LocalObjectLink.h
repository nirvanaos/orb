#ifndef NIRVANA_ORB_LOCALOBJECTLINK_H_
#define NIRVANA_ORB_LOCALOBJECTLINK_H_

#include "local_core.h"

namespace CORBA {
namespace Nirvana {

//! \brief Implements delegate to the core LocalObject implementation.
class LocalObjectLink :
	public Bridge <LocalObject>
{
public:
	Bridge <Object>* _get_object (String_in iid) const
	{
		return get_object_from_core (core_object_, iid);
	}

	// Object operations

	Boolean _non_existent () const
	{
		return false;
	}

protected:
	LocalObjectLink (const Bridge <LocalObject>::EPV& epv, Bridge <AbstractBase>& ab);

	LocalObjectLink (const LocalObjectLink&) = delete;
	LocalObjectLink& operator = (const LocalObjectLink&)
	{
		return *this; // Do nothing
	}

	Interface* _get_proxy ()
	{
		return get_proxy (core_object_);
	}

protected:
	LocalObject_var core_object_;
};

}
}

#endif

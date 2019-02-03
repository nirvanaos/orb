// Nirvana project.
// Object server stub implementation.
#ifndef NIRVANA_ORB_TEST_OBJECTCORE_H_
#define NIRVANA_ORB_TEST_OBJECTCORE_H_

#include <ORB.h>
#include "../../core/Source/AtomicCounter.h"

namespace CORBA {
namespace Nirvana {

class ObjectCore :
	public AbstractBaseNoRefCnt <ObjectCore>,
	public Bridge <Object>,
	public Skeleton <ObjectCore, Object>
{
public:
	ObjectCore (ServantBase_ptr servant, Bridge <Object>** active_object = nullptr) :
		servant_ (servant),
		active_object_ (active_object)
	{}

	void _add_ref ();
	void _remove_ref ();

	static Bridge <Interface>* _find_interface (Bridge <AbstractBase>& base, const Char* id)
	{
		ServantBase_var servant = static_cast <ObjectCore&> (base).servant_;
		if (servant)
			return servant->_find_interface (id);
		else
			return Skeleton <ObjectCore, Object>::_find_interface (base, id);
	}

	void deactivate ()
	{
	}

	static ImplementationDef_ptr _get_implementation ()
	{
		return nullptr;
	}

	InterfaceDef_ptr __get_interface () const
	{
		return servant_->_get_interface ();
	}

	Boolean _is_a (const Char* type_id) const
	{
		return servant_->_is_a (type_id);
	}

	Boolean _non_existent () const
	{
		return servant_->_non_existent ();
	}

	static Boolean __is_equivalent (Bridge <Object>* obj, Bridge <Object>* other, EnvironmentBridge*)
	{
		return obj == other;
	}

	ULong _hash (ULong maximum)
	{
		return 0; // TODO: Implement.
	}
	// TODO: Other Object operations shall be here...

private:
	ServantBase_ptr servant_;
	Bridge <Object>** active_object_;
};

}
}

#endif

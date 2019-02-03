#ifndef NIRVANA_ORB_TEST_SERVANTCORE_H_
#define NIRVANA_ORB_TEST_SERVANTCORE_H_

#include <ORB.h>

namespace CORBA {
namespace Nirvana {

class ServantCore :
	public AbstractBaseNoRefCnt <ServantCore>,
	public InterfaceImpl <ServantCore, ServantBase>
{
public:
	ServantCore (AbstractBase_ptr base, const Char* interface_id) :
		abstract_base_ (base),
		interface_id_ (interface_id)
	{}

	static void _add_ref ()
	{}

	static void _remove_ref ()
	{}

	static Bridge <Interface>* __find_interface (Bridge <AbstractBase>* base, const Char* id, EnvironmentBridge* env)
	{
		return nullptr;
	}

	POA_ptr _default_POA () const
	{
		return nullptr;
	}

	InterfaceDef_ptr _get_interface () const
	{
		return nullptr;
	}

	Boolean _is_a (const Char* type_id) const
	{
		Bridge <Interface>* itf = abstract_base_->_find_interface (type_id);
		if (itf) {
			Interface::__release (itf);
			return true;
		} else
			return false;
	}

	static Boolean _non_existent ()
	{
		return false;
	}

private:
	const AbstractBase_ptr abstract_base_;
	const Char* const interface_id_;
};

}
}

#endif

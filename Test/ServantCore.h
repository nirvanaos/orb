#ifndef NIRVANA_TESTORB_SERVANTCORE_H_
#define NIRVANA_TESTORB_SERVANTCORE_H_

#include <ServantBase_s.h>
#include <Servant.h>

namespace CORBA {
namespace Nirvana {

class ServantCore :
	public ServantTraits <ServantCore>,
	public InterfaceImpl <ServantCore, AbstractBase>,
	public InterfaceImpl <ServantCore, ServantBase>,
	public LifeCycleStatic
{
public:
	ServantCore (AbstractBase_ptr base, const Char* interface_id) :
		abstract_base_ (base),
		interface_id_ (interface_id)
	{}

	static Bridge <Interface>* __find_interface (Bridge <AbstractBase>* base, const Char* id, EnvironmentBridge* env)
	{
		return nullptr;
	}

	::PortableServer::POA_ptr _default_POA () const;

	InterfaceDef_ptr _get_interface () const
	{
		return InterfaceDef_ptr::nil ();	// TODO: Implement
	}

	static Boolean __is_a (Bridge <ServantBase>* obj, const Char* type_id, EnvironmentBridge* env)
	{
		AbstractBase_ptr base = _implementation (obj).abstract_base_;
		Bridge <Interface>* itf = (base->_epv ().epv.find_interface) (base, type_id, env);
		if (itf) {
			(itf->_epv ().release) (itf);
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

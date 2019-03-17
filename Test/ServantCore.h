#ifndef NIRVANA_TESTORB_SERVANTCORE_H_
#define NIRVANA_TESTORB_SERVANTCORE_H_

#include "ObjectCore.h"
#include <ServantBase_s.h>

namespace CORBA {
namespace Nirvana {

class ServantCore :
	public ServantTraits <ServantCore>,
	public Skeleton <ServantCore, AbstractBase>,
	public InterfaceImpl <ServantCore, ServantBase>,
	public LifeCycleNoCopy <ServantCore>
{
public:
	ServantCore (ServantBase_ptr servant, DynamicServant_ptr dynamic) :
		object_core_ (servant, dynamic)
	{}

	operator Bridge <Object>& ()
	{
		return object_core_;
	}

	::PortableServer::POA_ptr _default_POA () const;

	InterfaceDef_ptr _get_interface () const
	{
		return InterfaceDef_ptr::nil ();	// TODO: Implement
	}

	static Boolean __is_a (Bridge <ServantBase>* obj, const Char* type_id, EnvironmentBridge* env)
	{
		try {
			AbstractBase_ptr base = _implementation (obj).object_core_.abstract_base ();
			Bridge <Interface>* itf = (base->_epv ().epv.query_interface) (base, type_id, env);
			if (itf) {
				(itf->_epv ().release) (itf);
				return TRUE;
			} else
				return FALSE;
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	Boolean _non_existent () const
	{
		return !object_core_.is_active_;
	}

private:
	ObjectCore object_core_;
};

}
}

#endif

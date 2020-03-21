#ifndef NIRVANA_ORB_CORE_PROXYBASE_H_
#define NIRVANA_ORB_CORE_PROXYBASE_H_

#include "ProxyManager.h"
#include <CORBA/Object_s.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

template <class Proxy, class ImplItf>
class ProxyBase :
	public ServantTraits <Proxy>,
	public InterfaceImplBase <Proxy, AbstractBase>,
	public Skeleton <Proxy, Object>,
	public LifeCycleRefCnt < Proxy>,
	public ProxyManager
{
public:
	static Interface* __get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		return 0; // We dont implement it
	}

	InterfaceDef_var _get_interface () const
	{
		::Nirvana::Synchronized sync (sync_context ());
		return static_cast <const Proxy&> (*this).servant_->_get_interface ();
	}

	Boolean _is_a (const String& type_id) const
	{
		const String tmp (type_id);
		::Nirvana::Synchronized sync (sync_context ());
		return static_cast <const Proxy&> (*this).servant_->_is_a (tmp);
	}

	Boolean _non_existent () const
	{
		::Nirvana::Synchronized sync (sync_context ());
		return static_cast <const Proxy&> (*this).servant_->_non_existent ();
	}

protected:
	ProxyBase (I_ptr <ImplItf> servant, Interface_ptr lifecycle) :
		ProxyManager (Skeleton <Proxy, Object>::epv_, servant, lifecycle),
		servant_ (servant)
	{}

protected:
	I_ptr <ImplItf> servant_;
};

}
}
}

#endif

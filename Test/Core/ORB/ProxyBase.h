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
		::Nirvana::ContextFrame frame;
		sync_context ()->enter (frame);
		InterfaceDef_var ret = static_cast <const Proxy&> (*this).servant_->_get_interface ();
		sync_context ()->leave (frame);
		return ret;
	}

	Boolean _is_a (const String& type_id) const
	{
		const String tmp (type_id);
		::Nirvana::ContextFrame frame;
		sync_context ()->enter (frame);
		Boolean ret = static_cast <const Proxy&> (*this).servant_->_is_a (tmp);
		sync_context ()->leave (frame);
		return ret;
	}

	Boolean _non_existent () const
	{
		::Nirvana::ContextFrame frame;
		sync_context ()->enter (frame);
		Boolean ret = static_cast <const Proxy&> (*this).servant_->_non_existent ();
		sync_context ()->leave (frame);
		return ret;
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

#ifndef NIRVANA_ORB_PLATFORMPROXYBASE_H_
#define NIRVANA_ORB_PLATFORMPROXYBASE_H_

#include "../CORBA.h"
#include "../ServantImpl.h"
#include "../DynamicServant_s.h"
#include "PlatformObjRef.h"

namespace CORBA {
namespace Nirvana {

template <class I> class PlatformProxy;

class PlatformProxyRoot :
	public Bridge <DynamicServant>
{
public:
	Bridge <Object>* _get_object (String_in iid) const
	{
		Environment env;
		typename Bridge <PlatformObjRef>:: template Wide <Object>::Func func = proxy_manager_->_epv ().base;
		Bridge <Object>* ret = (func)(&proxy_manager_, iid, &env);
		env.check ();
		return ret;
	}

	void _add_ref ()
	{
		interface_duplicate (&proxy_manager_);
	}

	void _remove_ref ()
	{
		release (proxy_manager_);
	}

	DynamicServant_ptr _dynamic_servant ()
	{
		return &static_cast <DynamicServant&> (static_cast <Bridge <DynamicServant>&> (*this));
	}

	PlatformObjRef_ptr _target () const
	{
		return proxy_manager_;
	}

	UShort _interface_idx () const
	{
		return interface_idx_;
	}

protected:
	PlatformProxyRoot (const Bridge <DynamicServant>::EPV& epv, 
		PlatformObjRef_ptr proxy_manager, UShort interface_idx) :
		Bridge <DynamicServant> (epv),
		proxy_manager_ (proxy_manager),
		interface_idx_ (interface_idx)
	{}

private:
	PlatformObjRef_ptr proxy_manager_;
	UShort interface_idx_;
};

template <class S>
class ProxyLifeCycle :
	public ServantTraits <S>,
	public LifeCycleRefCnt <S>,
	public Skeleton <S, DynamicServant>
{};

template <class I>
class PlatformProxyBase :
	public PlatformProxyRoot,
	public ProxyLifeCycle <PlatformProxy <I> >,
	public InterfaceImplBase <PlatformProxy <I>, I>
{
public:
	Interface* _proxy ()
	{
		return &static_cast <Bridge <I>&> (*this);
	}

protected:
	PlatformProxyBase (PlatformObjRef_ptr proxy_manager, UShort interface_idx) :
		PlatformProxyRoot (ProxyLifeCycle <PlatformProxy <I> >::epv_, proxy_manager, interface_idx)
	{}
};

}
}

#endif

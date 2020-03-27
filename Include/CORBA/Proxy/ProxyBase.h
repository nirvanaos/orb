#ifndef NIRVANA_ORB_PROXYBASE_H_
#define NIRVANA_ORB_PROXYBASE_H_

#include "../CORBA.h"
#include "../ServantImpl.h"
#include "../DynamicServant_s.h"
#include "IOReference.h"

namespace CORBA {
namespace Nirvana {

template <class I> class Proxy;

class ProxyRoot
{
public:
	Bridge <Object>* _get_object (String_in iid) const
	{
		Environment env;
		typename Bridge <IOReference>:: template Wide <Object>::Func func = proxy_manager_->_epv ().base;
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

	IOReference_ptr _target () const
	{
		return proxy_manager_;
	}

	UShort _interface_idx () const
	{
		return interface_idx_;
	}

protected:
	ProxyRoot (IOReference_ptr proxy_manager, UShort interface_idx) :
		proxy_manager_ (proxy_manager),
		interface_idx_ (interface_idx)
	{}

private:
	IOReference_ptr proxy_manager_;
	UShort interface_idx_;
};

template <class S>
class ProxyLifeCycle :
	public ServantTraits <S>,
	public LifeCycleRefCnt <S>,
	public InterfaceImplBase <S, DynamicServant>
{
public:
	DynamicServant_ptr _dynamic_servant ()
	{
		return &static_cast <DynamicServant&> (static_cast <Bridge <DynamicServant>&> (*this));
	}
};

template <class I>
class ProxyBase :
	public ProxyRoot,
	public ProxyLifeCycle <Proxy <I> >,
	public InterfaceImplBase <Proxy <I>, I>
{
public:
	Interface* _proxy ()
	{
		return &static_cast <Bridge <I>&> (*this);
	}

protected:
	ProxyBase (IOReference_ptr proxy_manager, UShort interface_idx) :
		ProxyRoot (proxy_manager, interface_idx)
	{}
};

}
}

#endif

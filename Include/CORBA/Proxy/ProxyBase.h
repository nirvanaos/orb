#ifndef NIRVANA_ORB_PROXYBASE_H_
#define NIRVANA_ORB_PROXYBASE_H_

#include "../CORBA.h"
#include "../ServantImpl.h"
#include "../ServantMemory.h"
#include "../DynamicServant_s.h"
#include "IOReference.h"

namespace CORBA {
namespace Nirvana {

template <class I> class Proxy;

class ProxyRoot : public ServantMemory
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

template <class I>
class ProxyBaseInterface
{
public:
	void init (AbstractBase_ptr ab)
	{
		proxy_ = static_cast <Bridge <I>*> (ab->_query_interface (I::repository_id_));
		if (!proxy_)
			throw OBJ_ADAPTER ();
	}

	Bridge <I>* get () const
	{
		return proxy_;
	}

private:
	Bridge <I>* proxy_;
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

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, String_in id, EnvironmentBridge* env)
	{
		try {
			if (!RepositoryId::compatible (Bridge <Base>::repository_id_, id))
				::Nirvana::throw_INV_OBJREF ();
			return static_cast <ProxyBaseInterface <Base>&> (S::_implementation (derived)).get ();
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
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

#include <CORBA/Server.h>
#include "Test_I1_s.h"
#include <CORBA/ProxyFactory_s.h>
#include <Nirvana/Synchronized.h>

namespace CORBA {
namespace Nirvana {

class ProxyBase :
	public Bridge <DynamicServant>
{
public:
	Bridge <Object>* _get_object (String_in iid)
	{
		return static_cast <Bridge <Object>*> (AbstractBase_ptr (proxy_manager_)->_query_interface (iid));
	}

	void _add_ref ()
	{
		interface_duplicate (proxy_manager_);
	}

	void _remove_ref ()
	{
		interface_release (proxy_manager_);
	}

	DynamicServant_ptr _dynamic_servant ()
	{
		return &static_cast <DynamicServant&> (static_cast <Bridge <DynamicServant>&> (*this));
	}

protected:
	ProxyBase (const Bridge <DynamicServant>::EPV& epv, Object_ptr proxy_manager) :
		Bridge <DynamicServant> (epv),
		proxy_manager_ (proxy_manager)
	{}

private:
	Object_ptr proxy_manager_;
};

class ServantProxyBase :
	public ProxyBase
{
protected:
	ServantProxyBase (const Bridge <DynamicServant>::EPV& epv, Object_ptr proxy_manager, ::Nirvana::SyncDomainTraits_ptr sync) :
		ProxyBase (epv, proxy_manager),
		sync_domain_ (sync)
	{}

	::Nirvana::SyncDomainTraits_ptr _sync_domain () const
	{
		return sync_domain_;
	}

private:
	::Nirvana::SyncDomainTraits_ptr sync_domain_;
};

template <class S>
class ProxyLifeCycle :
	public ServantTraits <S>,
	public LifeCycleRefCnt <S>,
	public Skeleton <S, DynamicServant>
{};

template <class S, class I>
class ServantProxyBaseT :
	public ServantProxyBase,
	public ProxyLifeCycle <S>,
	public InterfaceImplBase <S, I>
{
public:
	Interface* _proxy ()
	{
		return &static_cast <Bridge <I>&> (*this);
	}

protected:
	ServantProxyBaseT (CORBA::Object_ptr proxy_manager, ::Nirvana::SyncDomainTraits_ptr sync, Interface_ptr servant) :
		ServantProxyBase (ProxyLifeCycle <S>::epv_, proxy_manager, sync),
		servant_ (I::_check (servant))
	{}

	I_ptr <I> _servant () const
	{
		return servant_;
	}

private:
	I_ptr <I> servant_;
};

}
}

namespace Test {

class I1_ServantProxy :
	public CORBA::Nirvana::ServantProxyBaseT <I1_ServantProxy, I1>
{
	typedef CORBA::Nirvana::ServantProxyBaseT <I1_ServantProxy, I1> Base;
public:
	I1_ServantProxy (CORBA::Object_ptr proxy_manager, ::Nirvana::SyncDomainTraits_ptr sync,
		CORBA::Nirvana::Interface_ptr servant) :
		Base (proxy_manager, sync, servant)
	{}

	CORBA::Long op1 (CORBA::Long p1)
	{
		Nirvana::Synchronized sync (_sync_domain ());
		return _servant ()->op1 (p1);
	}

	void throw_NO_IMPLEMENT ();
	I1_var object_op (I1_ptr in_obj, I1_var& out_obj, I1_var& inout_obj);
	std::string string_op (const std::string& in_s, std::string& out_s, std::string& inout_s);
	std::vector <CORBA::Long> seq_op (const std::vector <CORBA::Long>& in_s, std::vector <CORBA::Long>& out_s, std::vector <CORBA::Long>& inout_s);
	CORBA::Any any_op (const CORBA::Any& in_any, CORBA::Any& out_any, CORBA::Any& inout_any);
};

class I1_proxy_factory :
	public CORBA::Nirvana::ServantStatic <I1_proxy_factory, CORBA::Nirvana::ProxyFactory>
{
public:
	CORBA::Nirvana::Interface_ptr create_servant_proxy (
		CORBA::Object_ptr proxy_manager,
		Nirvana::SyncDomainTraits_ptr sync, CORBA::Nirvana::Interface_ptr servant,
		CORBA::Nirvana::DynamicServant_ptr& deleter)
	{
		I1_ServantProxy* proxy =
			Nirvana::stateless_create <I1_ServantProxy> (proxy_manager, sync, servant);
		deleter = proxy->_dynamic_servant ();
		return proxy->_proxy ();
	}

};

}

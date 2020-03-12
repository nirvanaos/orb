#include <CORBA/Server.h>
#include "Test_I1_s.h"
#include <CORBA/ProxyFactory_s.h>
#include <CORBA/MarshalTraits.h>

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
	ServantProxyBase (const Bridge <DynamicServant>::EPV& epv, Object_ptr proxy_manager, ::Nirvana::SynchronizationContext_ptr sync) :
		ProxyBase (epv, proxy_manager),
		sync_context_ (sync)
	{}

	::Nirvana::SynchronizationContext_ptr _sync_context () const
	{
		return sync_context_;
	}

private:
	::Nirvana::SynchronizationContext_ptr sync_context_;
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
	ServantProxyBaseT (CORBA::Object_ptr proxy_manager, ::Nirvana::SynchronizationContext_ptr sync, Interface_ptr servant) :
		ServantProxyBase (ProxyLifeCycle <S>::epv_, proxy_manager, sync),
		servant_ (I::_check (&servant))
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
	I1_ServantProxy (CORBA::Object_ptr proxy_manager, ::Nirvana::SynchronizationContext_ptr sync,
		CORBA::Nirvana::Interface_ptr servant) :
		Base (proxy_manager, sync, servant)
	{}

	CORBA::Long op1 (CORBA::Long p1) const
	{
		::Nirvana::Synchronized sync (_sync_context ());
		return _servant ()->op1 (p1);
	}

	void throw_NO_IMPLEMENT () const
	{
		::Nirvana::Synchronized sync (_sync_context ());
		_servant ()->throw_NO_IMPLEMENT ();
	}

	I1_var object_op (I1_ptr in_obj, I1_var& out_obj, I1_var& inout_obj) const
	{
		I1_var _tmp_in_obj (I1::_duplicate (in_obj));
		I1_var _tmp_out_obj;
		I1_var _tmp_inout_obj (inout_obj);
		I1_var _ret;
		{
			::Nirvana::Synchronized sync (_sync_context ());
			_ret = _servant ()->object_op (_tmp_in_obj, _tmp_out_obj, _tmp_inout_obj);
		}
		out_obj = _tmp_out_obj._retn ();
		inout_obj = _tmp_inout_obj._retn ();
		return _ret;
	}

	CORBA::Nirvana::String string_op (const CORBA::Nirvana::String& in_s, CORBA::Nirvana::String& out_s, CORBA::Nirvana::String& inout_s) const
	{
		CORBA::Nirvana::String _ret;

		// out parameters
		CORBA::Nirvana::String _ret_out_s;
		CORBA::Nirvana::String _ret_inout_s;

		{
			// in parameters
			CORBA::Nirvana::String _tmp_in_s (in_s);

			::Nirvana::Synchronized context;

			// Copy inout parameters to the target context memory
			_sync_context ()->enter_memory ();
			CORBA::Nirvana::String _tmp_out_s;
			CORBA::Nirvana::String _tmp_inout_s (inout_s);

			// Enter into synchronization domain and perform call
			_sync_context ()->enter (false);
			CORBA::Nirvana::String _tmp_ret = _servant ()->string_op (_tmp_in_s, _tmp_out_s, _tmp_inout_s);

			// Move output data back to the call context
			CORBA::Nirvana::MarshalTraits <CORBA::Nirvana::String>::move_out (_tmp_out_s, context.call_context (), _ret_out_s);
			CORBA::Nirvana::MarshalTraits <CORBA::Nirvana::String>::move_out (_tmp_inout_s, context.call_context (), _ret_inout_s);
			CORBA::Nirvana::MarshalTraits <CORBA::Nirvana::String>::move_out (_tmp_ret, context.call_context (), _ret);

			// Leave context
		}
		// Return data to the caller
		_ret_out_s.swap (out_s);
		_ret_inout_s.swap (inout_s);
		return _ret;
	}

	std::vector <CORBA::Long> seq_op (const SeqLong& in_s, SeqLong& out_s, SeqLong& inout_s) const
	{
		SeqLong _ret;

		// out parameters
		SeqLong _ret_out_s;
		SeqLong _ret_inout_s;

		{
			// in parameters
			SeqLong _tmp_in_s (in_s);

			::Nirvana::Synchronized context;

			// Copy inout parameters to the target context memory
			_sync_context ()->enter_memory ();
			SeqLong _tmp_out_s;
			SeqLong _tmp_inout_s (inout_s);

			// Enter into synchronization domain and perform call
			_sync_context ()->enter (false);
			SeqLong _tmp_ret = _servant ()->seq_op (_tmp_in_s, _tmp_out_s, _tmp_inout_s);

			// Move output data back to the call context
			CORBA::Nirvana::MarshalTraits <SeqLong>::move_out (_tmp_out_s, context.call_context (), _ret_out_s);
			CORBA::Nirvana::MarshalTraits <SeqLong>::move_out (_tmp_inout_s, context.call_context (), _ret_inout_s);
			CORBA::Nirvana::MarshalTraits <SeqLong>::move_out (_tmp_ret, context.call_context (), _ret);

			// Leave context
		}
		// Return data to the caller
		_ret_out_s.swap (out_s);
		_ret_inout_s.swap (inout_s);
		return _ret;
	}

	CORBA::Any any_op (const CORBA::Any& in_any, CORBA::Any& out_any, CORBA::Any& inout_any) const;
};

class I1_proxy_factory :
	public CORBA::Nirvana::ServantStatic <I1_proxy_factory, CORBA::Nirvana::ProxyFactory>
{
public:
	CORBA::Nirvana::Interface_ptr create_servant_proxy (
		CORBA::Object_ptr proxy_manager,
		Nirvana::SynchronizationContext_ptr sync, CORBA::Nirvana::Interface_ptr servant,
		CORBA::Nirvana::DynamicServant_ptr& deleter)
	{
		I1_ServantProxy* proxy =
			Nirvana::stateless_create <I1_ServantProxy> (proxy_manager, sync, servant);
		deleter = proxy->_dynamic_servant ();
		return proxy->_proxy ();
	}

};

}

// Nirvana project
// Object Request Broker
// Tied interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONTIED_H_
#define NIRVANA_ORB_IMPLEMENTATIONTIED_H_

#include "POA.h"
#include "LocalObject.h"
#include "DynamicServant.h"

namespace CORBA {
namespace Nirvana {

template <class T, class I>
class ServantTied :
	public Servant <ServantTied <T, I>, I>
{
	typedef Servant <ServantTied <T, I>, I> BaseImpl;
public:
	ServantTied (T& t) :
		ptr_ (&t),
		rel_ (false),
		poa_ (::PortableServer::POA::_nil ())
	{}

	ServantTied (T& t, ::PortableServer::POA_ptr poa) :
		ptr_ (&t),
		rel_ (false),
		poa_ (::PortableServer::POA::_duplicate (poa))
	{}

	ServantTied (T* tp, Boolean release = true) :
		ptr_ (tp),
		rel_ (release),
		poa_ (::PortableServer::POA::_nil ())
	{}

	ServantTied (T* tp, ::PortableServer::POA_ptr poa, Boolean release = true) :
		ptr_ (tp),
		rel_ (release),
		poa_ (poa)
	{}

	virtual ~ServantTied ()
	{
		CORBA::release (poa_);
		if (rel_)
			delete ptr_;
	}

	T* _tied_object () const
	{
		return ptr_;
	}

	void _tied_object (T* obj, Boolean release = true)
	{
		if (rel_)
			delete ptr_;
		ptr_ = obj;
		rel_ = release;
	}

	Boolean _is_owner () const
	{
		return rel_;
	}

	void _is_owner (Boolean b)
	{
		rel_ = b;
	}

	::PortableServer::POA_ptr _defauilt_POA ()
	{
		if (!is_nil (poa_))
			return ::PortableServer::POA::_duplicate (poa_);
		else
			return BaseImpl::_default_POA ();
	}

	Boolean _non_existent () const
	{
		return !ptr_;
	}

	static ServantTied <T, I>& _implementation (Bridge <AbstractBase>* itf)
	{
		return BaseImpl::_implementation (itf);
	}

	static ServantTied <T, I>& _implementation (Bridge <ReferenceCounter>* itf)
	{
		return BaseImpl::_implementation (itf);
	}

	static ServantTied <T, I>& _implementation (Bridge <DynamicServant>* itf)
	{
		return BaseImpl::_implementation (itf);
	}

	static ServantTied <T, I>& _implementation (Bridge <PortableServer::ServantBase>* itf)
	{
		return BaseImpl::_implementation (itf);
	}

	static ServantTied <T, I>& _implementation (Bridge <Object>* itf)
	{
		return BaseImpl::_implementation (itf);
	}

	static ServantTied <T, I>& _implementation (Bridge <LocalObject>* itf)
	{
		return BaseImpl::_implementation (itf);
	}

	template <class I>
	static T& _implementation (Bridge <I>* bridge)
	{
		return *(BaseImpl::_implementation (bridge).ptr_);
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, const Char* id, EnvironmentBridge* env)
	{
		try {
			if (!RepositoryId::compatible (Bridge <Base>::interface_id_, id))
				::Nirvana::throw_MARSHAL ();
			return &static_cast <Bridge <Base>&> (BaseImpl::_implementation (derived));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	template <class I>
	static Bridge <I>* _duplicate (Bridge <I>* itf)
	{
		if (itf)
			BaseImpl::_implementation (itf)._add_ref ();
		return itf;
	}

	template <class I>
	static void _release (Bridge <I>* itf)
	{
		if (itf)
			BaseImpl::_implementation (itf)._remove_ref ();
	}

private:
	T* ptr_;
	Boolean rel_;
	::PortableServer::POA_ptr poa_;
};

}
}

#endif

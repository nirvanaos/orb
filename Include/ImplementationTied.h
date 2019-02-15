// Nirvana project
// Object Request Broker
// Tied interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONTIED_H_
#define NIRVANA_ORB_IMPLEMENTATIONTIED_H_

#include "Implementation.h"

namespace CORBA {
namespace Nirvana {

template <class T, class I>
class ServantTied :
	public Servant <ServantTied <T, I>, I>
{
	typedef Servant <ServantTied <T, I>, I> Base;
public:
	ServantTied (T& t) :
		ptr_ (&t),
		poa_ (POA::_nil ()),
		rel_ (false)
	{}

	ServantTied (T& t, ::PortableServer::POA_ptr poa) :
		ptr_ (&t),
		poa_ (::PortableServer::POA::_duplicate (poa)),
		rel_ (false)
	{}

	ServantTied (T* tp, Boolean release = true) :
		ptr_ (tp),
		poa_ (POA::_nil ()),
		rel_ (release)
	{}

	ServantTied (T* tp, ::PortableServer::POA_ptr poa, Boolean release = true) :
		ptr_ (tp),
		poa_ (poa),
		rel_ (release)
	{}

	~ServantTied ()
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

	POA_ptr _defauilt_POA ()
	{
		if (!is_nil (poa_))
			return PortableServer::POA::_duplicate (poa_);
		else
			return Base::_default_POA ();
	}

	Boolean _non_existent () const
	{
		return !ptr_;
	}

	static ServantTied <T, I>& _implementation (Bridge <AbstractBase>* itf)
	{
		return Base::_servant (itf);
	}

	static ServantTied <T, I>& _implementation (Bridge <ServantBase>* itf)
	{
		return Base::_servant (itf);
	}

	template <class I>
	static T& _implementation (Bridge <I>* bridge)
	{
		return *(Base::_servant (bridge).ptr_);
	}

private:
	T* ptr_;
	Boolean rel_;
	::PortableServer::POA_ptr poa_;
};


}
}

#endif

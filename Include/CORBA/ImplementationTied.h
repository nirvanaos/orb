/// \file
/// Tied interface implementation

/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
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
		rel_ (false)
	{}

	ServantTied (T& t, ::PortableServer::POA::_ptr_type poa) :
		ptr_ (&t),
		rel_ (false),
		poa_ (poa)
	{}

	ServantTied (T* tp, Boolean release = true) :
		ptr_ (tp),
		rel_ (release)
	{}

	ServantTied (T* tp, ::PortableServer::POA::_ptr_type poa, Boolean release = true) :
		ptr_ (tp),
		rel_ (release),
		poa_ (poa)
	{}

	virtual ~ServantTied ()
	{
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

	::PortableServer::POA::_ref_type _defauilt_POA ()
	{
		if (poa_)
			return poa_;
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

	template <class I1>
	static T& _implementation (Bridge <I1>* bridge)
	{
		return *(BaseImpl::_implementation (bridge).ptr_);
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, String_in id, Interface* env)
	{
		try {
			if (!RepositoryId::compatible (Bridge <Base>::repository_id_, id))
				::Nirvana::throw_MARSHAL ();
			return &static_cast <Bridge <Base>&> (BaseImpl::_implementation (derived));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	template <class Derived>
	static Bridge <Object>* _wide_object (Bridge <Derived>* derived, String_in id, Interface* env)
	{
		try {
			return BaseImpl::_implementation (derived)._get_object (id);
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	template <class I1>
	static Bridge <I1>* _duplicate (Bridge <I1>* itf)
	{
		if (itf)
			BaseImpl::_implementation (itf)._add_ref ();
		return itf;
	}

	template <class I1>
	static void _release (Bridge <I1>* itf)
	{
		if (itf)
			BaseImpl::_implementation (itf)._remove_ref ();
	}

private:
	T* ptr_;
	Boolean rel_;
	::PortableServer::POA::_ref_type poa_;
};

}
}

#endif

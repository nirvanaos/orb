/// \file
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
#ifndef NIRVANA_ORB_VALUEBOX_H_
#define NIRVANA_ORB_VALUEBOX_H_
#pragma once

#include "ValueBaseImpl.h"

namespace CORBA {
namespace Internal {

class ValueBoxBridge;

template <> struct Bridge <ValueBoxBridge>::EPV
{
	typedef Bridge <ValueBoxBridge> MyBridge;

	Interface::EPV header;
	struct
	{
		NIRVANA_BASE_ENTRY (CORBA::ValueBase, CORBA_ValueBase)
	} base;
};

template <class S>
class Skeleton <S, ValueBoxBridge>
{
public:
	static const Bridge <ValueBoxBridge>::EPV epv_;

};

template <class S>
const Bridge <ValueBoxBridge>::EPV Skeleton <S, ValueBoxBridge>::epv_ = {
	{ // header
		RepIdOf <S>::id,
		S::template __duplicate <ValueBase>,
		S::template __release <ValueBase>
	},
	{ // base
		S::template _wide_val <ValueBase, ValueBoxBridge>
	}
};

template <class S, class T>
class ValueBox :
	public ValueImpl <S, ValueBase>,
	public ValueTraits <S>,
	public Bridge <ValueBoxBridge>
{
public:
	typedef typename Type <T>::Var BoxedType;

	typedef I_ptr <S> _ptr_type;

#ifdef LEGACY_CORBA_CPP
	typedef Servant_var <S> _var_type;
	typedef _var_type& _out_type;

	// TODO: Change return type to I_var?
	NIRVANA_NODISCARD static _ptr_type _duplicate (_ptr_type obj)
	{
		return static_cast <S*> (LifeCycleRefCnt <S>::_duplicate (&obj));
	}
#else
	typedef servant_reference <S> _ref_type;
#endif

	static _ptr_type _nil () NIRVANA_NOEXCEPT
	{
		return _ptr_type (nullptr);
	}

	static _ptr_type _check (Interface* bridge)
	{
		return _wide (Interface::_check (bridge, RepIdOf <S>::id));
	}

	static _ptr_type _downcast (ValueBase::_ptr_type val)
	{
		if (val)
			return val->_query_valuetype <S> ();
		return nullptr;
	}

	const BoxedType& _value () const
	{
		return static_cast <const S&> (*this).value_;
	}

	BoxedType& _value ()
	{
		return static_cast <S&> (*this).value_;
	}

	void _value (const BoxedType& v)
	{
		static_cast <S&> (*this).value_ = v;
	}

	void _value (BoxedType&& v)
	{
		static_cast <S&> (*this).value_ = std::move (v);
	}

	void _marshal (IORequest_ptr rq) const
	{
		Type <T>::marshal_in (static_cast <const S&> (*this).value_, rq);
	}

	void _unmarshal (IORequest_ptr rq)
	{
		Type <T>::unmarshal (rq, static_cast <S&> (*this).value_);
	}

	Interface* _query_valuetype (String_in id) NIRVANA_NOEXCEPT
	{
		if (RepId::compatible (RepIdOf <S>::id, id))
			return &static_cast <Bridge <ValueBoxBridge>&> (*this);
		return nullptr;
	}

protected:
	typedef typename CORBA::Internal::Type <T>::Var _ValueType;

	ValueBox () :
		Bridge <ValueBoxBridge> (Skeleton <S, ValueBoxBridge>::epv_)
	{}

	ValueBox (const ValueBox&) :
		Bridge <ValueBoxBridge> (Skeleton <S, ValueBoxBridge>::epv_)
	{}

private:
	friend class I_ptr <Interface>;

	operator Interface* () NIRVANA_NOEXCEPT
	{
		return &static_cast <Bridge <ValueBoxBridge>&> (*this);
	}

	static _ptr_type _wide (Interface* itf) NIRVANA_NOEXCEPT
	{
		return static_cast <S*> (
			static_cast <Bridge <ValueBoxBridge>*> (itf));
	}
};

template <class I>
struct TypeValueBox : TypeValue <I>
{
	static const TCKind tc_kind = TCKind::tk_value_box;
};

}
}

#endif

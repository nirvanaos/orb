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
		S::template __duplicate <ValueBoxBridge>,
		S::template __release <ValueBoxBridge>
	},
	{ // base
		S::template _wide_val <ValueBase, ValueBoxBridge>
	}
};

class ValueBoxBridge :
	public Bridge <ValueBoxBridge>,
	public ClientInterfaceBase <ValueBoxBridge, ValueBase>
{
protected:
	ValueBoxBridge (const Bridge <ValueBoxBridge>::EPV& epv) :
		Bridge <ValueBoxBridge> (epv)
	{}
};

template <class S, class T> class ValueBoxClientEx;

template <class S, class T>
class ValueBoxClient :
	public ValueBoxBridge
{
	typedef ValueBoxClient <S, T> BoxClient;

public:
	typedef typename Type <T>::Var BoxedType;

	const BoxedType& _value () const
	{
		return static_cast <const ValueBoxClientEx <S, T>&> (*this).value_;
	}

	BoxedType& _value ()
	{
		return static_cast <ValueBoxClientEx <S, T>&> (*this).value_;
	}

	void _value (const BoxedType& v)
	{
		static_cast <ValueBoxClientEx <S, T>&> (*this).value_ = v;
	}

	void _value (BoxedType&& v)
	{
		static_cast <ValueBoxClientEx <S, T>&> (*this).value_ = std::move (v);
	}

public:
	typedef I_ptr <BoxClient> _ptr_type;

#ifdef LEGACY_CORBA_CPP
	typedef I_var <BoxClient> _var_type;
	typedef _var_type& _out_type;

	// TODO: Change return type to I_var?
	NIRVANA_NODISCARD static _ptr_type _duplicate (_ptr_type obj)
	{
		return _unsafe_cast (interface_duplicate (&obj));
	}
#else
	typedef I_ref <BoxClient> _ref_type;
#endif

	static _ptr_type _check (Interface* bridge)
	{
		return _unsafe_cast (Interface::_check (bridge, RepIdOf <S>::id));
	}

	static _ptr_type _downcast (ValueBase::_ptr_type val)
	{
		if (val)
			return val->_query_valuetype (RepIdOf <S>::id).template downcast <BoxClient> ();
		return nullptr;
	}

	static _ptr_type _nil () NIRVANA_NOEXCEPT
	{
		return _ptr_type (nullptr);
	}

protected:
	ValueBoxClient (const Bridge <ValueBoxBridge>::EPV& epv) :
		ValueBoxBridge (epv)
	{}

private:
	friend class I_ref_base <BoxClient>;
	friend class I_ptr <Interface>;
	friend class I_ref <Interface>;

	static BoxClient* _unsafe_cast (Interface* itf) NIRVANA_NOEXCEPT
	{
		assert (!itf || RepId::compatible (itf->_epv ().interface_id, RepIdOf <S>::id));
		return static_cast <BoxClient*> (itf);
	}

};

template <class S, class T>
class ValueBoxClientEx :
	public ValueBoxClient <S, T>
{
private:
	friend class ValueBoxClient <S, T>;
	typename Type <T>::Var value_;
};

template <class S, class T>
class ValueBox :
	public ValueImpl <S, ValueBase>,
	public ValueTraits <S>,
	public ValueBoxClient <S, T>
{
public:
	typedef ValueBoxClient <S, T> BoxClient;

	typedef typename BoxClient::_ptr_type _ptr_type;
	using BoxClient::_nil;

#ifdef LEGACY_CORBA_CPP
	typedef typename BoxClient::_var_type _var_type;
	typedef typename BoxClient::_out_type _out_type;

	using BoxClient::_duplicate;
	using LifeCycleRefCnt <S>::_duplicate;
#else
	typedef typename BoxClient::_ref_type _ref_type;
#endif

	using ValueTraits <S>::_copy_value;

	void _marshal (IORequest_ptr rq) const
	{
		Type <T>::marshal_in (BoxClient::_value (), rq);
	}

	void _unmarshal (IORequest_ptr rq)
	{
		Type <T>::unmarshal (rq, BoxClient::_value ());
	}

	Interface* _query_valuetype (String_in id) NIRVANA_NOEXCEPT
	{
		if (RepId::compatible (RepIdOf <S>::id, id))
			return &static_cast <Bridge <ValueBoxBridge>&> (*this);
		return nullptr;
	}

protected:
	ValueBox () :
		BoxClient (Skeleton <S, ValueBoxBridge>::epv_)
	{}

	ValueBox (const ValueBox&) :
		BoxClient (Skeleton <S, ValueBoxBridge>::epv_)
	{}

};

template <class I>
struct TypeValueBox :
	TypeItfCommon <typename I::BoxClient>,
	MarshalHelper <I, I_ref <typename I::BoxClient> >
{
	static void marshal_in (I_ptr <typename I::BoxClient> src, IORequest_ptr rq);
	static void marshal_out (I_ref <typename I::BoxClient>& src, IORequest_ptr rq);
	static void unmarshal (IORequest_ptr rq, I_ref <typename I::BoxClient>& dst);

};

// For sequences and arrays map reference type to interface type

#ifndef LEGACY_CORBA_CPP

template <class S, class T>
struct Type <I_ref <ValueBoxClient <S, T> > > : public Type <S>
{};

#else

template <class S, class T>
struct Type <I_var <ValueBoxClient <S, T> > > : public Type <S>
{};

#endif

}
}

#endif

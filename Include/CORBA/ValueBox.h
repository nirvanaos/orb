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

/// The common bridge for all value boxes.
class ValueBoxBridge : public Interface
{
public:
	/// Entry-point vector
	struct EPV
	{
		Interface::EPV header;
		struct
		{
			Bridge <ValueBase>* (*CORBA_ValueBase) (ValueBoxBridge*, Type <String>::ABI_in, Interface*);
		}
		base;
	};

	const EPV& _epv () const NIRVANA_NOEXCEPT
	{
		return (const EPV&)Interface::_epv ();
	}

	operator I_ptr <ValueBase> ()
	{
		Environment env;
		return static_cast <ValueBase*> (this->_get_bridge_ptr (env));
	}

protected:
	Bridge <ValueBase>* _get_bridge_ptr (EnvironmentBase& env)
	{
		Bridge <ValueBase>* ret = (_epv ().base.CORBA_ValueBase) (this, &StringView <Char> (RepIdOf <ValueBase>::id), &env);
		env.check ();
		return ret;
	}

	Bridge <ValueBase>& _get_bridge (EnvironmentBase& env)
	{
		Bridge <ValueBase>* ret = _get_bridge_ptr (env);
		if (!ret)
			::Nirvana::throw_INV_OBJREF ();
		return *ret;
	}

protected:
	ValueBoxBridge (const EPV& epv) NIRVANA_NOEXCEPT :
		Interface (epv.header)
	{}
};

/// Value box client part.
/// 
/// \tparam T Boxed type.
/// 
/// The boxed value always follow immediately to ValueBoxBridge
/// (with possible alignment).
/// 
/// The memory layout:
///
/// +----------------------------+
/// | ValueBoxBridge             |
/// +----------------------------+
/// | The boxed value            |
/// +----------------------------+
/// | Reference counter etc.     |
/// | Implementation dependent.  |
/// | Not accessed directly.     |
/// +----------------------------+
///
template <typename T>
class ValueBoxClient : public ValueBoxBridge
{
public:
	typedef typename Type <T>::Var BoxedType;

	const BoxedType& _value () const NIRVANA_NOEXCEPT
	{
		return *reinterpret_cast <const BoxedType*> (&value_);
	}

	BoxedType& _value () NIRVANA_NOEXCEPT
	{
		return *reinterpret_cast <BoxedType*> (&value_);
	}

	void _value (const BoxedType& v)
	{
		_value () = v;
	}

	void _value (BoxedType&& v) NIRVANA_NOEXCEPT
	{
		_value () = std::move (v);
	}

protected:
	ValueBoxClient& operator = (const ValueBoxClient& v)
	{
		_value () = v._value ();
		return *this;
	}

	ValueBoxClient& operator = (ValueBoxClient&& v) NIRVANA_NOEXCEPT
	{
		_value () = std::move (v._value ());
		return *this;
	}

protected:
	ValueBoxClient (const EPV& epv) NIRVANA_NOEXCEPT :
		ValueBoxBridge (epv)
	{}

	ValueBoxClient (const EPV& epv, const BoxedType& src) NIRVANA_NOEXCEPT :
		ValueBoxBridge (epv)
	{
		::new (&value_) BoxedType (src);
	}

	ValueBoxClient (const EPV& epv, BoxedType&& src) NIRVANA_NOEXCEPT :
		ValueBoxBridge (epv)
	{
		::new (&value_) BoxedType (std::move (src));
	}

protected:
	std::aligned_storage_t <sizeof (BoxedType), alignof (BoxedType)> value_;
};

/// Value box implementation
/// 
/// \tparam VB Value box type, derived from this class.
/// \tparam T Boxed type.
template <class VB, typename T>
class ValueBoxImpl :
	public ValueBoxClient <T>,
	public RefCountBase <VB>,
	private LifeCycleRefCnt <VB>,
	public Skeleton <VB, ValueBase>,
	public ValueTraits <VB>
{
	friend class Skeleton <VB, ValueBase>;
	friend class LifeCycleRefCnt <VB>;
	friend class LifeCycleDynamic <VB>;

	typedef ValueBoxClient <T> Base;
	typedef ValueBoxImpl <VB, T> ThisClass;
public:
	typedef I_ptr <VB> _ptr_type;

#ifdef LEGACY_CORBA_CPP
	typedef I_var <VB> _var_type;
	typedef _var_type& _out_type;

	// TODO: Change return type to I_var?
	NIRVANA_NODISCARD static _ptr_type _duplicate (_ptr_type obj)
	{
		return _unsafe_cast (interface_duplicate (&obj));
	}
	using LifeCycleRefCnt <VB>::_duplicate;
#else
	typedef I_ref <VB> _ref_type;
#endif

	static _ptr_type _check (Interface* bridge)
	{
		return _unsafe_cast (Interface::_check (bridge, RepIdOf <VB>::id));
	}

	static _ptr_type _downcast (ValueBase::_ptr_type val)
	{
		if (val)
			return val->_query_valuetype (RepIdOf <VB>::id).template downcast <VB> ();
		return nullptr;
	}

	static _ptr_type _nil () NIRVANA_NOEXCEPT
	{
		return _ptr_type (nullptr);
	}

protected:
	ValueBoxImpl () NIRVANA_NOEXCEPT :
		Base (epv_),
		base_ (Skeleton <VB, ValueBase>::epv_)
	{}

	ValueBoxImpl (const ValueBoxImpl& src) NIRVANA_NOEXCEPT :
		Base (epv_, src._value ()),
		base_ (Skeleton <VB, ValueBase>::epv_)
	{}

	ValueBoxImpl (ValueBoxImpl&& src) NIRVANA_NOEXCEPT :
		Base (epv_, std::move (src._value ())),
		base_ (Skeleton <VB, ValueBase>::epv_)
	{}

	ValueBoxImpl& operator = (const ValueBoxImpl& src)
	{
		Base::operator = (src);
		return *this;
	}

	ValueBoxImpl& operator = (ValueBoxImpl&& src) NIRVANA_NOEXCEPT
	{
		Base::operator = (std::move (src));
		return *this;
	}

	static Interface* __truncatable_base (Bridge <ValueBase>* _b, Interface* _env)
	{
		return nullptr;
	}

private:
	static Bridge <ValueBase>* _CORBA_ValueBase (ValueBoxBridge* bridge, Type <String>::ABI_in id, Interface* env)
	{
		if (!RepId::compatible (RepIdOf <ValueBase>::id, Type <String>::in (id)))
			::Nirvana::throw_INV_OBJREF ();
		check_pointer (bridge, epv_.header);
		return &static_cast <ValueBox <VB, T>&> (*bridge).base_;
	}

	void _marshal (IORequest_ptr rq) const
	{
		Type <T>::marshal_in (this->_value (), rq);
	}

	void _unmarshal (IORequest_ptr rq)
	{
		Type <T>::unmarshal (rq, this->_value ());
	}

	Interface* _query_valuetype (String_in id) NIRVANA_NOEXCEPT
	{
		if (RepId::compatible (RepIdOf <VB>::id, id))
			return &static_cast <ValueBoxBridge&> (*this);
		return nullptr;
	}

	static ThisClass& _implementation (Bridge <ValueBase>* bridge)
	{
		check_pointer (bridge, Skeleton <VB, ValueBase>::epv_.header);
		return *reinterpret_cast <ThisClass*>
			(reinterpret_cast <uint8_t*> (bridge) - (uintptr_t) & (((ThisClass*)0)->base_));
	}

	static ThisClass& _implementation (Bridge <VB>* bridge)
	{
		check_pointer (bridge, epv_.header);
		return reinterpret_cast <ThisClass&> (*bridge);
	}

private:
	friend class I_ptr <Interface>;
	friend class I_ref <Interface>;
	friend class I_ref_base <VB>;

	static VB* _unsafe_cast (Interface* itf) NIRVANA_NOEXCEPT
	{
		assert (!itf || RepId::compatible (itf->_epv ().interface_id, RepIdOf <VB>::id));
		return static_cast <VB*> (itf);
	}

private:
	static const ValueBoxBridge::EPV epv_;

	class ValueBaseBridge : public BridgeVal <ValueBase>
	{
	public:
		ValueBaseBridge (const Bridge <ValueBase>::EPV& epv) NIRVANA_NOEXCEPT :
			BridgeVal <ValueBase> (epv)
		{}
	};

	ValueBaseBridge base_;
};

template <class VB, typename T>
const ValueBoxBridge::EPV ValueBoxImpl <VB, T>::epv_ = {
	{ // header
		RepIdOf <VB>::id,
		VB::template __duplicate <VB>,
		VB::template __release <VB>
	},
	{ // base
		_CORBA_ValueBase
	}
};

/// The value box class.
/// 
/// Adds derived ValueBase client API to the value box implementation.
/// 
/// \tparam VB Value box type, derived from this class.
/// \tparam T Boxed type.
template <class VB, typename T>
class ValueBox : public Client <ValueBoxImpl <VB, T>, ValueBase>
{};

template <class I>
using TypeValueBox = TypeValue <I>;

}
}

#endif

/// \file
/// Client implementation templates.
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

#ifndef NIRVANA_ORB_CLIENT_H_
#define NIRVANA_ORB_CLIENT_H_
#pragma once

#include "Environment.h"

namespace CORBA {
namespace Internal {

template <class T, class I> class Client;

template <class I> struct NativeDecls
{};

template <class I> struct Decls : NativeDecls <I>
{};

template <class I>
class ClientBridge :
	public Bridge <I>
{
protected:
	Bridge <I>& _get_bridge (Environment&) noexcept
	{
		return *this;
	}
};

//! Primary interface client implementation.
template <class I>
class ClientInterfacePrimary :
	public Client <ClientBridge <I>, I>
{
public:
	typedef I_ptr <I> _ptr_type;

#ifdef LEGACY_CORBA_CPP
	typedef I_var <I> _var_type;
	typedef _var_type& _out_type;

	NIRVANA_NODISCARD static _ptr_type _duplicate (_ptr_type obj)
	{
		return _unsafe_cast (interface_duplicate (&obj));
	}
#else
	typedef I_ref <I> _ref_type;
#endif

	static _ptr_type _nil () noexcept
	{
		return _ptr_type (nullptr);
	}

	static _ptr_type _check (Interface* bridge)
	{
		return _unsafe_cast (Interface::_check (bridge, RepIdOf <I>::id));
	}

private:
	friend class I_ptr <Interface>;
	friend class I_ref <Interface>;
	friend class I_ref_base <I>;

	static I* _unsafe_cast (Interface* itf) noexcept
	{
		assert (!itf || RepId::compatible (itf->_epv ().interface_id, RepIdOf <I>::id));
		return static_cast <I*> (itf);
	}
};

/// Obtain pointer to bridge from primary interface.
/// No conversion to I_ptr <Base>.
template <class Primary, class Base>
class ClientBaseNoPtr
{
protected:
	Bridge <Base>* _get_bridge_ptr (Environment& env)
	{
		Primary& t = static_cast <Primary&> (*this);
		typename Bridge <Primary>:: template Wide <Base>::Func func = t._epv ().base;
		Bridge <Base>* ret = (func)(&t, &StringView <Char> (RepIdOf <Base>::id), &env);
		env.check ();
		return ret;
	}

	Bridge <Base>& _get_bridge (Environment& env)
	{
		Bridge <Base>* ret = _get_bridge_ptr (env);
		if (!ret)
			::Nirvana::throw_INV_OBJREF ();
		return *ret;
	}
};

/// Add conversion to I_ptr <Base>
template <class Primary, class Base>
class ClientBase : public ClientBaseNoPtr <Primary, Base>
{
public:
	operator I_ptr <Base> ()
	{
		Environment env;
		return static_cast <Base*> (this->_get_bridge_ptr (env));
	}
};

/// Base interface client implementation.
/// Has specializations for Object and ValueBase.
template <class Primary, class I>
class ClientInterfaceBase :
	public Client <ClientBase <Primary, I>, I>
{};

/// Base for client interface.
template <class Primary, class ... Bases>
class ClientInterface :
	public ClientInterfacePrimary <Primary>,
	public ClientInterfaceBase <Primary, Bases>...
{};

/// Support interface client implementation.
template <class Primary, class I>
class ClientInterfaceSupport :
	public Client <ClientBaseNoPtr <Primary, I>, I>
{};

template <class Primary, class ... Intfs>
class ClientSupports :
	public ClientInterfaceSupport <Primary, Intfs>...
{};

template <class I> inline
const AMI_EPV <I>& AMI_epv (Bridge <I>& bridge)
{
	assert (bridge._epv ().ami.epv);
	return *bridge._epv ().ami.epv;
}

}
}

#endif

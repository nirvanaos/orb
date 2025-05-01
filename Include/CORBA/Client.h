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

template <class T, class Itf> class Client;

template <class Itf> struct NativeDecls
{};

template <class Itf> struct Decls : NativeDecls <Itf>
{};

template <class Itf>
class ClientBridge :
	public Bridge <Itf>
{
protected:
	Bridge <Itf>& _get_bridge (Environment&) noexcept
	{
		return *this;
	}

	const Bridge <Itf>& _get_bridge (Environment&) const noexcept
	{
		return *this;
	}

};

//! Primary interface client implementation.
template <class Itf>
class ClientInterfacePrimary :
	public Client <ClientBridge <Itf>, Itf>
{
public:
	typedef I_ptr <Itf> _ptr_type;

#ifdef LEGACY_CORBA_CPP
	typedef I_var <Itf> _var_type;
	typedef _var_type& _out_type;

	NIRVANA_NODISCARD static _ptr_type _duplicate (_ptr_type obj)
	{
		return _unsafe_cast (interface_duplicate (&obj));
	}
#else
	typedef I_ref <Itf> _ref_type;
#endif

	static _ptr_type _nil () noexcept
	{
		return _ptr_type (nullptr);
	}

	static _ptr_type _check (Interface* bridge)
	{
		return _unsafe_cast (Interface::_check (bridge, RepIdOf <Itf>::id));
	}

private:
	static Itf* _unsafe_cast (Interface* itf) noexcept
	{
		return I_ptr <Interface> (itf).template downcast <Itf> ();
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

	const Bridge <Base>& _get_bridge (Environment& env) const
	{
		return const_cast <ClientBaseNoPtr <Primary, Base>&> (*this)._get_bridge (env);
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
template <class Primary, class Itf>
class ClientInterfaceBase :
	public Client <ClientBase <Primary, Itf>, Itf>
{};

/// Base for client interface.
template <class Primary, class ... Bases>
class ClientInterface :
	public ClientInterfacePrimary <Primary>,
	public ClientInterfaceBase <Primary, Bases>...
{};

/// Support interface client implementation.
template <class Primary, class Itf>
class ClientInterfaceSupport :
	public Client <ClientBaseNoPtr <Primary, Itf>, Itf>
{};

template <class Primary, class ... Intfs>
class ClientSupports :
	public ClientInterfaceSupport <Primary, Intfs>...
{};

template <class Itf> inline
const AMI_EPV <Itf>& AMI_epv (Bridge <Itf>& bridge)
{
	assert (bridge._epv ().ami.epv);
	return *bridge._epv ().ami.epv;
}

}
}

#endif

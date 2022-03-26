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
#ifndef NIRVANA_ORB_IMPLEMENTATIONVALUE_H_
#define NIRVANA_ORB_IMPLEMENTATIONVALUE_H_
#pragma once

#include "ValueBaseImpl.h"
#include "ServantBaseImpl.h"
#include "LocalObjectImpl.h"
#include "FindInterface.h"

namespace CORBA {
namespace Internal {

template <class S, class Primary, class ... Bases>
class ImplementationValue :
	public InterfaceImpl <S, Primary>,
	public InterfaceImpl <S, Bases>...,
	public InterfaceImpl <S, ValueBase>,
	public InterfaceImpl <S, AbstractBase>
{
public:
	typedef Primary PrimaryInterface;

	Interface* _query_interface (String_in id) NIRVANA_NOEXCEPT
	{
		return FindInterface <Primary, Bases...>::find (static_cast <S&> (*this), id);
	}
};

template <class S, class Primary, class ... Bases>
class ImplementationValObject :
	public InterfaceImpl <S, Primary>,
	public InterfaceImpl <S, Bases>...,
	public InterfaceImplBase <S, ValueBase>,
	public InterfaceImpl <S, PortableServer::ServantBase>
{
public:
	typedef Primary PrimaryInterface;

	Interface* _query_interface (String_in id) NIRVANA_NOEXCEPT
	{
		return FindInterface <Primary, Bases...>::find (static_cast <S&> (*this), id);
	}
};

template <class S, class Primary, class ... Bases>
class ImplementationValLocal :
	public InterfaceImpl <S, Primary>,
	public InterfaceImpl <S, Bases>...,
	public InterfaceImplBase <S, ValueBase>,
	public InterfaceImpl <S, LocalObject>
{
public:
	typedef Primary PrimaryInterface;

	Interface* _query_interface (String_in id) NIRVANA_NOEXCEPT
	{
		return FindInterface <Primary, Bases...>::find (static_cast <S&> (*this), id);
	}
};

template <class T>
class ValueBox :
	public InterfaceImpl <ValueBox <T>, ValueBase>
{
public:
	typename Type <T>::ConstRef _value () const
	{
		return val_;
	}

	T& _value ()
	{
		return val_;
	}

	void _value (typename Type <T>::ConstRef v)
	{
		val_ = v;
	}

private:
	T val_;
};

}
}

#endif

/// \file Bridge.h

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
#ifndef NIRVANA_ORB_BRIDGE_H_
#define NIRVANA_ORB_BRIDGE_H_
#pragma once

#include "Interface.h"
#include "StringView.h"

namespace CORBA {
namespace Internal {

// Use inline anonimous namespace to avoid linker errors "duplicated symbol".
inline namespace {

/// Bind repository id to a type.
/// \tparam T Type
template <typename T>
struct RepIdOf
{
	/// The repository id of type T.
	static const Char id [];
};

}

/// The ABI for a particular interface.
/// A "bridge" between the client and servant sides.
///
/// \tparam I An interface.
template <class I>
class Bridge :
	public Interface
{
public:
	/// Entry-point vector
	struct EPV;

	const EPV& _epv () const noexcept
	{
		return (const EPV&)Interface::_epv ();
	}

	/// Helper for widening to a base interface
	template <class Base>
	struct Wide
	{
		typedef Bridge <Base>* (*Func) (Bridge <I>*, const ABI <String>* base_id, Interface* environment);
	};

public:
	constexpr Bridge (const EPV& epv) noexcept :
		Interface (epv.header)
	{
		static_assert (offsetof(EPV, header) == 0, "header must be at the beginning of EPV.");
	}
};

/// The bridge for value types.
template <class I>
class BridgeVal :
	public Bridge <I>
{
public:
	constexpr BridgeVal (const typename Bridge <I>::EPV& epv) noexcept :
		Bridge <I> (epv)
	{}
};

/// The entry-point vector for AMI
/// \tparam I Interface class.
template <class I> struct AMI_EPV;

template <class S, class I> class AMI_Skeleton;

template <class S, class I>
class AMI_Servant
{
public:
	static const AMI_EPV <I>* const ami_epv_;
};

template <class S, class I>
const AMI_EPV <I>* const AMI_Servant <S, I>::ami_epv_ = nullptr;

}
}

#define NIRVANA_BASE_ENTRY(type, name) MyBridge::Wide <type>::Func name;\
operator const MyBridge::Wide < type>::Func () const { return name; }

#define NIRVANA_BRIDGE_BEGIN(I) template <> struct Bridge < I>::EPV { typedef Bridge <I> MyBridge; Interface::EPV header; struct {
#define NIRVANA_BRIDGE_EPV } base; struct {
#define NIRVANA_BRIDGE_END() } epv;};

#define CORBA_REPOSITORY_ID(t) "IDL:omg.org/CORBA/" t ":1.0"
#define NIRVANA_REPOSITORY_ID(t) "IDL:Nirvana/" t ":1.0"
#define CORBA_INTERNAL_REPOSITORY_ID(t) "IDL:CORBA/Internal/" t ":1.0"
#define PORTABLESERVER_REPOSITORY_ID(t) "IDL:omg.org/PortableServer/" t ":1.0"

#define NIRVANA_AMI_BEGIN(I) template <> struct AMI_EPV < I> {
#define NIRVANA_AMI_END() };

#define NIRVANA_BRIDGE_AMI_EPV(I) } base; struct { const AMI_EPV <I>* epv; } ami; struct {
#define NIRVANA_BRIDGE_AMI_INIT(S, I) AMI_Servant <S, I>::ami_epv_

#define NIRVANA_IMPLEMENT_AMI(S, I) template <> const AMI_EPV <I>* const AMI_Servant <S, I>::ami_epv_ = &AMI_Skeleton <S, I>::epv_;

#endif

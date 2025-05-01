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
#ifndef NIRVANA_ORB_SERVANTSTATIC_H_
#define NIRVANA_ORB_SERVANTSTATIC_H_
#pragma once

#include "ServantImpl.h"
#include "I_ptr.h"
#include <Nirvana/ImportInterface.h>

namespace CORBA {
namespace Internal {

template <class S, class Itf> class ServantStatic;

//! Static servant traits
template <class S>
class ServantTraitsStatic :
	public ServantTraits <S>
{
public:
	template <class Itf>
	static S& _implementation (Bridge <Itf>* bridge) noexcept
	{
		return *(S*)nullptr;
	}
};

template <class S>
class ServantTraitsStaticEx :
	public ServantTraits <S>
{
public:
	template <class Itf>
	static S _implementation (Bridge <Itf>* bridge)
	{
		return S ();
	}
};

template <class S, class Itf>
class InterfaceStaticBase :
	public Skeleton <S, Itf>
{
public:
	constexpr operator Bridge <Itf>& () const noexcept
	{
		return *_bridge ();
	}

	constexpr static Bridge <Itf>* _bridge () noexcept
	{
		return const_cast <Bridge <Itf>*> (&bridge_);
	}

	static constexpr Bridge <Itf> bridge_{ Skeleton <S, Itf>::epv_ };
};

#define NIRVANA_STATIC_BRIDGE(Itf, ...) ::CORBA::Internal::InterfaceStaticBase <__VA_ARGS__, Itf>::_bridge ()

//template <class S, class Itf>
//const Bridge <Itf> InterfaceStaticBase <S, Itf>::bridge_ (InterfaceStaticBase <S, Itf>::epv_);

template <class S, class Itf>
class InterfaceStatic :
	public InterfaceStaticBase <S, Itf>
{};

class ServantStaticDummy
{
public:
	static void _add_ref () noexcept
	{
	}

	static void _remove_ref () noexcept
	{
	}

	static ULong _refcount_value () noexcept
	{
		return 1;
	}

	static Boolean _non_existent () noexcept
	{
		return false;
	}

	static Bridge <AbstractBase>* _get_abstract_base (Type <String>::ABI_in iid,
		Interface* env) noexcept
	{
		return nullptr;
	}

	static void __add_ref (Interface* obj, Interface* env) noexcept;
	static void __remove_ref (Interface* obj, Interface* env) noexcept;
	static ULong __refcount_value (Interface* obj, Interface* env) noexcept;
	static void __delete_object (Interface* _b, Interface* _env) noexcept;
};

// Use inline anonimous namespace to avoid linker errors "duplicated symbol".
inline namespace {

template <class S>
struct StaticId
{
	static const char id [];
};

}

}
}

#if defined (_MSC_VER) && !defined (__clang__)

#define NIRVANA_EXPORT_OBJECT(uname, Impl)\
extern "C" NIRVANA_STATIC_IMPORT void* uname = &Impl::export_struct_;\
__pragma(comment (linker, "/include:" C_NAME_PREFIX #uname))

#else

#define NIRVANA_EXPORT_OBJECT(uname, Impl) NIRVANA_STATIC_IMPORT void* __attribute__ ((used)) uname = &Impl::export_struct_;

#endif

#endif

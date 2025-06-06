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
#ifndef NIRVANA_ORB_LOCALOBJECTSTATIC_H_
#define NIRVANA_ORB_LOCALOBJECTSTATIC_H_
#pragma once

#include <Nirvana/NirvanaBase.h>
#include <Nirvana/OLF.h>
#include "LocalObject_s.h"
#include "local_core.h"
#include "ServantStatic.h"
#include "LifeCycleStatic.h"

namespace CORBA {
namespace Internal {

class LocalObjectStaticDummy : public ServantStaticDummy
{
public:
	static void __add_ref (Bridge <LocalObject>* obj, Interface* env) noexcept
	{
		ServantStaticDummy::__add_ref (obj, env);
	}

	static void __remove_ref (Bridge <LocalObject>* obj, Interface* env) noexcept
	{
		ServantStaticDummy::__remove_ref (obj, env);
	}

	static ULong __refcount_value (Bridge <LocalObject>* obj, Interface* env) noexcept
	{
		return ServantStaticDummy::__refcount_value (obj, env);
	}

	static void __delete_object (Bridge <LocalObject>* obj, Interface* env) noexcept
	{
		ServantStaticDummy::__delete_object (obj, env);
	}
};

//! Static implementation of LocalObject
//! \tparam S Servant class.
//! \tparam Primary Primary interface.
template <class S>
class InterfaceStatic <S, LocalObject> :
	public InterfaceStaticBase <S, LocalObject>,
	public ServantTraitsStatic <S>,
	public LifeCycleStatic,
	public LocalObjectStaticDummy
{
public:
	using LocalObjectStaticDummy::__add_ref;
	using LocalObjectStaticDummy::__remove_ref;
	using LocalObjectStaticDummy::__refcount_value;
	using LocalObjectStaticDummy::__delete_object;

	static Bridge <Object>* _get_object (Type <String>::ABI_in iid, Interface* env) noexcept
	{
		return get_object_from_core (core_object (), iid, env);
	}

protected:
	/// \brief Obtain `Object` interface pointer.
	/// 
	/// \returns `Object` interface from proxy.
	/// 
	/// Unlike the `_this()` method, `_object()` does not increment reference counter.
	/// It is intended for use inside servant implementation code only.
	/// 
	static Object::_ptr_type _object ()
	{
		Environment env;
		Object::_ptr_type ret (static_cast <Object*> (_get_object (&Type <IDL::String>::C_in (RepIdOf <Object>::id), &env)));
		env.check ();
		if (!ret)
			throw MARSHAL ();
		return ret;
	}

	static Type <Interface>::VRet _get_proxy ()
	{
#ifdef LEGACY_CORBA_CPP
		return interface_duplicate (&get_proxy (core_object ()));
#else
		return get_proxy (core_object ());
#endif
	}

private:
	static LocalObject::_ptr_type core_object () noexcept
	{
		return static_cast <LocalObject*> (export_struct_.core_object);
	}

public:
	NIRVANA_OLF_SECTION NIRVANA_STATIC_IMPORT static constexpr ::Nirvana::ExportObject export_struct_{
	::Nirvana::OLF_EXPORT_LOCAL, StaticId <S>::id,
	NIRVANA_STATIC_BRIDGE (LocalObject, S) };
};

}
}

#endif

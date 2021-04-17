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
#ifndef NIRVANA_ORB_OBJECTFACTORY_S_H_
#define NIRVANA_ORB_OBJECTFACTORY_S_H_

#include "ObjectFactory.h"
#include "ImplementationPseudo.h"
#include "ImplementationPseudoStatic.h"

// ObjectFactory skeleton

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ObjectFactory>
{
public:
	static const typename Bridge <ObjectFactory>::EPV epv_;

protected:
	static void* _memory_allocate (Bridge <ObjectFactory>* _b, size_t size, Interface* _env)
	{
		try {
			return S::_implementation (_b).memory_allocate (size);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}

	static void _memory_release (Bridge <ObjectFactory>* _b, void* p, size_t size, Interface* _env)
	{
		try {
			S::_implementation (_b).memory_release (p, size);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _stateless_begin (Bridge <ObjectFactory>* _b, Type <StatelessCreationFrame>::ABI_out scs, Interface* _env)
	{
		try {
			S::_implementation (_b).stateless_begin (Type <StatelessCreationFrame>::inout (scs));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void* _stateless_end (Bridge <ObjectFactory>* _b, Type <Boolean>::ABI_in success, Interface* _env)
	{
		try {
			return S::_implementation (_b).stateless_end (Type <bool>::in (success));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}

	static Interface* _create_reference_counter (Bridge <ObjectFactory>* _b, Interface* dynamic, Interface* _env)
	{
		try {
			return Type <ReferenceCounter>::ret (S::_implementation (_b).create_reference_counter (Type <DynamicServant>::in (dynamic)));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}

	static Interface* _create_servant (Bridge <ObjectFactory>* _b, Interface* impl, Interface* _env)
	{
		try {
			return Type <PortableServer::ServantBase>::ret (S::_implementation (_b).create_servant (Type <PortableServer::ServantBase>::in (impl)));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}

	static Interface* _create_local_object (Bridge <ObjectFactory>* _b, Interface* impl, Interface* ab, Interface* _env)
	{
		try {
			return Type <LocalObject>::ret (S::_implementation (_b).create_local_object (Type <LocalObject>::in (impl), Type <AbstractBase>::in (ab)));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}
};

template <class S>
const Bridge <ObjectFactory>::EPV Skeleton <S, ObjectFactory>::epv_ = {
	{ // interface
		Bridge <ObjectFactory>::repository_id_,
		S::template __duplicate <ObjectFactory>,
		S::template __release <ObjectFactory>
	},
	{ // epv
		S::_memory_allocate,
		S::_memory_release,
		S::_stateless_begin,
		S::_stateless_end,
		S::_create_reference_counter,
		S::_create_servant,
		S::_create_local_object
	}
};

// Standard implementation

template <class S>
class Servant <S, ObjectFactory> : public ImplementationPseudo <S, ObjectFactory>
{};

// Static implementation

template <class S>
class ServantStatic <S, ObjectFactory> : public ImplementationPseudoStatic <S, ObjectFactory>
{};

}
}

#endif

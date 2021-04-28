/// \file
/// ObjectFactory Interface to the core object functionality.

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
#ifndef NIRVANA_ORB_OBJECTFACTORY_H_
#define NIRVANA_ORB_OBJECTFACTORY_H_

#include "LocalObject.h"
#include "ServantBase.h"

namespace CORBA {
namespace Nirvana {

class DynamicServant;
#ifdef LEGACY_CORBA_CPP
typedef I_ptr <DynamicServant> DynamicServant_ptr;
typedef I_var <DynamicServant> DynamicServant_var;
#endif

class ObjectFactory;
#ifdef LEGACY_CORBA_CPP
typedef I_ptr <ObjectFactory> ObjectFactory_ptr;
typedef I_var <ObjectFactory> ObjectFactory_var;
#endif

template <>
struct Definitions <ObjectFactory>
{
	class StatelessCreationFrame
	{
	public:
		StatelessCreationFrame () :
			_tmp (),
			_size (),
			_offset ()
		{}

		explicit StatelessCreationFrame (::Nirvana::ConstPointer tmp, ::Nirvana::Size size, ::Nirvana::PtrDiff offset) :
			_tmp (tmp),
			_size (size),
			_offset (offset)
		{}

		::Nirvana::ConstPointer tmp () const
		{
			return _tmp;
		}
		void tmp (::Nirvana::ConstPointer val)
		{
			_tmp = val;
		}

		::Nirvana::Size size () const
		{
			return _size;
		}
		void size (::Nirvana::Size val)
		{
			_size = val;
		}

		::Nirvana::PtrDiff offset () const
		{
			return _offset;
		}
		void offset (::Nirvana::PtrDiff val)
		{
			_offset = val;
		}

	private:
		Type < ::Nirvana::ConstPointer>::Var _tmp; ///< Pointer to the servant temporary location in stack
		Type < ::Nirvana::Size>::Var _size; ///< Servant size
		Type < ::Nirvana::PtrDiff>::Var _offset; ///< Offset to the stateless memory block
	};
};

template <>
struct Type <Definitions <ObjectFactory>::StatelessCreationFrame> :
	public TypeByRef <Definitions <ObjectFactory>::StatelessCreationFrame>
{};

NIRVANA_BRIDGE_BEGIN (ObjectFactory, CORBA_NIRVANA_REPOSITORY_ID ("ObjectFactory"))
void* (*memory_allocate) (Bridge <ObjectFactory>*, size_t size, Interface*);
void (*memory_release) (Bridge <ObjectFactory>*, void* p, size_t size, Interface*);
void (*stateless_begin) (Bridge <ObjectFactory>*, Type <Definitions <ObjectFactory>::StatelessCreationFrame>::ABI_out, Interface*);
void* (*stateless_end) (Bridge <ObjectFactory>*, Type <Boolean>::ABI_in success, Interface*);
Interface* (*create_reference_counter) (Bridge <ObjectFactory>*, Interface*, Interface*);
Interface* (*create_servant) (Bridge <ObjectFactory>*, Interface*, Interface*);
Interface* (*create_local_object) (Bridge <ObjectFactory>*, Interface*, Interface*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ObjectFactory> :
	public T,
	public Definitions <ObjectFactory>
{
public:
	/// \brief Allocates memory for servant.
	///
	/// If we currently run out of SD, this operation creates a new
	/// SD and enters into it. Then it allocates memory from the current SD heap.
	/// This operation is not called for stateless servants.
	///
	/// \param size Amount of memory in bytes.
	void* memory_allocate (size_t size);

	/// \brief Releases servant memory.
	///
	/// This method detects when memory is stateless and releases it properly.
	/// \param p Servant memory.
	/// \param size Amount of memory in bytes.
	void memory_release (void* p, size_t size);

	void stateless_begin (Type <Definitions <ObjectFactory>::StatelessCreationFrame>::C_inout scb);
	void* stateless_end (Type <Boolean>::C_in success);

	ReferenceCounter::_ref_type create_reference_counter (I_in <DynamicServant> dynamic);
	PortableServer::ServantBase::_ref_type create_servant (I_in <PortableServer::ServantBase> impl);
	LocalObject::_ref_type create_local_object (I_in <LocalObject> impl, I_in <AbstractBase> abstract_base);
};

template <class T>
void* Client <T, ObjectFactory>::memory_allocate (size_t size)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	void* _ret = (_b._epv ().epv.memory_allocate) (&_b, size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ObjectFactory>::memory_release (void* p, size_t size)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.memory_release) (&_b, p, size, &_env);
	_env.check ();
}

template <class T>
void Client <T, ObjectFactory>::stateless_begin (Type <Definitions <ObjectFactory>::StatelessCreationFrame>::C_inout scs)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.stateless_begin) (&_b, &scs, &_env);
	_env.check ();
}

template <class T>
void* Client <T, ObjectFactory>::stateless_end (Type <Boolean>::C_in success)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	void* _ret = (_b._epv ().epv.stateless_end) (&_b, &success, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ReferenceCounter::_ref_type Client <T, ObjectFactory>::create_reference_counter (I_in <DynamicServant> dynamic)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <ReferenceCounter> _ret = (_b._epv ().epv.create_reference_counter) (&_b, &dynamic, &_env);
	_env.check ();
	return _ret;
}

template <class T>
PortableServer::ServantBase::_ref_type Client <T, ObjectFactory>::create_servant (I_in <PortableServer::ServantBase> impl)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <PortableServer::ServantBase> _ret = (_b._epv ().epv.create_servant) (&_b, &impl, &_env);
	_env.check ();
	return _ret;
}

template <class T>
LocalObject::_ref_type Client <T, ObjectFactory>::create_local_object (I_in <LocalObject> impl, I_in <AbstractBase> abstract_base)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <LocalObject> _ret = (_b._epv ().epv.create_local_object) (&_b, &impl, &abstract_base, &_env);
	_env.check ();
	return _ret;
}

class ObjectFactory : public ClientInterface <ObjectFactory>
{
public:
	using Definitions <ObjectFactory>::StatelessCreationFrame;
};

}
}

#endif

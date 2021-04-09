/// \file Marshal.h
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

/*
~~~{.idl}
module CORBA {
module Nirvana {

/// Marshals data between objects in the same platform domain.
pseudo interface Marshal
{
  /// Returns marshal context.
	/// Used for proxy code optimization.
  attribute readonly MarshalContext context;

  /// \brief Marshals memory block to the target memory space.
  ///
  /// \param          p    A memory block.
  /// \param [in,out] size The size of memory block.
  ///                      On return, contains the size of memory block allocated in the target memory space.
  ///                      If size = 0 on return, the memory block is owned by the marshaling system
  ///                      and mustn't be released.
  ///                      If size if not zero, the memory block may be adopted by the message recipient
  ///                      on the other side by `Unmarshal::adopt_memory()` call.
  /// \param release_size  If != 0 then source memory block at address p with size release_size must be released
  ///                      after marshaling.
  /// \return Address of the memory block at target memory space.
  ::Nirvana::UIntPtr marshal_memory (::Nirvana::ConstPointer p, inout :Nirvana::Size size, :Nirvana::Size release_size);

  /// \brief Allocates the uninitialized memory block and map it to the target memory space.
  ///   The caller gets pointer to the block and fill it with data.
  ///
  /// \param [in,out] size    The size of the memory block. See the `marshal_memory`.
  /// \param [out]    buf_ptr The buffer pointer in current memory space.
  ///
  /// \return Address of the memory block at target memory space.
  ::Nirvana::UIntPtr get_buffer (inout :Nirvana::Size size, out ::Nirvana::Pointer buf_ptr);

  ::Nirvana::UIntPtr marshal_object (Object obj);
  ::Nirvana::UIntPtr marshal_type_code (TypeCode tc);
};

}
}
~~~
*/
#ifndef NIRVANA_ORB_MARSHAL_H_
#define NIRVANA_ORB_MARSHAL_H_

#include "../Client.h"
#include "MarshalContext.h"

namespace CORBA {
namespace Nirvana {

class Marshal;
typedef I_ptr <Marshal> Marshal_ptr;
typedef I_var <Marshal> Marshal_var;
typedef I_out <Marshal> Marshal_out;

/// \interface Marshal
///
/// \brief Controls marshaling between different protection domains on the one system.

template <class T>
class Client <T, Marshal> :
	public T
{
public:
	MarshalContext context ();

	/// \fn uintptr_t Marshal::marshal_memory (const void* p, size_t& size);
	///
	/// \brief Marshals memory block to another protection domain memory space.
	///
	/// \param          p    A memory block.
	/// \param [in,out] size The size of memory block.
	///   On return, contains the size of memory block allocated in the target memory space.
	///   If size = 0 on return, the memory block is owned by the marshaling system
	///   and mustn't be released.
	///   If size if not zero, the memory block may be adopted by the message recipient
	///   on the other side by `Unmarshal::adopt_memory()` call.
	///
	/// \return An uintptr_t represents address of the memory block at target memory space.

	::Nirvana::UIntPtr marshal_memory (::Nirvana::ConstPointer p, ::Nirvana::Size& size, ::Nirvana::Size release_size);

	/// \fn uintptr_t Marshal::get_buffer (size_t& size, void*& buf_ptr);
	///
	/// \brief Allocates the uninitialized memory block and map it to the target memory space.
	///   The caller gets pointer to the block and fill it with data.
	///
	/// \param [in,out] size    The size of the memory block. See the `marshal_memory`.
	/// \param [out]    buf_ptr The buffer pointer in current memory space.
	///
	/// \return An uintptr_t represents address of the memory block at target memory space.

	::Nirvana::UIntPtr get_buffer (::Nirvana::Size& size, ::Nirvana::Pointer& buf_ptr);

	::Nirvana::UIntPtr marshal_interface (Interface_ptr);
};

BRIDGE_BEGIN (Marshal, CORBA_NIRVANA_REPOSITORY_ID ("Marshal"))
ABI_enum (*_get_context) (Bridge <Marshal>*, Interface*);
::Nirvana::UIntPtr (*marshal_memory) (Bridge <Marshal>*, ::Nirvana::ConstPointer, ::Nirvana::Size*, ::Nirvana::Size, Interface*);
::Nirvana::UIntPtr (*get_buffer) (Bridge <Marshal>*, ::Nirvana::Size*, ::Nirvana::Pointer*, Interface*);
::Nirvana::UIntPtr (*marshal_interface) (Bridge <Marshal>*, Interface*, Interface*);
BRIDGE_END ()

template <class T>
MarshalContext Client <T, Marshal>::context ()
{
	Environment _env;
	Bridge <Marshal>& _b (T::_get_bridge (_env));
	Type <MarshalContext>::C_ret _ret = (_b._epv ().epv._get_context) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Nirvana::UIntPtr Client <T, Marshal>::marshal_memory (::Nirvana::ConstPointer p, ::Nirvana::Size& size, ::Nirvana::Size release_size)
{
	Environment _env;
	Bridge <Marshal>& _b (T::_get_bridge (_env));
	::Nirvana::UIntPtr _ret = (_b._epv ().epv.marshal_memory) (&_b, p, &size, release_size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Nirvana::UIntPtr Client <T, Marshal>::get_buffer (::Nirvana::Size& size, ::Nirvana::Pointer& buf_ptr)
{
	Environment _env;
	Bridge <Marshal>& _b (T::_get_bridge (_env));
	::Nirvana::UIntPtr _ret = (_b._epv ().epv.get_buffer) (&_b, &size, &buf_ptr, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Nirvana::UIntPtr Client <T, Marshal>::marshal_interface (Interface_ptr obj)
{
	Environment _env;
	Bridge <Marshal>& _b (T::_get_bridge (_env));
	::Nirvana::UIntPtr _ret = (_b._epv ().epv.marshal_interface) (&_b, &obj, &_env);
	_env.check ();
	return _ret;
}

class Marshal : public ClientInterface <Marshal>
{};

}
}

#endif

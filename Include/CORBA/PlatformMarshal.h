/*
~~~{.idl}
module CORBA {
module Nirvana {

/// Marshals data between objects in the same platform domain.
pseudo interface PlatformMarshal
{
  /// Returns marshal context.
	/// Used for proxy code optimization.
  attribute readonly PlatformMarshalContext context;

  /// \brief Marshals memory block to the target memory space.
  ///
  /// \param          p    A memory block.
  /// \param [in,out] size The size of memory block.
  ///   On return, contains the size of memory block allocated in the target memory space.
  ///   If size = 0 on return, the memory block is owned by the marshaling system
  ///   and mustn't be released.
  ///   If size if not zero, the memory block may be adopted by the message recipient
  ///   on the other side by `PlatformUnmarshal::adopt_memory()` call.
  ///
  /// \return Address of the memory block at target memory space.
  ::Nirvana::UIntPtr marshal_memory (::Nirvana::ConstPointer p, inout :Nirvana::Size size);

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
#ifndef NIRVANA_ORB_PLATFORMMARSHAL_H_
#define NIRVANA_ORB_PLATFORMMARSHAL_H_

#include "Object.h"
#include "TypeCode_b.h"
#include "PlatformMarshalContext.h"

namespace CORBA {
namespace Nirvana {

class PlatformMarshal;
typedef I_ptr <PlatformMarshal> PlatformMarshal_ptr;
typedef I_var <PlatformMarshal> PlatformMarshal_var;
typedef I_out <PlatformMarshal> PlatformMarshal_out;

/// \interface PlatformMarshal
///
/// \brief Controls marshaling between different protection domains on the one system.

template <class T>
class Client <T, PlatformMarshal> :
	public T
{
public:
	PlatformMarshalContext context ();

	/// \fn uintptr_t PlatformMarshal::marshal_memory (const void* p, size_t& size);
	///
	/// \brief Marshals memory block to another protection domain memory space.
	///
	/// \param          p    A memory block.
	/// \param [in,out] size The size of memory block.
	///   On return, contains the size of memory block allocated in the target memory space.
	///   If size = 0 on return, the memory block is owned by the marshaling system
	///   and mustn't be released.
	///   If size if not zero, the memory block may be adopted by the message recipient
	///   on the other side by `PlatformUnmarshal::adopt_memory()` call.
	///
	/// \return An uintptr_t represents address of the memory block at target memory space.

	uintptr_t marshal_memory (const void* p, size_t& size);

	/// \fn uintptr_t PlatformMarshal::get_buffer (size_t& size, void*& buf_ptr);
	///
	/// \brief Allocates the uninitialized memory block and map it to the target memory space.
	///   The caller gets pointer to the block and fill it with data.
	///
	/// \param [in,out] size    The size of the memory block. See the `marshal_memory`.
	/// \param [out]    buf_ptr The buffer pointer in current memory space.
	///
	/// \return An uintptr_t represents address of the memory block at target memory space.

	uintptr_t get_buffer (size_t& size, void*& buf_ptr);

	uintptr_t marshal_object (Object_ptr);
	uintptr_t marshal_type_code (TypeCode_ptr);
	void release_message ();
	void* send ();
	void return_exception (const Exception*);
};

BRIDGE_BEGIN (PlatformMarshal, CORBA_NIRVANA_REPOSITORY_ID ("PlatformMarshal"))
ABI_enum (*context) (Bridge <PlatformMarshal>*, EnvironmentBridge*);
uintptr_t (*marshal_memory) (Bridge <PlatformMarshal>*, const void* p, size_t* size, EnvironmentBridge*);
uintptr_t (*get_buffer) (Bridge <PlatformMarshal>*, size_t* size, void** buf_ptr, EnvironmentBridge*);
uintptr_t (*marshal_object) (Bridge <PlatformMarshal>*, Interface*, EnvironmentBridge*);
uintptr_t (*marshal_type_code) (Bridge <PlatformMarshal>*, Interface*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
uintptr_t Client <T, PlatformMarshal>::marshal_memory (const void* p, size_t& size)
{
	Environment _env;
	Bridge <PlatformMarshal>& _b (T::_get_bridge (_env));
	uintptr_t _ret = (_b._epv ().epv.marshal_memory) (&_b, p, &size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
uintptr_t Client <T, PlatformMarshal>::get_buffer (size_t& size, void*& buf_ptr)
{
	Environment _env;
	Bridge <PlatformMarshal>& _b (T::_get_bridge (_env));
	uintptr_t _ret = (_b._epv ().epv.get_buffer) (&_b, &size, &buf_ptr, &_env);
	_env.check ();
	return _ret;
}

template <class T>
uintptr_t Client <T, PlatformMarshal>::marshal_object (Object_ptr obj)
{
	Environment _env;
	Bridge <PlatformMarshal>& _b (T::_get_bridge (_env));
	uintptr_t _ret = (_b._epv ().epv.marshal_object) (&_b, &obj, &_env);
	_env.check ();
	return _ret;
}

template <class T>
uintptr_t Client <T, PlatformMarshal>::marshal_type_code (TypeCode_ptr tc)
{
	Environment _env;
	Bridge <PlatformMarshal>& _b (T::_get_bridge (_env));
	uintptr_t _ret = (_b._epv ().epv.marshal_type_code) (&_b, &tc, &_env);
	_env.check ();
	return _ret;
}

class PlatformMarshal : public ClientInterface <PlatformMarshal>
{};

}
}

#endif
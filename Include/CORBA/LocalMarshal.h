#ifndef NIRVANA_ORB_LOCALMARSHAL_H_
#define NIRVANA_ORB_LOCALMARSHAL_H_

#include "Object.h"
#include "TypeCode_b.h"
#include <Nirvana/OLF.h>

namespace CORBA {

class Object;

namespace Nirvana {

class LocalMarshal;
typedef T_ptr <LocalMarshal> LocalMarshal_ptr;
typedef T_var <LocalMarshal> LocalMarshal_var;
typedef T_out <LocalMarshal> LocalMarshal_out;

struct LocalObjectId
{
	uint32_t id;
	uint32_t timestamp;
};

struct LocalObjectRef
{
	LocalObjectId protection_domain;
	LocalObjectId object;
};

struct OperationIndex
{
	uint16_t interface;
	uint16_t operation;
};

struct CallMessageBlock
{
	LocalObjectRef target;
	OperationIndex operation;
};

//! \interface LocalMarshal
//!
//! \brief Controls marshaling between different protection domains on the one system.

template <class T>
class Client <T, LocalMarshal> :
	public T
{
public:
	void begin_call (const LocalObjectRef& target, OperationIndex operation);
	void begin_return ();

	//! \fn uintptr_t LocalMarshal::marshal_memory (const void* p, size_t& size);
	//!
	//! \brief Marshals memory block to another protection domain memory space.
	//!
	//! \param          p    A memory block.
	//! \param [in,out] size The size of memory block.
	//!   On return, contains the size of memory block allocated in the target memory space.
	//!   If size = 0 on return, the memory block is owned by the marshaling system
	//!   and mustn't be released.
	//!   If size if not zero, the memory block may be adopted by the message recipient
	//!   on the other side by `adopt_memory()` call.
	//!
	//! \return An uintptr_t represents address of the memory block at target memory space.

	uintptr_t marshal_memory (const void* p, size_t& size);

	//! \fn uintptr_t LocalMarshal::get_buffer (size_t& size, void*& buf_ptr);
	//!
	//! \brief Allocates the uninitialized memory block and map it to the target memory space.
	//!   The caller gets pointer to the block and fill it with data.
	//!
	//! \param [in,out] size    The size of the memory block. See the `marshal_memory`.
	//! \param [out]    buf_ptr The buffer pointer in current memory space.
	//!
	//! \return An uintptr_t represents address of the memory block at target memory space.

	uintptr_t get_buffer (size_t& size, void*& buf_ptr);

	void adopt_memory (void* p, size_t size);
	uintptr_t marshal_object (Object_ptr);
	Interface_var unmarshal_interface (const void* marshal_data, const Char* interface_id);
	uintptr_t marshal_type_code (TypeCode_ptr);
	TypeCode_var unmarshal_type_code (const void* marshal_data);
	void release_message ();
	void* send ();
	void return_exception (const Exception*);
};

template <>
class Bridge <LocalMarshal> :
	public BridgeMarshal <LocalMarshal>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			void (*begin_call) (Bridge <LocalMarshal>*, const LocalObjectRef* target, OperationIndex operation, EnvironmentBridge*);
			void (*begin_return) (Bridge <LocalMarshal>*, EnvironmentBridge*);
			uintptr_t (*marshal_memory) (Bridge <LocalMarshal>*, const void* p, size_t* size, EnvironmentBridge*);
			uintptr_t (*get_buffer) (Bridge <LocalMarshal>*, size_t* size, void** buf_ptr, EnvironmentBridge*);
			void (*adopt_memory) (Bridge <LocalMarshal>*, void* p, size_t size, EnvironmentBridge*);
			uintptr_t (*marshal_object) (Bridge <LocalMarshal>*, BridgeMarshal <Object>*, EnvironmentBridge*);
			BridgeMarshal <Interface>* (*unmarshal_interface) (Bridge <LocalMarshal>*, const void*, const Char*, EnvironmentBridge*);
			uintptr_t (*marshal_type_code) (Bridge <LocalMarshal>*, BridgeMarshal <TypeCode>*, EnvironmentBridge*);
			BridgeMarshal <TypeCode>* (*unmarshal_type_code) (Bridge <LocalMarshal>*, const void*, EnvironmentBridge*);
			void (*release_message) (Bridge <LocalMarshal>*, EnvironmentBridge*);
			void* (*send) (Bridge <LocalMarshal>*, EnvironmentBridge*);
			void (*return_exception) (Bridge <LocalMarshal>*, const Exception*, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal <LocalMarshal> (epv.interface)
	{}
};

template <class T>
void Client <T, LocalMarshal>::begin_call (const LocalObjectRef& target, OperationIndex operation)
{
	Environment _env;
	Bridge <LocalMarshal>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.begin_call) (&_b, &target, operation, &_env);
	_env.check ();
}

template <class T>
void Client <T, LocalMarshal>::begin_return ()
{
	Environment _env;
	Bridge <LocalMarshal>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.begin_return) (&_b, &_env);
	_env.check ();
}

template <class T>
uintptr_t Client <T, LocalMarshal>::marshal_memory (const void* p, size_t& size)
{
	Environment _env;
	Bridge <LocalMarshal>& _b (T::_get_bridge (_env));
	uintptr_t _ret = (_b._epv ().epv.marshal_memory) (&_b, p, &size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
uintptr_t Client <T, LocalMarshal>::get_buffer (size_t& size, void*& buf_ptr)
{
	Environment _env;
	Bridge <LocalMarshal>& _b (T::_get_bridge (_env));
	uintptr_t _ret = (_b._epv ().epv.get_buffer) (&_b, &size, &buf_ptr, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, LocalMarshal>::adopt_memory (void* p, size_t size)
{
	Environment _env;
	Bridge <LocalMarshal>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.adopt_memory) (&_b, p, size, &_env);
	_env.check ();
}

template <class T>
uintptr_t Client <T, LocalMarshal>::marshal_object (Object_ptr obj)
{
	Environment _env;
	Bridge <LocalMarshal>& _b (T::_get_bridge (_env));
	uintptr_t _ret = (_b._epv ().epv.marshal_object) (&_b, obj, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Interface_var Client <T, LocalMarshal>::unmarshal_interface (const void* marshal_data, const Char* interface_id)
{
	Environment _env;
	Bridge <LocalMarshal>& _b (T::_get_bridge (_env));
	T_ret <Interface> _ret = (_b._epv ().epv.unmarshal_interface) (&_b, marshal_data, interface_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
uintptr_t Client <T, LocalMarshal>::marshal_type_code (TypeCode_ptr tc)
{
	Environment _env;
	Bridge <LocalMarshal>& _b (T::_get_bridge (_env));
	uintptr_t _ret = (_b._epv ().epv.marshal_type_code) (&_b, tc, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, LocalMarshal>::unmarshal_type_code (const void* marshal_data)
{
	Environment _env;
	Bridge <LocalMarshal>& _b (T::_get_bridge (_env));
	T_ret <TypeCode> _ret = (_b._epv ().epv.unmarshal_type_code) (&_b, marshal_data, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, LocalMarshal>::release_message ()
{
	Environment _env;
	Bridge <LocalMarshal>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.release_message) (&_b, &_env);
	_env.check ();
}

template <class T>
void* Client <T, LocalMarshal>::send ()
{
	Environment _env;
	Bridge <LocalMarshal>& _b (T::_get_bridge (_env));
	void* _ret = (_b._epv ().epv.send) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, LocalMarshal>::return_exception (const Exception*)
{
	Environment _env;
	Bridge <LocalMarshal>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.return_exception) (&_b, &_env);
	_env.check ();
}

class LocalMarshal : public ClientInterface <LocalMarshal>
{};

extern const ::Nirvana::ImportInterfaceT <LocalMarshal> g_local_marshal;

}
}

#endif

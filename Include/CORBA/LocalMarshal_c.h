#ifndef NIRVANA_ORB_LOCALMARSHAL_C_H_
#define NIRVANA_ORB_LOCALMARSHAL_C_H_

#include "Object_c.h"
#include "OLF.h"

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
	void begin_call (LocalObjectRef target, OperationIndex operation);
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
	Interface_ptr unmarshal_interface (const void* marshal_data, const Char* interface_id, EnvironmentBridge*);
	void release_message ();
	void post_call (uintptr_t target_object, OperationIndex operation);
	void post_return (uintptr_t return_object);
	void post_exception (uintptr_t return_object, const Exception*, EnvironmentBridge*);
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
			void (*begin) (uintptr_t protection_domain);
			uintptr_t (*marshal_memory) (Bridge <LocalMarshal>*, const void* p, size_t* size, EnvironmentBridge*);
			uintptr_t (*get_buffer) (Bridge <LocalMarshal>*, size_t size, void** buf_ptr, EnvironmentBridge*);
			void (*adopt_memory) (Bridge <LocalMarshal>*, void* p, size_t size, EnvironmentBridge*);
			uintptr_t (*marshal_object) (Bridge <LocalMarshal>*, BridgeMarshal <Object>*, EnvironmentBridge*);
			Bridge <Interface>* (*unmarshal_interface) (Bridge <LocalMarshal>*, const void*, const Char*, EnvironmentBridge*);
			void (*release_message) (Bridge <LocalMarshal>*, EnvironmentBridge*);
			void (*post_call) (Bridge <LocalMarshal>*, uintptr_t target_object, OperationIndex operation, EnvironmentBridge*);
			void (*post_return) (Bridge <LocalMarshal>*, uintptr_t return_object, EnvironmentBridge*);
			void (*post_exception) (Bridge <LocalMarshal>*, uintptr_t return_object, const Exception*, EnvironmentBridge*);
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

class LocalMarshal : public ClientInterface <LocalMarshal>
{};

extern const ImportInterfaceT <LocalMarshal> g_local_marshal;

}
}

#endif

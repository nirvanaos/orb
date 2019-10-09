#ifndef NIRVANA_ORB_LOCALMARSHAL_C_H_
#define NIRVANA_ORB_LOCALMARSHAL_C_H_

#include <CORBA/Object_c.h>
#include <Nirvana/Memory_c.h>

namespace CORBA {

class Object;

namespace Nirvana {

class LocalMarshal;
typedef T_ptr <LocalMarshal> LocalMarshal_ptr;
typedef T_var <LocalMarshal> LocalMarshal_var;
typedef T_out <LocalMarshal> LocalMarshal_out;

struct LocalObjectRef
{
	uintptr_t protection_domain;
	uintptr_t object;
};

struct OperationIndex
{
	uint16_t interface;
	uint16_t operation;
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
			uintptr_t (*marshal_object) (Bridge <LocalMarshal>*, BridgeMarshal <Object>*, EnvironmentBridge*);
			Bridge <Interface>* (*unmarshal_interface) (Bridge <LocalMarshal>*, const void*, const Char*, EnvironmentBridge*);
			void (*release_marshal_data) (Bridge <LocalMarshal>*, EnvironmentBridge*);
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

template <class T>
class Client <T, LocalMarshal> :
	public T
{
public:
	void begin (uintptr_t protection_domain);
	uintptr_t marshal_memory (const void* p, size_t& size);
	uintptr_t get_buffer (size_t size, void** buf_ptr);
	uintptr_t marshal_object (Object_ptr);
	Interface_ptr unmarshal_interface (const void* marshal_data, const Char* interface_id, EnvironmentBridge*);
	void release_marshal_data ();
	void post_call (uintptr_t target_object, OperationIndex operation);
	void post_return (uintptr_t return_object);
	void post_exception (uintptr_t return_object, const Exception*, EnvironmentBridge*);
};

class LocalMarshal : public ClientInterface <LocalMarshal>
{
public:
	static LocalMarshal_ptr singleton ();
};

}
}

#endif

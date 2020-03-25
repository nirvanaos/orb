// ObjectFactory Interface to the core object functionality.
#ifndef NIRVANA_ORB_OBJECTFACTORY_H_
#define NIRVANA_ORB_OBJECTFACTORY_H_

#include "ServantBase.h"
#include "DynamicServant.h"
#include "LocalObject.h"

namespace CORBA {
namespace Nirvana {

class ObjectFactory;
typedef I_ptr <ObjectFactory> ObjectFactory_ptr;
typedef I_var <ObjectFactory> ObjectFactory_var;
typedef I_out <ObjectFactory> ObjectFactory_out;

struct StatelessCreationFrame
{
	const void* tmp; ///< Pointer to the servant temporary location in stack
	size_t size; ///< Servant size
	ptrdiff_t offset; ///< Offset to the stateless memory block
};

template <>
struct Type <StatelessCreationFrame> :
	public TypeFixLen <StatelessCreationFrame>
{};

BRIDGE_BEGIN (ObjectFactory, CORBA_NIRVANA_REPOSITORY_ID ("ObjectFactory"))
void* (*memory_allocate) (Bridge <ObjectFactory>*, size_t size, EnvironmentBridge*);
void (*memory_release) (Bridge <ObjectFactory>*, void* p, size_t size, EnvironmentBridge*);
void (*stateless_begin) (Bridge <ObjectFactory>*, Type <StatelessCreationFrame>::ABI_inout, EnvironmentBridge*);
void* (*stateless_end) (Bridge <ObjectFactory>*, ABI_in <bool> success, EnvironmentBridge*);
const void* (*stateless_copy) (Bridge <ObjectFactory>*, const void* p, size_t size, EnvironmentBridge*);
Interface* (*create_reference_counter) (Bridge <ObjectFactory>*, Interface*, EnvironmentBridge*);
Interface* (*create_servant) (Bridge <ObjectFactory>*, Interface*, EnvironmentBridge*);
Interface* (*create_local_object) (Bridge <ObjectFactory>*, Interface*, Interface*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ObjectFactory> :
	public T
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

	void stateless_begin (T_inout <StatelessCreationFrame> scb);

	void* stateless_end (T_in <bool> success);

	const void* stateless_copy (const void* src, size_t size);

	ReferenceCounter_var create_reference_counter (I_in <DynamicServant> dynamic);
	PortableServer::ServantBase_var create_servant (I_in <PortableServer::ServantBase> impl);
	LocalObject_var create_local_object (I_in <LocalObject> impl, I_in <AbstractBase> abstract_base);
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
void Client <T, ObjectFactory>::stateless_begin (T_inout <StatelessCreationFrame> scs)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.stateless_begin) (&_b, &scs, &_env);
	_env.check ();
}

template <class T>
void* Client <T, ObjectFactory>::stateless_end (T_in <bool> success)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	void* _ret = (_b._epv ().epv.stateless_end) (&_b, &success, &_env);
	_env.check ();
	return _ret;
}

template <class T>
const void* Client <T, ObjectFactory>::stateless_copy (const void* src, size_t size)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	const void* _ret = (_b._epv ().epv.stateless_copy) (&_b, src, size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ReferenceCounter_var Client <T, ObjectFactory>::create_reference_counter (I_in <DynamicServant> dynamic)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <ReferenceCounter> _ret = (_b._epv ().epv.create_reference_counter) (&_b, &dynamic, &_env);
	_env.check ();
	return _ret;
}

template <class T>
PortableServer::ServantBase_var Client <T, ObjectFactory>::create_servant (I_in <PortableServer::ServantBase> impl)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <PortableServer::ServantBase> _ret = (_b._epv ().epv.create_servant) (&_b, &impl, &_env);
	_env.check ();
	return _ret;
}

template <class T>
LocalObject_var Client <T, ObjectFactory>::create_local_object (I_in <LocalObject> impl, I_in <AbstractBase> abstract_base)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <LocalObject> _ret = (_b._epv ().epv.create_local_object) (&_b, &impl, &abstract_base, &_env);
	_env.check ();
	return _ret;
}

class ObjectFactory : public ClientInterface <ObjectFactory>
{};

}
}

#endif

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
	static void* _memory_allocate (Bridge <ObjectFactory>* obj, size_t size, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).memory_allocate (size);
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}

	static void _memory_release (Bridge <ObjectFactory>* obj, void* p, size_t size, EnvironmentBridge* env)
	{
		try {
			S::_implementation (obj).memory_release (p, size);
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}

	static void _stateless_begin (Bridge <ObjectFactory>*, ABI_inout <StatelessCreationBlock> scb, EnvironmentBridge*)
	{
		try {
			S::_implementation (obj).stateless_begin (Type <StatelessCreationBlock>::inout (scb));
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}

	static void* _stateless_end (Bridge <ObjectFactory>*, ABI_in <StatelessCreationBlock> scb, ABI_in <bool> success, EnvironmentBridge*)
	{
		try {
			return S::_implementation (obj).stateless_end (Type <StatelessCreationBlock>::in (scb), Type <bool>::in (success));
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}

	static const void* _stateless_copy (Bridge <ObjectFactory>*, const void* p, size_t size, EnvironmentBridge*)
	{
		try {
			return S::_implementation (obj).stateless_copy (p, size);
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}

	static Interface* _create_reference_counter (Bridge <ObjectFactory>* obj, Interface* dynamic, EnvironmentBridge* env)
	{
		try {
			return TypeI <ReferenceCounter>::ret (S::_implementation (obj).create_reference_counter (TypeI <DynamicServant>::in (dynamic)));
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Interface* _create_servant (Bridge <ObjectFactory>* obj, Interface* impl, EnvironmentBridge* env)
	{
		try {
			return TypeI <PortableServer::ServantBase>::ret (S::_implementation (obj).create_servant (TypeI <PortableServer::ServantBase>::in (impl)));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Interface* _create_local_object (Bridge <ObjectFactory>* obj, Interface* impl, EnvironmentBridge* env)
	{
		try {
			return TypeI <Object>::ret (S::_implementation (obj).create_local_object (TypeI <Object>::in (impl)));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}
};

template <class S>
const Bridge <ObjectFactory>::EPV Skeleton <S, ObjectFactory>::epv_ = {
	{ // interface
		Bridge <ObjectFactory>::interface_id_,
		S::template __duplicate <ObjectFactory>,
		S::template __release <ObjectFactory>
	},
	{ // epv
		S::_memory_allocate,
		S::_memory_release,
		S::_stateless_begin,
		S::_stateless_end,
		S::_stateless_copy,
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

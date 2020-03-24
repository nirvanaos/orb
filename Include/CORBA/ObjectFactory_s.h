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
	static void* _memory_allocate (Bridge <ObjectFactory>* _b, size_t size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).memory_allocate (size);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static void _memory_release (Bridge <ObjectFactory>* _b, void* p, size_t size, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).memory_release (p, size);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void _stateless_begin (Bridge <ObjectFactory>* _b, Type <StatelessCreationFrame>::ABI_inout scs, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).stateless_begin (Type <StatelessCreationFrame>::inout (scs));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void* _stateless_end (Bridge <ObjectFactory>* _b, ABI_in <bool> success, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).stateless_end (Type <bool>::in (success));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static const void* _stateless_copy (Bridge <ObjectFactory>* _b, const void* p, size_t size, EnvironmentBridge* _env)
	{
		try {
			return S::_implementation (_b).stateless_copy (p, size);
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Interface* _create_reference_counter (Bridge <ObjectFactory>* _b, Interface* dynamic, EnvironmentBridge* _env)
	{
		try {
			return TypeI <ReferenceCounter>::ret (S::_implementation (_b).create_reference_counter (TypeI <DynamicServant>::in (dynamic)));
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Interface* _create_servant (Bridge <ObjectFactory>* _b, Interface* impl, EnvironmentBridge* _env)
	{
		try {
			return TypeI <PortableServer::ServantBase>::ret (S::_implementation (_b).create_servant (TypeI <PortableServer::ServantBase>::in (impl)));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
	}

	static Interface* _create_local_object (Bridge <ObjectFactory>* _b, Interface* impl, Interface* ab, EnvironmentBridge* _env)
	{
		try {
			return TypeI <Object>::ret (S::_implementation (_b).create_local_object (TypeI <LocalObject>::in (impl), TypeI <LocalObject>::in (ab)));
		} catch (const Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return 0;
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

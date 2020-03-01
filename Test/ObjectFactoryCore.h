#ifndef NIRVANA_TESTORB_OBJECTFACTORYCORE_H_
#define NIRVANA_TESTORB_OBJECTFACTORYCORE_H_

#include "Server.h"
#include <CORBA/ObjectFactory_s.h>
#include "ServantCore.h"
#include "LocalObjectCore.h"
#include "ReferenceCounterImpl.h"

namespace CORBA {
namespace Nirvana {

class ObjectFactoryCore :
	public ServantStatic <ObjectFactoryCore, ObjectFactory>
{
public:
	static void* memory_allocate (size_t size)
	{
		return ::Nirvana::g_memory->allocate (0, size, 0);
	}

	static void memory_release (void* p, size_t size)
	{
		::Nirvana::g_memory->release (p, size);
	}

	void stateless_begin (StatelessCreationStruct& scs)
	{
		if (!(scs.tmp && scs.size))
			throw BAD_PARAM ();
		void* p = ::Nirvana::g_memory->allocate (0, scs.size, ::Nirvana::Memory::READ_ONLY | ::Nirvana::Memory::RESERVED);
		scs.offset = (Octet*)p - (Octet*)scs.tmp;
		stateless_ = &scs;
	}

	void* stateless_end (bool success)
	{
		StatelessCreationStruct* scs = stateless_;
		stateless_ = 0;
		if (!scs)
			throw BAD_INV_ORDER ();
		void* p = (Octet*)scs->tmp + scs->offset;
		if (success) {
			::Nirvana::g_memory->copy (p, const_cast <void*> (scs->tmp), scs->size, ::Nirvana::Memory::READ_ONLY);
			return p;
		} else {
			::Nirvana::g_memory->release (p, scs->size);
			return 0;
		}
	}

	const void* stateless_copy (const void* src, size_t size)
	{
		return ::Nirvana::g_memory->copy (0, const_cast <void*> (src), size, ::Nirvana::Memory::READ_ONLY);
	}

	static CORBA::Nirvana::ReferenceCounter_ptr create_reference_counter (CORBA::Nirvana::DynamicServant_ptr dynamic)
	{
		return (new ReferenceCounterCore (dynamic))->_get_ptr ();
	}

	static PortableServer::Servant create_servant (PortableServer::Servant servant)
	{
		return (new ServantCore (servant))->_get_ptr ();
	}

	static Object_ptr create_local_object (Object_ptr servant)
	{
#ifndef TEST_LOCAL_OBJECT
		assert (false);
#endif
		return (new LocalObjectCore (servant))->_get_ptr ();
	}

private:
	static StatelessCreationStruct* stateless_;
};

}
}

#endif

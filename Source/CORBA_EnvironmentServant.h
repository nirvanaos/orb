// Internal header
#ifndef NIRVANA_ORB_CORBA_ENVIRONMENTSERVANT_H_
#define NIRVANA_ORB_CORBA_ENVIRONMENTSERVANT_H_

#include <CORBA/CORBA_Environment.h>
#include <CORBA/Implementation.h>

namespace CORBA {
namespace Nirvana {

template <class S>
class CORBA_EnvironmentImpl :
	public ::CORBA::Environment,
	public EnvironmentBase,
	public ReferenceCounterLink,
	public Skeleton <S, DynamicServant>,
	public LifeCycleRefCnt <S>,
	public ServantTraits <S>
{
public:
	template <class I>
	static Bridge <I>* _duplicate (Bridge <I>* itf)
	{
		return LifeCycleRefCnt <S>::_duplicate (itf);
	}

protected:
	CORBA_EnvironmentImpl () :
		::CORBA::Environment (Skeleton <S, DynamicServant>::epv_),
		ReferenceCounterLink (&static_cast <DynamicServant&> (static_cast <Bridge <DynamicServant>&> (*this)))
	{}
};

class CORBA_EnvironmentServant :
	public CORBA_EnvironmentImpl <CORBA_EnvironmentServant>,
	public InterfaceImpl <CORBA_EnvironmentServant, ::CORBA::Environment>
{
public:
	CORBA_EnvironmentServant () :
		user_exceptions_ (nullptr)
	{}

	virtual operator Nirvana::Bridge <Environment>& ()
	{
		return *this;
	}

	virtual void exception (Exception* ex)
	{
		delete exception_;
		exception_ = ex;
	}

	virtual Exception* exception () const
	{
		return exception_;
	}

	virtual void clear ()
	{
		exception_free ();
	}

	static bool is_my_bridge (const BridgeMarshal < ::CORBA::Environment>& bridge)
	{
		return &bridge._epv () == &Skeleton <CORBA_EnvironmentServant, ::CORBA::Environment>::epv_.interface;
	}

	static CORBA_EnvironmentServant* _narrow (Nirvana::BridgeMarshal < ::CORBA::Environment>* bridge)
	{
		if (bridge && is_my_bridge (*bridge))
			return &static_cast <CORBA_EnvironmentServant&> (*bridge);
		else
			return nullptr;
	}

	static CORBA_EnvironmentServant* _narrow (Environment_ptr env)
	{
		if (env) {
			Bridge<CORBA::Environment>& bridge = env->operator Bridge<CORBA::Environment>& ();
			if (is_my_bridge (bridge))
				return &static_cast <CORBA_EnvironmentServant&> (bridge);
		}
		return nullptr;
	}

	void reset (const ExceptionEntry** user_exceptions)
	{
		exception_free ();
		user_exceptions_ = user_exceptions;
	}

private:
	const ExceptionEntry** user_exceptions_;
};

}
}

#endif


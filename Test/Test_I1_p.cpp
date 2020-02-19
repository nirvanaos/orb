#include "Test_I1_s.h"
#include <CORBA/ProxyFactory_s.h>
#include <CORBA/LifeCycleRefCntLink.h>
#include <Nirvana/SynchronizationDomain.h>

namespace Test {

class I1_proxy :
	public CORBA::Nirvana::ImplementationPseudo <I1_proxy, I1>
{
	typedef I1_proxy ThisClass;
public:
	I1_proxy (I1_ptr target);

	static CORBA::Long _op1 (Bridge < ::Test::I1>* _b, CORBA::Long p1, CORBA::Nirvana::EnvironmentBridge* _env)
	{
		const ThisClass& _this = _implementation (_b);
		_b = _this.target_;
		Nirvana::SynchronizationDomain_ptr _sync_domain = _this.sync_domain_;
		Nirvana::ContextFrame _frame;
		_sync_domain->enter (_frame);
		CORBA::Long _ret = (_b->_epv ().epv.op1) (_b, p1, _env);
		_sync_domain->leave (_frame);
		return _ret;
	}

private:
	CORBA::Nirvana::I_ptr <I1> target_;
	Nirvana::SynchronizationDomain_var sync_domain_;
};

class I1_proxy_factory :
	public CORBA::Nirvana::ServantStatic <I1_proxy_factory, CORBA::Nirvana::ProxyFactory>
{
public:
};

}

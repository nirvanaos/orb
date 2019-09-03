#ifndef NIRVANA_ORB_PROXYHELPER_C_H_
#define NIRVANA_ORB_PROXYHELPER_C_H_

#include <Nirvana\Runnable_c.h>

namespace CORBA {
namespace Nirvana {

class ProxyHelper;
typedef T_ptr <ProxyHelper> ProxyHelper_ptr;
typedef T_var <ProxyHelper> ProxyHelper_var;
typedef T_out <ProxyHelper> ProxyHelper_out;

typedef void (*Function1) (void*);

struct ContextFrame
{
	Function1 function;
	void* parameter;
};

template <>
class Bridge <ProxyHelper> :
	public BridgeMarshal <ProxyHelper>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			void (*enter) (Bridge <ProxyHelper>*, ContextFrame*, EnvironmentBridge*);
			void (*leave) (Bridge <ProxyHelper>*, ContextFrame*, EnvironmentBridge*);
			void (*oneway) (Bridge <ProxyHelper>*, Bridge <::Nirvana::Runnable>*, EnvironmentBridge*);
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
		BridgeMarshal <ProxyHelper> (epv.interface)
	{}
};

}
}

#endif

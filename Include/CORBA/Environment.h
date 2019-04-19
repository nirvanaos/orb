#ifndef NIRVANA_ORB_ENVIRONMENT_H_
#define NIRVANA_ORB_ENVIRONMENT_H_

#include "Bridge.h"
#include "Exception.h"

namespace CORBA {
namespace Nirvana {

class EnvironmentBase
{
public:
	void clear ()
	{
		delete exception_;
		exception_ = nullptr;
	}

	void check () const
	{
		if (exception_)
			exception_->_raise ();
	}

protected:
	EnvironmentBase () :
		exception_ (nullptr)
	{}

	~EnvironmentBase ()
	{
		delete exception_;
	}

	void set_exception (Long code, const char* rep_id, const void* param,
											const ExceptionEntry* const* user_exceptions = 0);

protected:
	Exception* exception_;
};

template <>
class BridgeMarshal < ::CORBA::Environment> :
	public Bridge <Interface>
{
public:
	void set_exception (Long code, const char* rep_id, const void* param);
	void set_exception (const Exception& e);
	void set_unknown_exception ();

protected:
	BridgeMarshal (const EPV& epv) :
		Bridge <Interface> (epv)
	{}

private:
	Bridge < ::CORBA::Environment>* unmarshal ();
};

template <>
class Bridge < ::CORBA::Environment> :
	public BridgeMarshal < ::CORBA::Environment>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			void (*set_exception) (Bridge < ::CORBA::Environment>*, Long code, const char* rep_id, const void* param);
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
		BridgeMarshal <Environment> (epv.interface)
	{}
};

}
}

#endif

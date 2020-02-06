#ifndef NIRVANA_ORB_ENVIRONMENTIMPL_H_
#define NIRVANA_ORB_ENVIRONMENTIMPL_H_

#include "Environment_c.h"
#include "ServantImpl.h"
#include "Environment_s.h"

namespace CORBA {
namespace Nirvana {

struct ExceptionEntry;

class EnvironmentBase :
	public Bridge < ::CORBA::Environment>
{
public:
	void exception_set (Long code, const char* rep_id, const void* param,
		const ExceptionEntry* user_exceptions = nullptr);

	const Char* exception_id () const;

	const void* exception_value () const;

	void exception_free ();

	Exception* exception () const
	{
		return exception_;
	}

	void exception (Exception* ex)
	{
		exception_free ();
		exception_ = ex;
	}

	void check () const;

protected:
	EnvironmentBase (const EPV& epv) :
		Bridge < ::CORBA::Environment> (epv),
		exception_ (nullptr)
	{}

	~EnvironmentBase ();

private:
	Exception* exception_;
};

}
}

#endif

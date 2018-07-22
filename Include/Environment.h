#ifndef NIRVANA_ORB_ENVIRONMENT_H_
#define NIRVANA_ORB_ENVIRONMENT_H_

#include "Exception.h"

namespace CORBA
{

namespace Nirvana {

class EnvironmentBridge
{
public:
	void set_exception (const Exception& e);
	void set_unknown_exception ();

protected:
	struct EPV
	{
		void (*set_exception) (EnvironmentBridge*, Long code, const char* rep_id, const void* param);
	};

	EnvironmentBridge (const EPV& epv) :
		epv_ptr_ (&epv)
	{}

private:
	const EPV* epv_ptr_;
};

}

class Environment :
	public Nirvana::EnvironmentBridge
{
public:
	Environment (const Nirvana::ExceptionEntry* const* exceptions = 0);
	~Environment ();

	void exception (Exception* e);

	Exception* exception () const
	{
		return exception_;
	}

	void clear ();

	void check () const
	{
		if (exception_)
			exception_->_raise ();
	}

private:
	static void __set_exception (EnvironmentBridge* bridge, Long code, const char* rep_id, const void* param);

private:
	const Nirvana::ExceptionEntry* const* user_exceptions_;
	Exception* exception_;

	static const EPV epv_;
};

}

#endif

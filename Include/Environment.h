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
		m_epv (&epv)
	{}

private:
	const EPV* m_epv;
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
		return m_exception;
	}

	void clear ();

	void check () const
	{
		if (m_exception)
			m_exception->_raise ();
	}

private:
	static void __set_exception (EnvironmentBridge* bridge, Long code, const char* rep_id, const void* param);

private:
	const Nirvana::ExceptionEntry* const* m_user_exceptions;
	Exception* m_exception;

	static const EPV sm_epv;
};

}

#endif

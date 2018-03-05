#pragma once
#include "Exception.h"

namespace CORBA
{

class Environment
{
public:
	Environment () :
		m_code (0)
	{}

	void set_exception (const Exception& e)
	{
		if (this) // Client can pass NULL environment in special cases.
			m_code = e.code ();
	}

	void set_unknown_exception ()
	{
		m_code = -1;
	}

	void check () const
	{
		if (m_code)
			throw Exception (m_code);
	}
private:
	int m_code;
};

}
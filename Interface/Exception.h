#pragma once
#include "BasicTypes.h"

namespace CORBA
{

class Exception
{
public:
	Exception (int code) :
		m_code (code)
	{}

	Exception () :	// Unknown
		m_code (-1)
	{}

	int code () const
	{
		return m_code;
	}

private:
	int m_code;
};

}

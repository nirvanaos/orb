#include <CORBA/Exception.h>
#include <CORBA/TypeCode.h>

namespace CORBA {

Exception* Exception::__clone () const
{
	Octet* pex = nullptr;
	try {
		TypeCode_ptr tc = __type_code ();
		size_t cb = tc->_size ();
		pex = new Octet [cb];
		tc->_copy (pex, __data ());
		return (Exception*)pex;
	} catch (...) {
		delete [] pex;
		throw;
	}
}

}


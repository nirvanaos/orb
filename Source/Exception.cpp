#include <CORBA/Exception.h>
#include <CORBA/TypeCode.h>
#include <CORBA/system_exceptions.h>

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

namespace Nirvana {

void set_exception (EnvironmentBridge* environment, Long code, const char* rep_id, const void* param)
{
	if (environment && RepositoryId::compatible (environment->_epv ().interface_id, Bridge < ::CORBA::Environment>::interface_id_)) {
		Bridge < ::CORBA::Environment>* b = &static_cast <Bridge < ::CORBA::Environment>&> (*environment);
		(b->_epv ().epv.exception_set) (b, code, rep_id, param);
	}
}

void set_exception (EnvironmentBridge* environment, const Exception& e)
{
	set_exception (environment, e.__code (), e._rep_id (), e.__data ());
}

void set_unknown_exception (EnvironmentBridge* environment)
{
	set_exception (environment, SystemException::EC_UNKNOWN, CORBA_REPOSITORY_ID (UNKNOWN), nullptr);
}

}

}


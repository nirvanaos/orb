#include <CORBA/ABI_forward.h>
#include <Nirvana/throw_exception.h>
#include <exception>

namespace CORBA {
namespace Nirvana {

void _check_pointer (const void* p)
{
	if (!p)
		::Nirvana::throw_BAD_PARAM ();	// Invalid pointer
}

bool uncaught_exception ()
{
#ifdef NIRVANA_C17
	return std::uncaught_exceptions () != 0;
#else
	return std::uncaught_exception ();
#endif
}

}
}

#include <CORBA/Environment.h>

namespace CORBA {
namespace Nirvana {

using namespace std;

Environment& Environment::operator = (const EnvironmentBase& src)
{
	EnvironmentBase::operator = (src);
	return *this;
}

Environment& Environment::operator = (EnvironmentBase&& src) NIRVANA_NOEXCEPT
{
	move_from (src);
	return *this;
}

}
}

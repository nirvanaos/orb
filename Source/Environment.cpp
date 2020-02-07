#include <CORBA/Environment.h>
#include <utility>

namespace CORBA {
namespace Nirvana {

using namespace std;

Environment::Environment (const EnvironmentBase& src)
{
	EnvironmentBase::operator = (src);
}

Environment::Environment (EnvironmentBase&& src) NIRVANA_NOEXCEPT
{
	EnvironmentBase::operator = (move (src));
}

Environment& Environment::operator = (const EnvironmentBase& src)
{
	EnvironmentBase::operator = (src);
	return *this;
}

Environment& Environment::operator = (EnvironmentBase&& src) NIRVANA_NOEXCEPT
{
	EnvironmentBase::operator = (move (src));
	return *this;
}

}
}

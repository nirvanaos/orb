#include <CORBA/StringManager_c.h>
#include <CORBA/RepositoryId.h>

namespace CORBA {
namespace Nirvana {

const Char Bridge <StringManager <Char> >::interface_id_ [] = CORBA_NIRVANA_REPOSITORY_ID (StringManager<Char>);
const Char Bridge <StringManager <WChar> >::interface_id_ [] = CORBA_NIRVANA_REPOSITORY_ID (StringManager<WChar>);

}
}

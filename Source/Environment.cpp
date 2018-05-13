#include "AbstractBase.h"

namespace CORBA {

namespace Nirvana {

void EnvironmentBridge::set_exception (const Exception& e)
{
	if (this) // Client can pass NULL environment in special cases.
		(m_epv->set_exception) (this, e.__code (), e._name (), e.__data ());
}

void EnvironmentBridge::set_unknown_exception ()
{
	set_exception (UNKNOWN ());
}

}

using namespace Nirvana;

const Nirvana::EnvironmentBridge::EPV Environment::sm_epv = { __set_exception };

Environment::Environment (const ExceptionEntry* const* exceptions) :
	Nirvana::EnvironmentBridge (sm_epv),
	m_user_exceptions (exceptions),
	m_exception (0)
{}

Environment::~Environment ()
{
	delete m_exception;
}

void Environment::exception (Exception* e)
{
	delete m_exception;
	m_exception = e;
}

void Environment::clear ()
{
	delete m_exception;
	m_exception = 0;
}

void Environment::__set_exception (EnvironmentBridge* bridge, Long code, const char* rep_id, const void* param)
{
	Environment* _this = static_cast <Environment*> (bridge);
	_this->clear ();
	if (rep_id) {
		Exception* e = 0;
		try {
			if (code >= 0)
				e = SystemException::_create (rep_id, param, code);
			else if (_this->m_user_exceptions) {
				for (const ExceptionEntry* const* p = _this->m_user_exceptions; *p; ++p)
					if (RepositoryId::compatible ((*p)->rep_id, rep_id)) {
						e = ((*p)->create) (param);
						break;
					}
				if (!e)
					e = new UNKNOWN ();
			}
		} catch (...) {
		}
		_this->m_exception = e;
	}
}

}

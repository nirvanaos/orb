#include "AbstractBase.h"

namespace CORBA {

namespace Nirvana {

void EnvironmentBridge::set_exception (const Exception& e)
{
	if (this) // Client can pass NULL environment in special cases.
		(epv_ptr_->set_exception) (this, e.__code (), e._rep_id (), e.__data ());
}

void EnvironmentBridge::set_unknown_exception ()
{
	set_exception (UNKNOWN ());
}

}

using namespace Nirvana;

const Nirvana::EnvironmentBridge::EPV Environment::epv_ = { __set_exception };

Environment::Environment (const ExceptionEntry* const* exceptions) :
	Nirvana::EnvironmentBridge (epv_),
	user_exceptions_ (exceptions),
	exception_ (nullptr)
{}

Environment::~Environment ()
{
	delete exception_;
}

void Environment::exception (Exception* e)
{
	delete exception_;
	exception_ = e;
}

void Environment::clear ()
{
	delete exception_;
	exception_ = nullptr;
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
			else if (_this->user_exceptions_) {
				for (const ExceptionEntry* const* p = _this->user_exceptions_; *p; ++p)
					if (RepositoryId::compatible ((*p)->rep_id, rep_id)) {
						e = ((*p)->create) (param);
						break;
					}
				if (!e)
					e = new UNKNOWN ();
			}
		} catch (...) {
		}
		_this->exception_ = e;
	}
}

}

#include <CORBA/EnvironmentImpl.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/CORBA_Environment.h>

namespace CORBA {
namespace Nirvana {

const Char Bridge < ::CORBA::Environment>::interface_id_ [] = CORBA_REPOSITORY_ID (Environment);

void BridgeMarshal < ::CORBA::Environment>::set_exception (Long code, const char* rep_id, const void* param)
{
	// Client can pass NULL environment in special cases, so we check this for NULL
	if (this && RepositoryId::compatible (_epv ().interface_id, Bridge < ::CORBA::Environment>::interface_id_)) {
		Bridge < ::CORBA::Environment>* b = &static_cast <Bridge < ::CORBA::Environment>&> (*this);
		(b->_epv ().epv.set_exception) (b, code, rep_id, param);
	}
}

void BridgeMarshal < ::CORBA::Environment>::set_exception (const Exception& e)
{
	set_exception (e.__code (), e._rep_id (), e.__data ());
}

void BridgeMarshal < ::CORBA::Environment>::set_unknown_exception ()
{
	set_exception (0, CORBA_REPOSITORY_ID (UNKNOWN), nullptr);
}

void EnvironmentBase::set_exception (Long code, const char* rep_id, const void* param, 
																		 const Nirvana::ExceptionEntry* const* user_exceptions)
{
	clear ();
	if (rep_id) {
		Exception* e = nullptr;
		try {
			if (code >= 0)
				e = SystemException::_create (rep_id, param, code);
			else if (user_exceptions) {
				for (const ExceptionEntry* const* p = user_exceptions; *p; ++p)
					if (RepositoryId::compatible ((*p)->rep_id, rep_id)) {
						e = ((*p)->create) (param);
						break;
					}
				if (!e)
					e = new UNKNOWN ();
			}
		} catch (...) {
		}
		exception_ = e;
	}
}

class EnvironmentProxy :
	public ::CORBA::Environment
{
public:
	EnvironmentProxy (Bridge < ::CORBA::Environment>* bridge) :
		bridge_ (bridge)
	{}

	virtual void exception (Exception* ex)
	{
		::CORBA::Environment::exception (ex);
		bridge_->set_exception (ex->__code (), ex->_rep_id (), ex->__data ());
	}

	virtual operator Nirvana::Bridge <Environment>& ()
	{
		return *bridge_;
	}

private:
	Bridge < ::CORBA::Environment>* bridge_;
};

}

Environment_ptr Environment::unmarshal (Nirvana::BridgeMarshal <Environment>* bridge)
{
	if (bridge && Nirvana::RepositoryId::compatible (bridge->_epv ().interface_id, Nirvana::Bridge <Environment>::interface_id_))
		return new Nirvana::EnvironmentProxy (&static_cast <Nirvana::Bridge <Environment>&> (*bridge));
	else
		return Environment_ptr::nil ();
}

}

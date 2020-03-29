/// \file ProxyManager.h
#ifndef NIRVANA_ORB_CORE_PROXYMANAGER_H_
#define NIRVANA_ORB_CORE_PROXYMANAGER_H_

#include <CORBA/Proxy/Proxy.h>
#include <Core/Array.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

/// \brief Base for all proxies.
class ProxyManager :
	public Bridge <IOReference>,
	public Bridge <Object>
{
public:
	// Abstract base
	Interface* _query_interface (const String& type_id) const
	{
		const InterfaceEntry* ie = find_interface (type_id);
		if (ie)
			return ie->proxy;
		else
			return nullptr;
	}

	// Object operations

	InterfaceDef_var _get_interface () const
	{
		return InterfaceDef::_nil (); // TODO: Implement.
	}

	Boolean _is_a (const String& type_id) const
	{
		return find_interface (type_id) != nullptr;
	}

	Boolean _non_existent ()
	{
		BooleanRet out;
		Marshal_var m;
		ior ()->call (OperationIndex{ object_itf_idx (), OBJ_OP_NON_EXISTENT }, nullptr, 0, m, &out, sizeof (out));
		Boolean _ret;
		_unmarshal (out._ret, Unmarshal::_nil (), _ret);
		return _ret;
	}

	Boolean _is_equivalent (Object_ptr other_object) const
	{
		return &static_cast <const Bridge <Object>&> (*this) == &other_object;
	}

	ULong _hash (ULong maximum) const
	{
		return (ULong)(uintptr_t)this % maximum;
	}

	// TODO: More Object operations shall be here...

protected:
	ProxyManager (const Bridge <IOReference>::EPV& epv_ior, const Bridge <Object>::EPV& epv_obj,
		String_in primary_iid, const Operation object_ops [3], Interface* object_impl);

	IOReference_ptr ior ()
	{
		return &static_cast <IOReference&> (static_cast <Bridge <IOReference>&> (*this));
	}

	struct InterfaceEntry
	{
		const Char* iid;
		size_t iid_len;
		Interface* proxy;
		DynamicServant_ptr deleter;
		Interface* implementation;
		CountedArray <Operation> operations;

		~InterfaceEntry () NIRVANA_NOEXCEPT
		{
			if (deleter)
				deleter->_delete ();
		}
	};

	struct OperationEntry
	{
		const Char* name;
		size_t name_len;
		OperationIndex idx;
	};

	const InterfaceEntry* find_interface (String_in iid) const;
	const OperationEntry* find_operation (String_in name) const;

	UShort object_itf_idx () const
	{
		return object_itf_idx_;
	}

	struct BooleanRet
	{
		ABI_boolean _ret;
	};

	static const Parameter is_a_param_;

	// Object operation indexes
	enum
	{
		OBJ_OP_GET_INTERFACE,
		OBJ_OP_IS_A,
		OBJ_OP_NON_EXISTENT
	};

private:
	struct IEPred;
	struct OEPred;

	void create_proxy (InterfaceEntry& ie);

private:
	::Nirvana::Core::Array <InterfaceEntry> interfaces_;
	::Nirvana::Core::Array <OperationEntry> operations_;

	UShort object_itf_idx_;
};

}
}
}

#endif

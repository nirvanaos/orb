#include <Nirvana/Nirvana.h>
#include <Core/ORB/ProxyManager.h>
#include <algorithm>

namespace CORBA {
namespace Nirvana {
namespace Core {

using namespace ::Nirvana;
using namespace std;

struct ProxyManager::IEPred
{
	bool operator () (const InterfaceEntry& lhs, const InterfaceEntry& rhs) const
	{
		return RepositoryId::compare (lhs.iid, lhs.iid_len, rhs.iid, rhs.iid_len) < 0;
	}

	bool operator () (const String& lhs, const InterfaceEntry& rhs) const
	{
		return RepositoryId::compare (rhs.iid, rhs.iid_len, lhs) > 0;
	}

	bool operator () (const InterfaceEntry& lhs, const String& rhs) const
	{
		return RepositoryId::compare (lhs.iid, lhs.iid_len, rhs) < 0;
	}
};

struct ProxyManager::OEPred
{
	bool operator () (const OperationEntry& lhs, const OperationEntry& rhs) const
	{
		return compare (lhs.name, lhs.name_len, rhs.name, rhs.name_len) < 0;
	}

	bool operator () (const String& lhs, const OperationEntry& rhs) const
	{
		return compare (lhs.data (), lhs.size (), rhs.name, rhs.name_len) < 0;
	}

	bool operator () (const OperationEntry& lhs, const String& rhs) const
	{
		return compare (lhs.name, lhs.name_len, rhs.data (), rhs.size ()) < 0;
	}

	static int compare (const Char* lhs, size_t lhs_len, const Char* rhs, size_t rhs_len);
};

int ProxyManager::OEPred::compare (const Char* lhs, size_t lhs_len, const Char* rhs, size_t rhs_len)
{
	return lexicographical_compare (lhs, lhs + lhs_len, rhs, rhs + rhs_len);
}

const Parameter ProxyManager::is_a_param_ = { "logical_type_id", _tc_string };

// Implicit operation names
const Char ProxyManager::op_get_interface_ [] = "_get_interface";
const Char ProxyManager::op_is_a_ []= "_is_a";
const Char ProxyManager::op_non_existent_ [] = "_non_existent";

ProxyManager::ProxyManager (const Bridge <IOReference>::EPV& epv_ior, const Bridge <Object>::EPV& epv_obj,
	String_in primary_iid, const Operation object_ops [3], void* object_impl) :
	Bridge <IOReference> (epv_ior),
	Bridge <Object> (epv_obj)
{
	ProxyFactory_var proxy_factory = g_binder->bind <ProxyFactory> (primary_iid);

	const InterfaceMetadata* metadata = proxy_factory->metadata ();
	if (!metadata)
		throw OBJ_ADAPTER (); // TODO: Log

	ULong itf_cnt = metadata->interfaces.size;
	if (!itf_cnt || itf_cnt > numeric_limits <UShort>::max ())
		throw OBJ_ADAPTER (); // TODO: Log
	interfaces_.allocate (itf_cnt + 1);
	InterfaceEntry* ie = interfaces_.begin ();

	{ // Interface Object
		ie->iid = Object::repository_id_;
		ie->iid_len = countof (Object::repository_id_);
		ie->proxy = &static_cast <Bridge <Object>&> (*this);
		ie->operations.p = object_ops;
		ie->operations.size = 3;
		ie->implementation = (Interface*)object_impl;
		++ie;
	}

	const Char* proxy_primary_iid;

	{ // Fill interface table
		const Char* const* itf = metadata->interfaces.p;
		
		// Primary interface must be first
		proxy_primary_iid = *itf;
		if (!RepositoryId::compatible (proxy_primary_iid, primary_iid))
			throw OBJ_ADAPTER (); // TODO: Log

		do {
			const Char* iid = *itf;
			if (!iid)
				throw OBJ_ADAPTER (); // TODO: Log
			ie->iid = iid;
			ie->iid_len = strlen (iid);
			++itf;
		} while (interfaces_.end () != ++ie);
	}

	sort (interfaces_.begin (), interfaces_.end (), IEPred ());

	// Create base proxies
	ie = interfaces_.begin ();
	InterfaceEntry* primary = nullptr;
	do {
		const Char* iid = ie->iid;
		if (iid == proxy_primary_iid)
			primary = ie;
		else if (iid == Object::repository_id_)
			object_itf_idx_ = (UShort)(ie - interfaces_.begin ());
		else
			create_proxy (*ie);
	} while (interfaces_.end () != ++ie);

	// Create primary proxy
	assert (primary);
	proxy_factory->create_proxy (ior (), (UShort)(primary - interfaces_.begin ()), primary->deleter);
	ie->operations = metadata->operations;

	// Total count of operations
	size_t op_cnt = 0;
	ie = interfaces_.begin ();
	do {
		op_cnt += ie->operations.size;
	} while (interfaces_.end () != ++ie);

	// Fill operation table
	operations_.allocate (op_cnt);
	OperationEntry* op = operations_.begin ();
	ie = interfaces_.begin ();
	do {
		OperationIndex idx;
		idx.interface_idx = (UShort)(ie - interfaces_.begin ());
		idx.operation_idx = 0;
		for (const Operation* p = ie->operations.p, *end = p + ie->operations.size; p != end; ++p) {
			const Char* name = p->name;
			if (!name)
				throw OBJ_ADAPTER (); // TODO: Log
			op->name = name;
			op->name_len = strlen (name);
			op->idx = idx;
			++idx.operation_idx;
		}
	} while (interfaces_.end () != ++ie);

	sort (operations_.begin (), operations_.end (), OEPred ());
	// TODO: Check name uniqueness?
}

void ProxyManager::create_proxy (InterfaceEntry& ie)
{
	if (!ie.proxy) {
		StringBase <Char> iid (ie.iid);
		ProxyFactory_var pf = g_binder->bind <ProxyFactory> (iid);
		const InterfaceMetadata* md = pf->metadata ();
		if (!md)
			throw OBJ_ADAPTER (); // TODO: Log
		const Char* const* base = md->interfaces.p;
		if (!base)
			throw OBJ_ADAPTER (); // TODO: Log
		if (!RepositoryId::compatible (*base, iid))
			throw OBJ_ADAPTER (); // TODO: Log
		if (!md->interfaces.size)
			throw OBJ_ADAPTER (); // TODO: Log
		const Char* const* base_end = base + md->interfaces.size;
		++base;
		for (; base != base_end; ++base) {
			InterfaceEntry* base_ie = const_cast <InterfaceEntry*> (find_interface (iid));
			if (!base_ie)
				throw OBJ_ADAPTER (); // TODO: Log
			create_proxy (*base_ie);
		}
		pf->create_proxy (ior (), (UShort)(&ie - interfaces_.begin ()), ie.deleter);
		ie.operations = md->operations;
	}
}

const ProxyManager::InterfaceEntry* ProxyManager::find_interface (String_in iid) const
{
	const String& siid = static_cast <const String&> (iid);
	const InterfaceEntry* pf = lower_bound (interfaces_.begin (), interfaces_.end (), siid, IEPred ());
	if (pf != interfaces_.end () && RepositoryId::compatible (pf->iid, pf->iid_len, siid))
		return pf;
	return nullptr;
}

const ProxyManager::OperationEntry* ProxyManager::find_operation (String_in name) const
{
	const String& sname = static_cast <const String&> (name);
	const OperationEntry* pf = lower_bound (operations_.begin (), operations_.end (), sname, OEPred ());
	if (pf != operations_.end () && !OEPred () (sname, *pf))
		return pf;
	return nullptr;
}

}
}
}

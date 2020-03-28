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

ProxyManager::ProxyManager (IOReference_ptr ior, String_in primary_iid)
{
	ProxyFactory_var proxy_factory = g_binder->bind <ProxyFactory> (primary_iid);

	const InterfaceMetadata* metadata = proxy_factory->metadata ();
	if (!metadata)
		throw OBJ_ADAPTER (); // TODO: Log

	ULong itf_cnt = metadata->interfaces.size;
	if (!itf_cnt)
		throw OBJ_ADAPTER (); // TODO: Log
	interfaces_.allocate (itf_cnt + 1);
	InterfaceEntry* ie = interfaces_.begin ();

	{
		Object_ptr obj_proxy = ior;
		const Char* obj_iid = obj_proxy->_epv ().header.interface_id;
		ie->iid = obj_iid;
		ie->iid_len = strlen (obj_iid);
		ie->proxy = &obj_proxy;
		// TODO: ie->operations = ???
	}

	{
		const Char* const* itf = metadata->interfaces.p;
		const Char* const* end = itf + itf_cnt;
		do {
			++ie;
			const Char* iid = *itf;
			if (!iid)
				throw OBJ_ADAPTER (); // TODO: Log
			ie->iid = iid;
			ie->iid_len = strlen (iid);
		} while (end != ++itf);
	}

	sort (interfaces_.begin (), interfaces_.end (), IEPred ());

	ie = interfaces_.begin ();
	InterfaceEntry* end = interfaces_.end ();
	InterfaceEntry* primary = nullptr;
	do {
		if (!ie->proxy) {
			if (RepositoryId::compatible (ie->iid, ie->iid_len, primary_iid)) {
				if (primary)
					throw OBJ_ADAPTER (); // TODO: Log
				primary = ie;
			} else
				create_proxy (ior, *ie);
		}
	} while (end != ++ie);

	if (!primary)
		throw OBJ_ADAPTER (); // TODO: Log

	proxy_factory->create_proxy (ior, (UShort)(primary - interfaces_.begin ()), primary->deleter);
	primary->operations = metadata->operations;

	size_t op_cnt = 0;
	ie = interfaces_.begin ();
	do {
		op_cnt += ie->operations.size;
	} while (end != ++ie);

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
	} while (end != ++ie);

	sort (operations_.begin (), operations_.end (), OEPred ());
	// TODO: Check name uniqueness?
}

void ProxyManager::create_proxy (IOReference_ptr ior, InterfaceEntry& ie)
{
	if (!ie.proxy) {
		StringBase <Char> iid (ie.iid);
		ProxyFactory_var pf = g_binder->bind <ProxyFactory> (iid);
		const InterfaceMetadata* md = pf->metadata ();
		if (!md)
			throw_OBJ_ADAPTER (); // TODO: Log
		const Char* const* base = md->interfaces.p;
		if (!base)
			throw_OBJ_ADAPTER (); // TODO: Log
		if (!RepositoryId::compatible (*base, iid))
			throw_OBJ_ADAPTER (); // TODO: Log
		if (!md->interfaces.size)
			throw_OBJ_ADAPTER (); // TODO: Log
		const Char* const* base_end = base + md->interfaces.size;
		++base;
		for (; base != base_end; ++base) {
			InterfaceEntry* base_ie = find_interface (iid);
			if (!base_ie)
				throw_OBJ_ADAPTER (); // TODO: Log
			create_proxy (ior, *base_ie);
		}
		pf->create_proxy (ior, (UShort)(&ie - interfaces_.begin ()), ie.deleter);
		ie.operations = md->operations;
	}
}

ProxyManager::InterfaceEntry* ProxyManager::find_interface (String_in iid)
{
	const String& siid = static_cast <const String&> (iid);
	InterfaceEntry* pf = lower_bound (interfaces_.begin (), interfaces_.end (), siid, IEPred ());
	if (pf != interfaces_.end () && RepositoryId::compatible (pf->iid, pf->iid_len, siid))
		return pf;
	return nullptr;
}

ProxyManager::OperationEntry* ProxyManager::find_operation (String_in name)
{
	const String& sname = static_cast <const String&> (name);
	OperationEntry* pf = lower_bound (operations_.begin (), operations_.end (), sname, OEPred ());
	if (pf != operations_.end () && !OEPred () (sname, *pf))
		return pf;
	return nullptr;
}

}
}
}

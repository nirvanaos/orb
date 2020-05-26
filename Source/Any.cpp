#include <CORBA/Any.h>
#include <CORBA/tc_constants.h>
#include <Nirvana/Memory.h>
#include <Nirvana/core_objects.h>

namespace CORBA {

namespace Nirvana {

void Type <Any>::check (const ABI_type& any)
{
	TypeCode::_check (any.type ());
	if (any.is_large () && !any.large_pointer ())
		::Nirvana::throw_BAD_PARAM ();
}

}

void Any::clear ()
{
	TypeCode_ptr tc = type ();
	bool large = is_large ();
	if (tc) {
		void* p;
		if (large)
			p = large_pointer ();
		else
			p = small_pointer ();
		tc->_destruct (p);
		release (tc);
	}
	if (large)
		::Nirvana::g_memory->release (large_pointer (), large_size ());
	reset ();
}

void* Any::prepare (TypeCode_ptr tc)
{
	clear ();
	void* dst = nullptr;
	if (tc) {
		ULong size = tc->_size ();
		if (!size)
			::Nirvana::throw_BAD_TYPECODE ();
		if (size <= SMALL_CAPACITY)
			dst = small_pointer ();
		else
			large_pointer (dst = ::Nirvana::g_memory->allocate (0, size, 0), size);
	}
	return dst;
}

void Any::set_type (TypeCode_ptr tc)
{
	ABI::type (static_cast <Nirvana::Bridge <TypeCode>*> (&TypeCode::_duplicate (tc)));
}

void Any::copy_from (TypeCode_ptr tc, const void* val)
{
	void* dst = prepare (tc);
	if (dst) {
		tc->_copy (dst, val);
		set_type (tc);
	}
}

void Any::copy_from (const Any& src)
{
	TypeCode_ptr tc = src.type ();
	void* dst = prepare (tc);
	if (dst) {
		tc->_copy (dst, src.data ());
		set_type (tc);
	}
}

void Any::move_from (TypeCode_ptr tc, void* val)
{
	void* dst = prepare (tc);
	tc->_move (dst, val);
	set_type (tc);
}

void Any::type (TypeCode_ptr alias)
{
	TypeCode_ptr tc = type ();
	if (tc && tc->equivalent (alias)) {
		release (tc);
		set_type (tc);
	}
}

static_assert (Nirvana::ABI <Any>::SMALL_CAPACITY >= sizeof (SystemException::Data), "Any data must fit SystemException::Data.");

bool Any::is_system_exception () const
{
	return SystemException::_get_exception_entry (type ());
}

const void* Any::data () const
{
	assert (type ());
	if (is_large ())
		return large_pointer ();
	else if (is_system_exception ())
		return ((const SystemException*)small_pointer ())->__data ();
	else
		return small_pointer ();
}

void Any::operator <<= (from_boolean from)
{
	clear ();
	*(Boolean*)small_pointer () = from.val;
	set_type (_tc_boolean);
}

Boolean Any::operator >>= (to_boolean to) const
{
	if (type ()->equivalent (_tc_boolean)) {
		to.ref = *(const Boolean*)small_pointer ();
		return true;
	} else
		return false;
}

void Any::operator <<= (from_char from)
{
	clear ();
	*(Char*)small_pointer () = from.val;
	set_type (_tc_char);
}

Boolean Any::operator >>= (to_char to) const
{
	if (type ()->equivalent (_tc_char)) {
		to.ref = *(const Char*)small_pointer ();
		return true;
	} else
		return false;
}

void Any::operator <<= (from_wchar from)
{
	clear ();
	*(WChar*)small_pointer () = from.val;
	set_type (_tc_wchar);
}

Boolean Any::operator >>= (to_wchar to) const
{
	if (type ()->equivalent (_tc_wchar)) {
		to.ref = *(const WChar*)small_pointer ();
		return true;
	} else
		return false;
}

void Any::operator <<= (from_octet from)
{
	clear ();
	*(Octet*)small_pointer () = from.val;
	set_type (_tc_octet);
}

Boolean Any::operator >>= (to_octet to) const
{
	if (type ()->equivalent (_tc_octet)) {
		to.ref = *(const Octet*)small_pointer ();
		return true;
	} else
		return false;
}

void operator <<= (Any& any, const Exception& e)
{
	any.copy_from (e.__type_code (), e.__data ());
}

void operator <<= (Any& any, Exception&& e)
{
	any.move_from (e.__type_code (), e.__data ());
}

Boolean operator >>= (const Any& any, SystemException& se)
{
	TypeCode_ptr tc = any.type ();
	if (tc && tc->kind () == tk_except) {
		const Char* id = tc->id ();
		const Char standard_prefix [] = "IDL:omg.org/CORBA/";
		const Char nirvana_prefix [] = "IDL:CORBA/";
		if (
			!strncmp(standard_prefix, id, countof (standard_prefix) - 1)
		||
			!strncmp (nirvana_prefix, id, countof (nirvana_prefix) - 1)
		) {
			const Nirvana::ExceptionEntry* pee = SystemException::_get_exception_entry (id, Exception::EC_SYSTEM_EXCEPTION);
			assert (pee);
			(pee->construct) (&se);
			if (Nirvana::RepositoryId::compatible (pee->rep_id, id))
				tc->_copy (se.__data (), any.data ());
			return true;
		}
	}
	return false;
}

}

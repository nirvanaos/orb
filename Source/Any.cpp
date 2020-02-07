#include <CORBA/Any.h>
#include <CORBA/tc_constants.h>
#include <CORBA/exceptions.h>
#include <Nirvana/Memory.h>

namespace CORBA {

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
		::Nirvana::g_default_heap->release (large_pointer (), large_size ());
	reset ();
}

void* Any::prepare (TypeCode_ptr tc)
{
	clear ();
	void* dst = nullptr;
	if (tc) {
		ULong size = tc->_size ();
		if (!size)
			throw BAD_TYPECODE ();
		if (size <= SMALL_CAPACITY)
			dst = small_pointer ();
		else
			large_pointer (dst = ::Nirvana::g_default_heap->allocate (0, size, 0), size);
	}
	return dst;
}

void Any::copy_from (TypeCode_ptr tc, const void* val)
{
	void* dst = prepare (tc);
	tc->_copy (dst, val);
	set_type (tc);
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

}

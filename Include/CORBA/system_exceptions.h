/// \file
/// Macros to define all system exceptions.
/// Order is significant because exceptions must be sorted by repo id.
/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIRVANA_ORB_SYSTEM_EXCEPTIONS_H_
#define NIRVANA_ORB_SYSTEM_EXCEPTIONS_H_

/// CORBA system exceptions
#define CORBA_EXCEPTIONS(M)/*
*/ M (BAD_CONTEXT) /* error processing context object
*/ M (BAD_INV_ORDER) /* routine invocations out of order
*/ M (BAD_OPERATION) /* invalid operation
*/ M (BAD_PARAM) /* an invalid parameter was passed
*/ M (BAD_QOS) /* bad quality of service
*/ M (BAD_TYPECODE) /* bad typecode
*/ M (CODESET_INCOMPATIBLE) /* incompatible code set
*/ M (COMM_FAILURE) /* communication failure
*/ M (DATA_CONVERSION) /* data conversion error
*/ M (FREE_MEM) /* cannot free memory
*/ M (IMP_LIMIT) /* violated implementation limit
*/ M (INITIALIZE) /* ORB initialization failure
*/ M (INTERNAL) /* ORB internal error
*/ M (INTF_REPOS) /* error accessing interface repository
*/ M (INVALID_TRANSACTION) /* invalid transaction
*/ M (INV_FLAG) /* invalid flag was specified
*/ M (INV_IDENT) /* invalid identifier syntax
*/ M (INV_OBJREF) /* invalid object reference
*/ M (INV_POLICY) /* invalid policy
*/ M (MARSHAL) /* error marshalling param/result
*/ M (NO_IMPLEMENT) /* operation implementation unavailable
*/ M (NO_MEMORY) /* dynamic memory allocation failure
*/ M (NO_PERMISSION) /* no permission for attempted op.
*/ M (NO_RESOURCES) /* insufficient resources for req.
*/ M (NO_RESPONSE) /* response to req. not yet available
*/ M (OBJECT_NOT_EXIST) /* non-existent object, delete reference
*/ M (OBJ_ADAPTER) /* failure detected by object adapter
*/ M (PERSIST_STORE) /* persistent storage failure
*/ M (REBIND) /* rebind needed
*/ M (TIMEOUT) /* operation timed out
*/ M (TRANSACTION_MODE) /* invalid transaction mode
*/ M (TRANSACTION_REQUIRED) /* transaction required
*/ M (TRANSACTION_ROLLEDBACK) /* transaction rolled back
*/ M (TRANSACTION_UNAVAILABLE) /* no transaction
*/ M (TRANSIENT) /* transient failure - reissue request
*/ M (UNKNOWN) /* the unknown exception
*/

/// Nirvana system exceptions. Currently, we do not define one.
#define NIRVANA_EXCEPTIONS(M)

#define SYSTEM_EXCEPTIONS(M) \
CORBA_EXCEPTIONS (M)\
NIRVANA_EXCEPTIONS (M)

#endif

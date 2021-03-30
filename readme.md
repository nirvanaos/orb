# ORB IDL support library

This is a part of the Nirvana project.

## Purpose

This repository contains source code for Nirvana ORB IDL support library.

* *Source* Source code for Nirvana runtime library.
* *Include/CORBA* Headers.
* *Include/CORBA/Proxy* Headers intended for compilation of the proxy code
 generated by the IDL compiler. You don't need to include them.
* *CoreImport* Source code for CoreImport library.

The main header for clients is *Include/CORBA/CORBA.h*

Namespace `CORBA` contains definitions compliant to the CORBA C++ mapping specification.

Namespace `CORBA::Nirvana` contains internal implementation details and is not intended
for usage by an application programmer.

The main header for servers is *Include/CORBA/Server.h*

Namespace `PortableServer` contains definitions compliant to the CORBA C++ mapping specification.

## How to build
This repository is a submodule and does not contain any project files.
It must be included as a Git submodule in a Nirvana library project.
To build the library under Visual C++ use library.vc repository.

## Does not implemented yet
* array
* fixed
* value types
* DII/DSI

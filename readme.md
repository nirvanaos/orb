## ORB IDL support library
This repository contains source code for Nirvana ORB IDL support library.

### How to use
The main header for clients is *Include/CORBA/CORBA.h*

Namespace `CORBA` contains definitions compliant to the CORBA C++ mapping specification.

Namespace `CORBA::Nirvana` contains internal implementation details and is not intended
for usage by an application programmer.

The main header for servers is *Include/CORBA/Server.h*

Namespace `PortableServer` contains definitions compliant to the CORBA C++ mapping specification.

### How to build
This repository does not contain any project files.
It must be included as a Git submodule in a Nirvana library project.
To build the library under Visual C++ use Library.VC repository.

### Does not implemented yet
* array
* fixed
* valuebase
* DII/DSI

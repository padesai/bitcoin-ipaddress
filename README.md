Team 9 Edits
============
 
net_processing.cpp
------------------
The logic to handle capturing transaction hashes and associating them with an IP address happens here. Nearly all changes are in the section devoted to processing "INV" messages from a peer starting around line 1579. While we added logic to attempt to associate IP addresses to hashes that weren't seen first on an inbound connection, we only used "exact" matches for our final analysis.

net.cpp
-------
Made significant re-writes to this code start around line 1182. The FD_SET/select API only scales to 1024 connections so that logic was replaced with the poll API. Also added code starting around 1667 to allow the client to read IP addresses from a file and attempt to connect to them first.

netbase.cpp
-----------
Changes starting near line 233 dealing with the same FD_SET/select API limitations as described above

net.h
-----
Changed constants than control maximum number of connected peers

util.h
------
Added debug output to validate that the node is starting with a high enough file descriptor limit to allow for enough sockets.

compat.h
--------
Disabled a check that the socket file descriptor was less than FD_SETSIZE (not applicable once FD_SET API was no longer used)

Transaction Hashmap
-------------------

cdonlan3/hashmap_transaction_saving.h (no coding done in .cpp--just used the header file)

classes:
--------

TransactionDataUnit: generic storage struct with serialization protocol (for boost serialization)
that is capable of storing individual transactions. 

TransactionData: hashmap of transaction data units; stores a stack (specified by an upper limit) of
transactions; also includes serialization protocol

TransactionDataWrapper: provides an API for single API-call storage and serialization. 


Compile
-------
Follow the instructions in the build-unix.md file in the docs folder to set up a development environment under Linux. Note that several packages must be installed before attempting to compile. We recommend disabling as many optional modules in the configuration as possible such as the example listed for Arch Linux: ./configure --disable-wallet --without-gui --without-miniupnpc




Bitcoin Core integration/staging tree
=====================================

[![Build Status](https://travis-ci.org/bitcoin/bitcoin.svg?branch=master)](https://travis-ci.org/bitcoin/bitcoin)

https://bitcoincore.org

What is Bitcoin?
----------------

Bitcoin is an experimental digital currency that enables instant payments to
anyone, anywhere in the world. Bitcoin uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing money are carried
out collectively by the network. Bitcoin Core is the name of open source
software which enables the use of this currency.

For more information, as well as an immediately useable, binary version of
the Bitcoin Core software, see https://bitcoin.org/en/download, or read the
[original whitepaper](https://bitcoincore.org/bitcoin.pdf).

License
-------

Bitcoin Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/bitcoin/bitcoin/tags) are created
regularly to indicate new official, stable release versions of Bitcoin Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

The developer [mailing list](https://lists.linuxfoundation.org/mailman/listinfo/bitcoin-dev)
should be used to discuss complicated or controversial changes before working
on a patch set.

Developer IRC can be found on Freenode at #bitcoin-core-dev.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/qa) of the RPC interface, written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/qa) are installed) with: `qa/pull-tester/rpc-tests.py`

The Travis CI system makes sure that every pull request is built for Windows, Linux, and OS X, and that unit/sanity tests are run automatically.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

Translations
------------

Changes to translations as well as new translations can be submitted to
[Bitcoin Core's Transifex page](https://www.transifex.com/projects/p/bitcoin/).

Translations are periodically pulled from Transifex and merged into the git repository. See the
[translation process](doc/translation_process.md) for details on how this works.

**Important**: We do not accept translation changes as GitHub pull requests because the next
pull from Transifex would automatically overwrite them again.

Translators should also subscribe to the [mailing list](https://groups.google.com/forum/#!forum/bitcoin-translators).

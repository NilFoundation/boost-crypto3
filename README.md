# Boost.Crypto3
[![Build Status](https://travis-ci.com/NilFoundation/boost-crypto3.svg?branch=master)](https://travis-ci.com/NilFoundation/boost-crypto3)

Boost.Crypto3 is a header-only concept-based C++ cryptography library derived from
 [=nil; Crypto3 C++ Cryptography Suite](https://github.com/nilfoundation/crypto3.git) 
 designed to be included in [Boost](https://boost.org).
 
## Repository Structure
 
The original [=nil; Crypto3 C++ Cryptography Suite](https://github.com/nilfoundation/crypto3.git) is a highly
modular one. Every particular cryptography field algorithms family implementation is emplaced in it's separate 
header-only submodule. This library merges those ones, which were selected by Boost community, implements 
Bjam-based build configuration for them, and emplaces in `include/boost/crypto3/` directory. 
Tests from sufficient merged libraries are being emplaced in `test`.
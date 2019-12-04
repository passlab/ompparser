# ompparser: A Standalone and Unified OpenMP Parser

ompparser is a standalone and unified OpenMP parser for both C/C++ and Fortran. It can be used as an independent tool as well as an integral component of an OpenMP compiler. It has syntax and semantics checking functionalitity for OpenMP constructs for validating and verifying usage of existing constructs. The formal grammar included in ompparser, developed with Flex and Bison tools, can help interpretation of the OpenMP standard. The implementation supports the full and latest OpenMP 5.0 and is released as open-source with BSD-license. 


## Build and usage
1. clone the repo, create build folder and use cmake to create Makefile

       git clone https://github.com/passlab/ompparser.git
       mkdir ompparser-build
       cd ompparser-build
       cmake -DCMAKE_INSTALL_PREFIX=../ompparser-install ../ompparser 

2. build and install
     
       make; make install

## omparser API

```
enum OpenMPBaseLang {
    Lang_C,
    Lang_Cplusplus,
    Lang_Fortran,
    Lang_unknown
};

class OpenMPClause : public SourceLocation {
 ...
}
 
class OpenMPDirective : public SourceLocation  {
 ...
}

extern  OpenMPDirective * parseOpenMP(const char *, void * exprParse(const char * expr));

```

## Features and Limitation
1. Full OpenMP 5.0 standard support for both C/C++ and Fortran, including parsing and unparsing
1. Flex lexer rules and Bison grammars for OpenMP 5.0 syntax
1. Intermediate representation of OpenMP constructs
1. Easy-to-use interface to parse OpenMP constructs and to emit OpenMP intermediate representation
1. Syntax checking in grammar, OpenMP parsing and IR construction, and post-parsing
1. Clause normalization, e.g. combining multiple shared clauses into one shared clause
1. Limited semantics checking when a construct use C/C++/Fortran identifiers or expressions
1. Testing driver and test cases for extensive testing of OpenMP constructs (ongoing work)
1. DOT graph output of OpenMP constructs (ongoing work)
1. Conversion between perfectly-nested OpenMP constructs and combined constructs (ongoing work)
1. Being integrated with ROSE compiler (ongoing work)

## Acknowledge and Support
The work has been performed with support from Department of Energy Lawrence Livermore National Laboratory and the National Science Foundation. To cite, please refer to following paper:

[Anjia Wang, Yaying Shi, Xinyao Yi, Yonghong Yan, Chunhua Liao and Bronis R. de Supinski, ompparser: A Standalone and Unified OpenMP Parser, the 15th International Workshop on OpenMP (IWOMP), 11th - 13th September, in Auckland, New Zealand](https://link.springer.com/chapter/10.1007%2F978-3-030-28596-8_10). [The presentation of the paper](http://parallel.auckland.ac.nz/iwomp2019/slides_ompparser.pdf), which is from IWOMP'19 website. 

## Authors

Anjia Wang, Yaying Shi, Xinyao Yi, Yonghong Yan, Chunhua Liao and Bronis R. de Supinski

## Contact
Please contact Yonghong Yan (@yanyh15) from github or gmail. 

## Release

ompparser is released under a BSD license. For more details see the file LICENSE.

LLNL-CODE-798101


# ompparser
Standalone OpenMP parser

## Build
1. clone the repo, create build folder and use cmake to create Makefile

       git clone https://github.com/passlab/ompparser.git
       mkdir ompparser-build
       cd ompparser-build
       cmake -DCMAKE_INSTALL_PREFIX=../ompparser-install ../ompparser 

2. build and install
     
       make; make install

## omparser API

```

public class OMPAttribute {
 OMPAttribute [] children; /* this could be other C/C++ vector */
 OMPAttribute parent;
 const char * lang_str;
   

}

public class OMPDirective extends OMPAttribute {


}

public class OMPClause extends OMPAttribute {

}




OMPAttribute * omp_parse(const char * filename, const line, const char * string, void * node);

```

## Related work
* Check Clang, e.g. https://github.com/llvm-mirror/clang/blob/master/include/clang/AST/OpenMPClause.h, https://github.com/llvm-mirror/clang/blob/master/include/clang/AST/StmtOpenMP.h
* Check Clang for how it easily specifies enums for OpenMP AST/Attribute. 
   * https://github.com/llvm-mirror/clang/blob/master/include/clang/Basic/OpenMPKinds.def
   * https://github.com/llvm-mirror/clang/blob/master/include/clang/Basic/OpenMPKinds.h
   
* Clang AST definition can be used as references for creating OpenMPAttribute class
   * clause AST: https://github.com/llvm-mirror/clang/blob/master/include/clang/AST/OpenMPClause.h
   * directive AST: https://github.com/llvm-mirror/clang/blob/master/include/clang/AST/StmtOpenMP.h
   * expression AST (mainly for array section): https://github.com/llvm-mirror/clang/blob/master/include/clang/AST/ExprOpenMP.h
   * declare AST: https://github.com/llvm-mirror/clang/blob/master/include/clang/AST/DeclOpenMP.h



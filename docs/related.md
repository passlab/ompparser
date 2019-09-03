
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

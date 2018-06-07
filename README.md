# ompparser
Standalone OpenMP parser

## omparser API

```
public class OMPAttribute {
   

}

public class OMPDirective extends OMPAttribute {


}

public class OMPClause extends OMPAttribute {

}




OMPAttribute * omp_parse(const char * filename, const line, const char * string, void * node);

```

## Related work
Check Clang, e.g. https://github.com/llvm-mirror/clang/blob/master/include/clang/AST/OpenMPClause.h, https://github.com/llvm-mirror/clang/blob/master/include/clang/AST/StmtOpenMP.h

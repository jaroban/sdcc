mkdir ..\build\ds390
del ..\build\ds390\libint.lib
..\..\..\bin_vc\sdar.exe -rcD ..\build\ds390\libint.lib obj\_divsint.rel obj\_divuint.rel obj\_modsint.rel obj\_moduint.rel obj\_mulint.rel

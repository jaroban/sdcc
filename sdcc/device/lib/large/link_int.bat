mkdir ..\build\large
del ..\build\large\libint.lib
..\..\..\bin_vc\sdar.exe -rcD ..\build\large\libint.lib obj\_divsint.rel obj\_divuint.rel obj\_modsint.rel obj\_moduint.rel obj\_mulint.rel

mkdir ..\build\ds400
del ..\build\ds400\libint.lib
..\..\..\bin_vc\sdar.exe -rcD ..\build\ds400\libint.lib obj\_divsint.rel obj\_divuint.rel obj\_modsint.rel obj\_moduint.rel obj\_mulint.rel

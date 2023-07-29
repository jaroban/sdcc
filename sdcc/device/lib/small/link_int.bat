mkdir ..\build\small
del ..\build\small\libint.lib
..\..\..\bin_vc\sdar.exe -rcD ..\build\small\libint.lib obj\_divsint.rel obj\_divuint.rel obj\_modsint.rel obj\_moduint.rel obj\_mulint.rel

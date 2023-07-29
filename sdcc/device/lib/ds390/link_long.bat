mkdir ..\build\ds390
del ..\build\ds390\liblong.lib
..\..\..\bin_vc\sdar.exe -rcD ..\build\ds390\liblong.lib obj\_divslong.rel obj\_modslong.rel obj\_modulong.rel obj\_divulong.rel obj\_mullong.rel

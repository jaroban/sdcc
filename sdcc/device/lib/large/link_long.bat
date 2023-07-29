mkdir ..\build\large
del ..\build\large\liblong.lib
..\..\..\bin_vc\sdar.exe -rcD ..\build\large\liblong.lib obj\_divslong.rel obj\_modslong.rel obj\_modulong.rel obj\_divulong.rel obj\_mullong.rel

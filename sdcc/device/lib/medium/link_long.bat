mkdir ..\build\medium
del ..\build\medium\liblong.lib
..\..\..\bin_vc\sdar.exe -rcD ..\build\medium\liblong.lib obj\_divslong.rel obj\_modslong.rel obj\_modulong.rel obj\_divulong.rel obj\_mullong.rel

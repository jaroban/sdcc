mkdir ..\build\small
del ..\build\small\liblong.lib
..\..\..\bin_vc\sdar.exe -rcD ..\build\small\liblong.lib obj\_divslong.rel obj\_modslong.rel obj\_modulong.rel obj\_divulong.rel obj\_mullong.rel

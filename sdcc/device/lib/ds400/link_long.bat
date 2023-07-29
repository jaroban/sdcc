mkdir ..\build\ds400
del ..\build\ds400\liblong.lib
..\..\..\bin_vc\sdar.exe -rcD ..\build\ds400\liblong.lib obj\_divslong.rel obj\_modslong.rel obj\_modulong.rel obj\_divulong.rel obj\_mullong.rel

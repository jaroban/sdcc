mkdir ..\build\ds400
del ..\build\ds400\libds400.lib
..\..\..\bin_vc\sdar.exe -rcD ..\build\ds400\libds400.lib obj\tinibios.rel obj\memcpyx.rel obj\ds400rom.rel

mkdir ..\build\ds390
del ..\build\ds390\libds390.lib
..\..\..\bin_vc\sdar.exe -rcD ..\build\ds390\libds390.lib obj\tinibios.rel obj\memcpyx.rel obj\lcd390.rel obj\i2c390.rel obj\rtc390.rel obj\putchar.rel obj\gptr_cmp.rel obj\atomic_flag_test_and_set.rel obj\atomic_flag_clear.rel

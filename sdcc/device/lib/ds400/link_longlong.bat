mkdir ..\build\ds400
del ..\build\ds400\liblonglong.lib
..\..\..\bin_vc\sdar.exe -rcD ..\build\ds400\liblonglong.lib obj\_rrulonglong.rel obj\_rrslonglong.rel obj\_rlulonglong.rel obj\_rlslonglong.rel obj\_mullonglong.rel obj\_divslonglong.rel obj\_divulonglong.rel obj\_modslonglong.rel obj\_modulonglong.rel

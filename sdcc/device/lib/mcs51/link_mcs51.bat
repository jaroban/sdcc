del mcs51.lib
..\..\..\bin_vc\sdar.exe -rcD mcs51.lib crtstart.rel crtxinit.rel crtxclear.rel crtclear.rel crtpagesfr.rel crtbank.rel crtcall.rel crtxstack.rel crtxpush.rel crtxpushr0.rel crtxpop.rel crtxpopr0.rel gptr_cmp.rel atomic_flag_test_and_set.rel atomic_flag_clear.rel

mkdir ..\build\small
mkdir ..\build\small-stack-auto
mkdir ..\build\medium
mkdir ..\build\large
mkdir ..\build\large-stack-auto
mkdir ..\build\huge

copy mcs51.lib ..\build\small\
copy mcs51.lib ..\build\small-stack-auto\
copy mcs51.lib ..\build\medium
copy mcs51.lib ..\build\large
copy mcs51.lib ..\build\large-stack-auto
copy mcs51.lib ..\build\huge


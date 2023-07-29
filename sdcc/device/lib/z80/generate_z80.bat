@echo off & perl -x %0 & pause & exit
#!perl
use strict;
use warnings;

my @COMMON_FLOAT = qw|
    _atof.c
    _fs2schar.c
    _fs2sint.c
    _fs2slong.c
    _fs2uchar.c
    _fs2uint.c
    _fs2ulong.c
    _fsadd.c
    _fsdiv.c
    _fseq.c
    _fslt.c
    _fsmul.c
    _fsneq.c
    _fssub.c
    _schar2fs.c
    _sint2fs.c
    _slong2fs.c
    _uchar2fs.c
    _uint2fs.c
    _ulong2fs.c
    acosf.c
    asincosf.c
    asinf.c
    atan2f.c
    atanf.c
    ceilf.c
    cosf.c
    coshf.c
    cotf.c
    errno.c
    expf.c
    fabsf.c
    floorf.c
    frexpf.c
    isinf.c
    isnan.c
    ldexpf.c
    log10f.c
    logf.c
    modff.c
    powf.c
    sincosf.c
    sincoshf.c
    sinf.c
    sinhf.c
    sqrtf.c
    tancotf.c
    tanf.c
    tanhf.c 
|;

my @COMMON_LONG = qw|
    _divslong.c
    _modslong.c
    _modulong.c
|;

my @COMMON_SDCC = qw|
    __assert.c
    _memcmp.c
    _memchr.c
    _memset.c
    _strcat.c
    _strcspn.c
    _strchr.c
    _strncat.c
    _strncmp.c
    _strncpy.c
    _strpbrk.c
    _strrchr.c
    _strspn.c
    _strstr.c
    _strtok.c
    aligned_alloc.c
    atoi.c
    atol.c
    atoll.c
    bsearch.c
    btowc.c
    c16rtomb.c
    c16stombs.c
    c32rtomb.c
    calloc.c
    free.c
    gets.c
    isalnum.c
    isalpha.c
    isblank.c
    iscntrl.c
    isdigit.c
    isgraph.c
    islower.c
    isprint.c
    ispunct.c
    isspace.c
    isupper.c
    isxdigit.c
    labs.c
    malloc.c
    mblen.c
    mbrlen.c
    mbrtoc16.c
    mbrtoc32.c
    mbrtowc.c
    mbsinit.c
    mbstoc16s.c
    mbstowcs.c
    mbtowc.c
    memccpy.c
    memset_explicit.c
    printf_large.c
    puts.c
    qsort.c
    rand.c
    realloc.c
    strdup.c
    strndup.c
    strtol.c
    strtoul.c
    strxfrm.c
    time.c
    tolower.c
    toupper.c
    wcrtomb.c
    wcscmp.c
    wcslen.c
    wcstombs.c
    wctob.c
    wctomb.c
|;

# generate z80
my @Z80_FLOAT = @COMMON_FLOAT;

my @Z80_LONG = (@COMMON_LONG, qw|
    _divulong.c
    _mullong.c
|);

my @Z80_LONGLONG = qw|
    _mullonglong.c
    _divslonglong.c
    _divulonglong.c
    _modslonglong.c
    _modulonglong.c
|;

my @Z80_SDCC = (@COMMON_SDCC, qw|
    _startup.c
    sprintf.c
    vprintf.c
    _strcmp.c
    atomic_flag_clear.c
|);

my @Z80_C = (@Z80_FLOAT, @Z80_LONG, @Z80_LONGLONG, @Z80_SDCC);
# my @Z8OBJECTS = $(patsubst %.c,%.rel,$(Z80_FLOAT) $(Z80_INT) $(Z80_LONG) $(Z80_LONGLONG) $(Z80_SDCC))

my @Z80_ASM = qw| 
    divunsigned.s divsigned.s divmixed.s modunsigned.s modsigned.s modmixed.s mul.s mulchar.s
    heap.s memmove.s strcpy.s strlen.s abs.s __sdcc_call_hl.s __sdcc_call_iy.s crtenter.s
    setjmp.s atomic_flag_test_and_set.s memcpy.s __uitobcd.s __ultobcd.s __itoa.s __ltoa.s
    __strreverse.s __sdcc_bcall.s __sdcc_critical.s
|;

my @object_files;

open(my $fh, '>', "z80_lib.vcxproj") or die $!;

print $fh <<END;
<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|Win32">
      <Configuration>Debug</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|Win32">
      <Configuration>Release</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Debug|x64">
      <Configuration>Debug</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|x64">
      <Configuration>Release</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <ItemGroup>
END

foreach my $asm_source (@Z80_ASM)
{
    my $rel_file = $asm_source;
    $rel_file =~ s/\.s$/.rel/;
    push @object_files, $rel_file;
    print $fh <<END;
    <CustomBuild Include="$asm_source">
      <FileType>Document</FileType>
      <Command>..\\..\\..\\bin_vc\\sdasz80.exe -plosgff %(FullPath)</Command>
      <Message>Compiling %(FullPath)</Message>
      <Outputs>%(Filename).rel</Outputs>
    </CustomBuild>
END
}

foreach my $c_source (@Z80_C)
{
    my $rel_file = $c_source;
    $rel_file =~ s/\.c$/.rel/;
    push @object_files, "obj\\$rel_file";
    print $fh <<END;
    <CustomBuild Include="..\\$c_source">
      <FileType>CppCode</FileType>
      <Command>..\\..\\..\\bin_vc\\sdcc.exe -mz80 --max-allocs-per-node 25000 -I..\\..\\include -I. --std-c11 -o obj\\ -c %(FullPath)</Command>
      <Message>Compiling %(FullPath)</Message>
      <Outputs>obj\\%(Filename).rel</Outputs>
    </CustomBuild>
END
}

my $additional_inputs = join(';', @object_files);

print $fh <<END;
    <CustomBuild Include="crt0.s">
      <FileType>Document</FileType>
      <Command>..\\..\\..\\bin_vc\\sdasz80.exe -plosgff %(FullPath)</Command>
      <Message>Compiling %(FullPath)</Message>
      <Outputs>%(Filename).rel</Outputs>
    </CustomBuild>
    <CustomBuild Include="link_z80.bat">
      <FileType>Document</FileType>
      <Command>call %(FullPath)</Command>
      <Message>Linking z80.lib</Message>
      <Outputs>..\\build\\z80\\z80.lib</Outputs>
      <AdditionalInputs>$additional_inputs</AdditionalInputs>
    </CustomBuild>
END

print $fh <<'END';
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <VCProjectVersion>16.0</VCProjectVersion>
    <Keyword>Win32Proj</Keyword>
    <ProjectGuid>{4adfe1ab-c823-494d-8616-1ab9c2e6d428}</ProjectGuid>
    <RootNamespace>z80lib</RootNamespace>
    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'" Label="Configuration">
    <ConfigurationType>Utility</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <PlatformToolset>v142</PlatformToolset>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'" Label="Configuration">
    <ConfigurationType>Utility</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v142</PlatformToolset>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'" Label="Configuration">
    <ConfigurationType>Utility</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <PlatformToolset>v142</PlatformToolset>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'" Label="Configuration">
    <ConfigurationType>Utility</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v142</PlatformToolset>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
  <ImportGroup Label="Shared">
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <LinkIncremental>true</LinkIncremental>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <LinkIncremental>false</LinkIncremental>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <LinkIncremental>true</LinkIncremental>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <LinkIncremental>false</LinkIncremental>
  </PropertyGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>WIN32;_DEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>WIN32;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>_DEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup>
  </ItemGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
</Project>
END

close $fh or die $!;

my $object_files_with_spaces = join(' ', @object_files);

open($fh, '>', "link_z80.bat") or die $!;

print $fh <<END;
mkdir ..\\build\\z80
del ..\\build\\z80\\z80.lib
..\\..\\..\\bin_vc\\sdar.exe -rcD ..\\build\\z80\\z80.lib $object_files_with_spaces
copy crt0.rel ..\\build\\z80\\
END

close $fh or die $!;

=aaa

    <CustomBuild Include="ds390\tinibios.c">
      <FileType>CppCode</FileType>
      <Command Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">..\..\bin_vc\sdcc.exe -c -mds390 -I..\include -I..\include\mcs51 --nostdinc --std-c11 -o ds390\obj\ %(FullPath)</Command>
      <Message Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">Compiling %(FullPath)</Message>
      <Outputs Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">ds390\obj\%(Filename).rel</Outputs>
      <Command Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">..\..\bin_vc\sdcc.exe -c -mds390 -I..\include -I..\include\mcs51 --nostdinc --std-c11 -o ds390\obj\ %(FullPath)</Command>
      <Message Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">Compiling %(FullPath)</Message>
      <Outputs Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">ds390\obj\%(Filename).rel</Outputs>
      <Command Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">..\..\bin_vc\sdcc.exe -c -mds390 -I..\include -I..\include\mcs51 --nostdinc --std-c11 -o ds390\obj\ %(FullPath)</Command>
      <Message Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">Compiling %(FullPath)</Message>
      <Outputs Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">ds390\obj\%(Filename).rel</Outputs>
      <Command Condition="'$(Configuration)|$(Platform)'=='Release|x64'">..\..\bin_vc\sdcc.exe -c -mds390 -I..\include -I..\include\mcs51 --nostdinc --std-c11 -o ds390\obj\ %(FullPath)</Command>
      <Message Condition="'$(Configuration)|$(Platform)'=='Release|x64'">Compiling %(FullPath)</Message>
      <Outputs Condition="'$(Configuration)|$(Platform)'=='Release|x64'">ds390\obj\%(Filename).rel</Outputs>
    </CustomBuild>
  </ItemGroup>
  <ItemGroup>
    <CustomBuild Include="mcs51\atomic_flag_clear.asm">
      <FileType>Document</FileType>
      <Command>..\..\bin_vc\sdas8051.exe -plosgff %(FullPath)</Command>
      <Message>Compiling %(FullPath)</Message>
      <Outputs>mcs51\%(Filename).rel</Outputs>
    </CustomBuild>
=cut

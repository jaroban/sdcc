@echo off & perl -x %0 & pause & exit
#!perl
use strict;
use warnings;
use Data::GUID;
use Data::Dumper;

open(my $fh, '>', "ds390_lib.vcxproj") or die $!;

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

my %sources;

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

$sources{'float'} = [@COMMON_FLOAT, qw|
    _fscmp.c
    _fsget1arg.c
    _fsget2args.c
    _fsnormalize.c
    _fsreturnval.c
    _fsrshift.c
    _fsswapargs.c
    _logexpf.c
|];

$sources{'int'} = [qw|
    _divsint.c
    _divuint.c
    _modsint.c
    _moduint.c
    _mulint.c
|];

my @COMMON_LONG = qw|
    _divslong.c
    _modslong.c
    _modulong.c
|;

$sources{'long'} = [@COMMON_LONG, qw|
    _divulong.c
    _mullong.c
|];

$sources{'longlong'} = [qw|
    _rrulonglong.c
    _rrslonglong.c
    _rlulonglong.c
    _rlslonglong.c
    _mullonglong.c
    _divslonglong.c
    _divulonglong.c
    _modslonglong.c
    _modulonglong.c
|];

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

$sources{'sdcc'} = [@COMMON_SDCC, qw|
    _autobaud.c
    _bp.c
    _decdptr.c
    _gptrget.c
    _gptrgetc.c
    _gptrput.c
    _ser.c
    _setjmp.c
    serial.c
    __itoa.c
    __ltoa.c
    _spx.c
    _startup.c
    _strcmp.c
    _strlen.c
    __memcpy.c
    memcpy.c
    _memmove.c
    _strcpy.c
    _heap.c
    sprintf.c
    vprintf.c
    printf_fast.c
    printf_fast_f.c
    printf_tiny.c
    printfl.c
    bpx.c
|];

$sources{'ds390'} = [qw|
    tinibios.c memcpyx.c lcd390.c i2c390.c rtc390.c putchar.c gptr_cmp.c
    atomic_flag_test_and_set.c atomic_flag_clear.c
|];

# print Dumper(\%sources);

my @C_FILES = (
    (map { "..\\$_" } @{$sources{'float'}}),
    (map { "..\\$_" } @{$sources{'int'}}),
    (map { "..\\$_" } @{$sources{'long'}}),
    (map { "..\\$_" } @{$sources{'longlong'}}),
    (map { "..\\$_" } @{$sources{'sdcc'}}),
    @{$sources{'ds390'}}
);

foreach my $c_source (@C_FILES)
{
    print $fh <<END;
    <CustomBuild Include="$c_source">
      <FileType>CppCode</FileType>
      <Command>..\\..\\..\\bin_vc\\sdcc.exe -mds390 -I. -I..\\..\\include -I..\\..\\include\\mcs51 -I..\\..\\include\\ds390 --std-c11 -o obj\\ -c %(FullPath)</Command>
      <Message>Compiling %(FullPath)</Message>
      <Outputs>obj\\%(Filename).rel</Outputs>
    </CustomBuild>
END
}

foreach my $type (keys %sources)
{
    my @object_files = get_rel_files(@{$sources{$type}});
    my $object_files_with_spaces = join(' ', @object_files);
    my $additional_inputs = join(';', @object_files);

    print $fh <<END;
    <CustomBuild Include="link_$type.bat">
      <FileType>Document</FileType>
      <Command>call %(FullPath)</Command>
      <Message>Linking lib$type.lib</Message>
      <Outputs>..\\build\\ds390\\lib$type.lib</Outputs>
      <AdditionalInputs>$additional_inputs</AdditionalInputs>
    </CustomBuild>
END

    open(my $link, '>', "link_$type.bat") or die $!;
    print $link <<END;
mkdir ..\\build\\ds390
del ..\\build\\ds390\\lib$type.lib
..\\..\\..\\bin_vc\\sdar.exe -rcD ..\\build\\ds390\\lib$type.lib $object_files_with_spaces
END
    close $link or die $!;
}

# rest of project file
my $guid = Data::GUID->new;
$guid = '7383327A-6BF6-1014-A5DE-5CA30E936024'; 
my $namespace = 'ds390lib';

print $fh <<END;
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <VCProjectVersion>16.0</VCProjectVersion>
    <Keyword>Win32Proj</Keyword>
    <ProjectGuid>{$guid}</ProjectGuid>
    <RootNamespace>$namespace</RootNamespace>
    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
  </PropertyGroup>
END
print $fh <<'END';
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


sub get_rel_files {
    return map {
        my $rel_file = $_;
        $rel_file =~ s/\.c$/.rel/;
        "obj\\$rel_file";
    } @_;
}

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

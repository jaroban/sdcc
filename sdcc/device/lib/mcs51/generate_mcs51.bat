@echo off & perl -x %0 & pause & exit
#!perl
use strict;
use warnings;
use Data::GUID;

my @ASM_FILES = qw|
    crtstart.asm crtxinit.asm crtxclear.asm crtclear.asm
    crtpagesfr.asm crtbank.asm crtcall.asm
    crtxstack.asm crtxpush.asm crtxpushr0.asm crtxpop.asm crtxpopr0.asm
    gptr_cmp.asm atomic_flag_test_and_set.asm atomic_flag_clear.asm
|;

my @object_files;

open(my $fh, '>', "mcs51_lib.vcxproj") or die $!;

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

foreach my $asm_source (@ASM_FILES)
{
    my $rel_file = $asm_source;
    $rel_file =~ s/\.asm$/.rel/;
    push @object_files, $rel_file;
    print $fh <<END;
    <CustomBuild Include="$asm_source">
      <FileType>Document</FileType>
      <Command>..\\..\\..\\bin_vc\\sdas8051.exe -plosgff %(FullPath)</Command>
      <Message>Compiling %(FullPath)</Message>
      <Outputs>%(Filename).rel</Outputs>
    </CustomBuild>
END
}

my $additional_inputs = join(';', @object_files);

print $fh <<END;
    <CustomBuild Include="link_mcs51.bat">
      <FileType>Document</FileType>
      <Command>call %(FullPath)</Command>
      <Message>Linking mcs51.lib</Message>
      <Outputs>mcs51.lib</Outputs>
      <AdditionalInputs>$additional_inputs</AdditionalInputs>
    </CustomBuild>
END

my $guid = Data::GUID->new;
$guid = 'D78D25B4-6C11-1014-B6E8-58EC0E936024'; 
my $namespace = 'mcs51lib';

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

my $object_files_with_spaces = join(' ', @object_files);

open($fh, '>', "link_mcs51.bat") or die $!;

print $fh <<END;
del mcs51.lib
..\\..\\..\\bin_vc\\sdar.exe -rcD mcs51.lib $object_files_with_spaces

mkdir ..\\build\\small
mkdir ..\\build\\small-stack-auto
mkdir ..\\build\\medium
mkdir ..\\build\\large
mkdir ..\\build\\large-stack-auto
mkdir ..\\build\\huge

copy mcs51.lib ..\\build\\small\\
copy mcs51.lib ..\\build\\small-stack-auto\\
copy mcs51.lib ..\\build\\medium
copy mcs51.lib ..\\build\\large
copy mcs51.lib ..\\build\\large-stack-auto
copy mcs51.lib ..\\build\\huge

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

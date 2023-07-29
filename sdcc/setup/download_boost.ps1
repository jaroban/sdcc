# you may have to run:
# Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy Unrestricted
#
# TODO: check for updates at https://www.boost.org
# if the version changes, also update the include path in \sdcc\SDCC.props !
$version = '1_82_0'

$version_with_dots = $version.replace('_', '.')
# Source file location
$source_url = "https://boostorg.jfrog.io/artifactory/main/release/$version_with_dots/source/boost_$version.7z"
# Destination to save the file
$boost_folder = "C:/boost"
$boost_zipped = "$boost_folder/boost_$version.7z"

if(!(test-path $boost_folder))
{
    New-Item -ItemType Directory -Force -Path $boost_folder
}

if (-not(Test-Path -Path "$boost_folder/boost_$version/boost" -PathType Container)) {
    if (-not(Test-Path -Path $boost_zipped -PathType Leaf)) {
        # Download the file
        "downloading $source_url"
        Invoke-WebRequest -Uri $source_url -OutFile $boost_zipped
    }
    else {
        "$boost_zipped already exists"
    }
    
    # Extract boost
    & "C:/Program Files/7-Zip/7z.exe" x $boost_zipped "-o$boost_folder"
}
else {
    "$boost_zipped is already extracted"
}

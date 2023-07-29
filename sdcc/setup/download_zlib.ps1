# you may have to run:
# Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy Unrestricted
#
# TODO: check for updates at https://zlib.net
# if the version changes, also update the include path in .vcxproj and .props files
$version = '1.2.13'
$version_without_dots = '1213'

# Source file location
$source_url = "https://zlib.net/zlib$version_without_dots.zip"
# Destination to save the file
$zlib_folder = "C:/zlib"
$zlib_zipped = "$zlib_folder/zlib$version_without_dots.zip"

if(!(test-path $zlib_folder))
{
    New-Item -ItemType Directory -Force -Path $zlib_folder
}

if (-not(Test-Path -Path "$zlib_folder/zlib-$version/README" -PathType Leaf)) {
    if (-not(Test-Path -Path $zlib_zipped -PathType Leaf)) {
        # Download the file
        "downloading $source_url"
        Invoke-WebRequest -Uri $source_url -OutFile $zlib_zipped
    }
    else {
        "$zlib_zipped already exists"
    }
    
    # Extract zlib
    & "C:/Program Files/7-Zip/7z.exe" x $zlib_zipped "-o$zlib_folder"

    # these project files were updated to VS community 2019
    Copy-Item zlibstat.vcxproj $zlib_folder/zlib-$version/contrib/vstudio/vc14/
    Copy-Item zlibvc.vcxproj $zlib_folder/zlib-$version/contrib/vstudio/vc14/
}
else {
    "$zlib_zipped is already extracted"
}

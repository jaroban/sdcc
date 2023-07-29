# you may have to run:
# Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy Unrestricted
# Unblock-File <this script>
#
# TODO: check for updates at https://github.com/mbuilov/gawk-windows
$version = '06102020'

# Source file location
$source_url = "https://github.com/mbuilov/gawk-windows/releases/download/$version/msvc_cl_64_rel_c.zip"
# Destination to save the file
$utils_folder = "C:/gawk"
$zipped = "$utils_folder/msvc_cl_64_rel_c.zip"
# Folder where files are extracted
$extracted_folder = "$utils_folder/msvc_cl_64_rel_c"

if(!(test-path $utils_folder))
{
    New-Item -ItemType Directory -Force -Path $utils_folder
}

if (-not(Test-Path -Path "$utils_folder/gawk.exe" -PathType Leaf)) {
    if (-not(Test-Path -Path $extracted_folder -PathType Container)) {
        if (-not(Test-Path -Path $zipped -PathType Leaf)) {
            # Download the file
            "downloading $source_url"
            Invoke-WebRequest -Uri $source_url -OutFile $zipped
        }
        else {
            "$zipped already exists"
        }

        # Extract
        & "C:/Program Files/7-Zip/7z.exe" x $zipped "-o$utils_folder"
    }
    else {
        "$zipped is already extracted"
    }

    # move one level up
    Get-ChildItem -Path $extracted_folder -Recurse | Move-Item -Destination $utils_folder

    # delete folder
    Remove-Item $extracted_folder
}
else {
    "Files are already moved"
}

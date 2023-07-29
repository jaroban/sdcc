# you may have to run:
# Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy Unrestricted
#
# TODO: check for updates at https://github.com/lexxmark/winflexbison
$version = '2.5.25'

# Source file location
$source_url = "https://github.com/lexxmark/winflexbison/releases/download/v$version/win_flex_bison-$version.zip"
# Destination to save the file
$utils_folder = "C:/win_flex_bison"
$zipped = "$utils_folder/win_flex_bison-$version.zip"

if(!(test-path $utils_folder))
{
    New-Item -ItemType Directory -Force -Path $utils_folder
}

if (-not(Test-Path -Path "$utils_folder/bison.exe" -PathType Leaf)) {
    if (-not(Test-Path -Path "$utils_folder/win_bison.exe" -PathType Leaf)) {
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

    # Rename
    Copy-Item $utils_folder/win_bison.exe $utils_folder/bison.exe
    Copy-Item $utils_folder/win_flex.exe $utils_folder/flex.exe
}
else {
    "Files are already renamed"
}

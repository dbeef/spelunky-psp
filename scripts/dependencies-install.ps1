[CmdletBinding()]
param (
    [Parameter()][String]
    $Version="1.2.15",
    
    [Parameter()][String]
    $DepsDirectory="deps"
)

$ErrorActionPreference = "Stop"

Try {
    Try {
        Invoke-WebRequest -Uri "https://www.libsdl.org/release/SDL-devel-$Version-VC.zip" -OutFile "SDL.zip"
    }
    Catch {
        Invoke-WebRequest -Uri "https://osdn.net/frs/g_redir.php?m=kent&f=libsdl%2FSDL%2F$Version%2FSDL-devel-$Version-VC.zip" -OutFile "SDL.zip"
    }
    If (-Not (Test-Path "SDL.zip")) {
        Write-Error "Could not download SDL dependency"
        Exit 1
    }
    
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    If (Test-Path $DepsDirectory) {
        Remove-Item "$DepsDirectory\SDL*" -Recurse -Force
    }

    [System.IO.Compression.ZipFile]::ExtractToDirectory("SDL.zip", "$DepsDirectory\")
    
    New-Item -Path "$DepsDirectory\SDL\include\SDL" -ItemType "Directory"
    Move-Item -Path "$DepsDirectory\SDL-$Version\lib" -Destination "$DepsDirectory\SDL\" -Force
    Move-Item -Path "$DepsDirectory\SDL-$Version\include\*.h" -Destination "$DepsDirectory\SDL\include\SDL\" -Force
    Remove-Item -Path "$DepsDirectory\SDL-$Version" -Force -Recurse
}
Catch {
    
}
Finally {
    Remove-Item -path "SDL.zip"
}

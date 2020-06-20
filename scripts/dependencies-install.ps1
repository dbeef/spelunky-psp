[CmdletBinding()]
param (
    [Parameter()][String]
    $Version="2.0.12",
    
    [Parameter()][String]
    $DepsDirectory="deps"
)

$ErrorActionPreference = "Stop"

Try {
    Try {
        Invoke-WebRequest -Uri "https://www.libsdl.org/release/SDL2-devel-$Version-VC.zip" -OutFile "SDL.zip"
    }
    Catch {
        Invoke-WebRequest -Uri "https://osdn.net/frs/g_redir.php?m=kent&f=libsdl%2FSDL%2F$Version%2FSDL-devel-$Version-VC.zip" -OutFile "SDL.zip"
    }
    If (-Not (Test-Path "SDL.zip")) {
        Write-Error "Could not download SDL2 dependency"
        Exit 1
    }
    
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    If (Test-Path $DepsDirectory) {
        Remove-Item "$DepsDirectory\SDL*" -Recurse -Force
    }

    [System.IO.Compression.ZipFile]::ExtractToDirectory("SDL.zip", "$DepsDirectory\")
    
    Move-Item -Path "$DepsDirectory\SDL2-$Version\lib" -Destination "$DepsDirectory\SDL2\" -Force
    Move-Item -Path "$DepsDirectory\SDL2-$Version\include\*.h" -Destination "$DepsDirectory\SDL2\include\" -Force
    Remove-Item -Path "$DepsDirectory\SDL-$Version" -Force -Recurse
}
Catch {
    
}
Finally {
    Remove-Item -path "SDL.zip"
}

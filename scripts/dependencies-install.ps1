[CmdletBinding()]
param (
    [Parameter()][String]
    $Version="2.0.12",
    
    [Parameter()][String]
    $DepsDirectory="deps"
)

$ErrorActionPreference = "Stop"

Try {
    Invoke-WebRequest -Uri "https://www.libsdl.org/release/SDL2-devel-$Version-VC.zip" -OutFile "SDL.zip"
    If (-Not (Test-Path "SDL.zip")) {
        Write-Error "Could not download SDL2 dependency"
        Exit 1
    }
    
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    If (Test-Path "$DepsDirectory\SDL2") {
        Remove-Item "$DepsDirectory\SDL2" -Recurse -Force
    }

    [System.IO.Compression.ZipFile]::ExtractToDirectory("SDL.zip", "$DepsDirectory\")
    
    Move-Item -Path "$DepsDirectory\SDL2-$Version" -Destination "$DepsDirectory\SDL2" -Force
}
Finally {
    Remove-Item -path "SDL.zip"
}

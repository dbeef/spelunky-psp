[CmdletBinding()]
param (
    [Parameter()][String]
    $SDLVersion="1.2.15",

    [Parameter()][String]
    $SDLMixerVersion="1.2.12",
    
    [Parameter()][String]
    $DepsDirectory="../../tmp/deps-windows"
)

$ErrorActionPreference = "Stop"

# Download and extract SDL artifact:

Try {
    Try {
        Invoke-WebRequest -Uri "https://www.libsdl.org/release/SDL-devel-$SDLVersion-VC.zip" -OutFile "SDL.zip"
    }
    Catch {
        Invoke-WebRequest -Uri "https://osdn.net/frs/g_redir.php?m=kent&f=libsdl%2FSDL%2F$SDLVersion%2FSDL-devel-$SDLVersion-VC.zip" -OutFile "SDL.zip"
    }
    If (-Not (Test-Path "SDL.zip")) {
        Write-Error "Could not download SDL dependency"
        Exit 1
    }
    
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    If (Test-Path $DepsDirectory) {
        Remove-Item "$DepsDirectory\SDL*" -Recurse -Force
    }

	$SDLArchivePath = (Join-Path -Path (Get-Location) -ChildPath "SDL.zip")
	$DepsFullPath = (Join-Path -Path (Get-Location) -ChildPath $DepsDirectory)
	
    [System.IO.Compression.ZipFile]::ExtractToDirectory($SDLArchivePath, $DepsFullPath)
    
    New-Item -Path "$DepsDirectory\SDL\include\SDL" -ItemType "Directory"
    Move-Item -Path "$DepsDirectory\SDL-$SDLVersion\lib" -Destination "$DepsDirectory\SDL\" -Force
    Move-Item -Path "$DepsDirectory\SDL-$SDLVersion\include\*.h" -Destination "$DepsDirectory\SDL\include\SDL\" -Force
    Remove-Item -Path "$DepsDirectory\SDL-$SDLVersion" -Force -Recurse
}
Catch {
   $_    
}
Finally {
    Remove-Item -path "SDL.zip"
}

# Download and extract SDL_mixer artifact:

Try {
	Try { Invoke-WebRequest -Uri "https://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-devel-$SDLMixerVersion-VC.zip" -OutFile "SDL_mixer.zip" } Catch {}

    If (-Not (Test-Path "SDL_mixer.zip")) {
        Write-Error "Could not download SDL_mixer dependency"
        Exit 1
    }

    $SDLArchivePath = (Join-Path -Path (Get-Location) -ChildPath "SDL_mixer.zip")
    $DepsFullPath = (Join-Path -Path (Get-Location) -ChildPath $DepsDirectory)

    Add-Type -AssemblyName System.IO.Compression.FileSystem
    [System.IO.Compression.ZipFile]::ExtractToDirectory($SDLArchivePath, $DepsFullPath)

    Move-Item -Path "$DepsDirectory\SDL_mixer-$SDLMixerVersion\lib\x86\*" -Destination "$DepsDirectory\SDL\lib\x86" -Force
    Move-Item -Path "$DepsDirectory\SDL_mixer-$SDLMixerVersion\lib\x64\*" -Destination "$DepsDirectory\SDL\lib\x64" -Force
    Move-Item -Path "$DepsDirectory\SDL_mixer-$SDLMixerVersion\include\*" -Destination "$DepsDirectory\SDL\include\SDL\" -Force
    Remove-Item -Path "$DepsDirectory\SDL_mixer-$SDLMixerVersion" -Force -Recurse
}
Catch {
   $_
}
Finally {
    Remove-Item -path "SDL_mixer.zip"
}

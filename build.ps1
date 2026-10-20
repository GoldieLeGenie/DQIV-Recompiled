# Builds the game and writes dist\DragonQuestIV.exe, with every resource packed
# inside: the game's data (assets) and the PC version's additions (addons).
# When assets is incomplete, its data is extracted first from the files in
# ROMandOBBfileHere: the European DS ROM (.nds, or a .zip holding it) and the
# Android version's data file (.obb, optional: HD sound).
# The game keeps its save, settings and journal in dist\saves.
$ErrorActionPreference = 'Stop'

# Visual Studio 2026 or 2022 with its C++ tools (the newest one), found by the Visual Studio
# installer's own tool: its folder, its major version (18 or 17) and CMake's generator for it.
function Find-VisualStudio {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path $vswhere)) { return $null }
    $found = & $vswhere -latest -products * -version '[17.0,19.0)' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -format json | ConvertFrom-Json
    $found = @($found) | Select-Object -First 1
    if (-not $found) { return $null }
    $major = [int]($found.installationVersion -split '\.')[0]
    $generator = if ($major -ge 18) { 'Visual Studio 18 2026' } else { 'Visual Studio 17 2022' }
    return [pscustomobject]@{ Path = $found.installationPath; Major = $major; Generator = $generator }
}

# CMake: on the PATH, installed on its own, or the one Visual Studio comes with; Visual
# Studio 2026 needs CMake 4.2 or later (its generator), so an older one is passed over then.
function Find-CMake($visualStudio) {
    $minimum = if ($visualStudio.Major -ge 18) { [version]'4.2' } else { [version]'3.24' }
    $candidates = @()
    $found = Get-Command cmake -ErrorAction SilentlyContinue
    if ($found) { $candidates += $found.Source }
    $candidates += Join-Path $env:ProgramFiles 'CMake\bin\cmake.exe'
    $candidates += Join-Path $visualStudio.Path 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
    foreach ($candidate in $candidates) {
        if (-not (Test-Path $candidate)) { continue }
        $text = (& $candidate --version | Select-Object -First 1) -replace '^cmake version\s+', ''
        $version = [version](($text -split '-')[0])
        if ($version -ge $minimum) { return $candidate }
    }
    return $null
}

# Started from Explorer (double-click, "Run with PowerShell"): the window stays
# open at the end, so its messages can be read.
function Test-StartedFromExplorer {
    try {
        $parent = (Get-CimInstance Win32_Process -Filter "ProcessId=$PID").ParentProcessId
        $name = (Get-Process -Id $parent -ErrorAction Stop).ProcessName
        return $name -eq 'explorer'
    } catch { return $false }
}

$failed = $false
Push-Location $PSScriptRoot
try {
    $visualStudio = Find-VisualStudio
    if (-not $visualStudio) {
        throw 'Visual Studio 2026 or 2022 with "Desktop development with C++" was not found. Install it (or add that workload in the Visual Studio Installer), then build again.'
    }
    $cmake = Find-CMake $visualStudio
    if (-not $cmake) {
        if ($visualStudio.Major -ge 18) { throw 'CMake 4.2 or later was not found (Visual Studio 2026 needs it). Install it (cmake.org), or the "C++ CMake tools for Windows" component of Visual Studio.' }
        throw 'CMake was not found. Install CMake 3.24 or later (cmake.org), or the "C++ CMake tools for Windows" component of Visual Studio.'
    }
    Write-Host "Visual Studio: $($visualStudio.Path) ($($visualStudio.Generator))"
    Write-Host "CMake: $cmake"

    # A build folder made with another Visual Studio is configured again from scratch.
    $cache = 'build32\CMakeCache.txt'
    if (Test-Path $cache) {
        $previous = (Select-String -Path $cache -Pattern '^CMAKE_GENERATOR:INTERNAL=(.*)$').Matches | ForEach-Object { $_.Groups[1].Value }
        if ($previous -and $previous -ne $visualStudio.Generator) {
            Write-Host "build32 was made with $previous`: configuring it again for $($visualStudio.Generator)"
            Remove-Item $cache
            Remove-Item -Recurse -Force 'build32\CMakeFiles' -ErrorAction SilentlyContinue
            # (the downloaded dependencies' own sub-builds too: they keep their generator)
            Get-ChildItem 'build32\_deps' -Directory -Filter '*-subbuild' -ErrorAction SilentlyContinue | Remove-Item -Recurse -Force
        }
    }
    & $cmake --preset windows-game -G $visualStudio.Generator
    if ($LASTEXITCODE -ne 0) { throw 'Configuration failed (see the messages above)' }
    & $cmake --build --preset windows-game --parallel 8
    if ($LASTEXITCODE -ne 0) { throw 'Build failed (see the messages above)' }
    $pack = '.\build32\Release\dqiv_pack.exe'
    $inbox = 'ROMandOBBfileHere'

    # The game's data, from the player's ROM.
    if (-not (Test-Path 'assets\data\SOUND\sound_data.sdat')) {
        $rom = Get-ChildItem $inbox -Filter *.nds -File -ErrorAction SilentlyContinue | Select-Object -First 1
        if (-not $rom) {
            $zip = Get-ChildItem $inbox -Filter *.zip -File -ErrorAction SilentlyContinue | Select-Object -First 1
            if ($zip) {
                $unzipped = 'build32\rom'
                if (Test-Path $unzipped) { Remove-Item -Recurse -Force $unzipped }
                Expand-Archive -LiteralPath $zip.FullName -DestinationPath $unzipped
                $rom = Get-ChildItem $unzipped -Filter *.nds -File -Recurse | Select-Object -First 1
                if (-not $rom) { throw "$($zip.Name) contains no .nds ROM" }
            }
        }
        if (-not $rom) { throw "Put your European Dragon Quest IV ROM (.nds, or a .zip holding it) in the $inbox folder, then build again." }
        Write-Host "Extracting the game's data from $($rom.Name)"
        & $pack --rom $rom.FullName assets
        if ($LASTEXITCODE -ne 0) { throw 'ROM extraction failed' }
        if ($unzipped) { Remove-Item -Recurse -Force $unzipped }
    }

    # HD sound, from the Android version's data file (optional).
    if (-not (Test-Path 'assets\data\SOUND\MOBILE\snd_vol.dat')) {
        $obb = Get-ChildItem $inbox -Filter *.obb -File -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($obb) {
            Write-Host "Extracting the HD sound from $($obb.Name)"
            & $pack --obb $obb.FullName assets
            if ($LASTEXITCODE -ne 0) { throw 'HD sound extraction failed' }
        } else {
            Write-Warning "No .obb file in $inbox`: the game will use the DS sound."
        }
    }

    New-Item -ItemType Directory -Force dist | Out-Null
    & $pack build32\Release\DragonQuestIV.exe assets addons dist\DragonQuestIV.exe
    if ($LASTEXITCODE -ne 0) { throw 'Packaging failed' }
    Write-Host 'Created dist\DragonQuestIV.exe' -ForegroundColor Green
} catch {
    $failed = $true
    Write-Host ''
    Write-Host "ERROR: $($_.Exception.Message)" -ForegroundColor Red
} finally {
    Pop-Location
}
if (Test-StartedFromExplorer) { Read-Host 'Press Enter to close' | Out-Null }
if ($failed) { exit 1 }

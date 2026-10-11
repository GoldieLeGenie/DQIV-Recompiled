# Builds the game and writes dist\DragonQuestIV.exe, with every resource packed
# inside: the game's data (assets) and the PC version's additions (addons).
# When assets is incomplete, its data is extracted first from the files in
# ROMandOBBfileHere: the European DS ROM (.nds, or a .zip holding it) and the
# Android version's data file (.obb, optional: HD sound).
# The game keeps its save, settings and journal in dist\saves.
$ErrorActionPreference = 'Stop'
$cmake = (Get-Command cmake -ErrorAction SilentlyContinue).Source
if (-not $cmake) { $cmake = Join-Path $env:ProgramFiles 'CMake\bin\cmake.exe' }
Push-Location $PSScriptRoot
try {
    & $cmake --preset windows-game
    if ($LASTEXITCODE -ne 0) { throw 'Configuration failed' }
    & $cmake --build --preset windows-game --parallel 8
    if ($LASTEXITCODE -ne 0) { throw 'Build failed' }
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
    Write-Host 'Created dist\DragonQuestIV.exe'
} finally { Pop-Location }

param()
$ErrorActionPreference='Stop'
if(Get-CimInstance Win32_Process -Filter "name='GearsTactics.exe'"){throw 'Close Gears Tactics before installing.'}
$manifest=Get-Content -Raw -LiteralPath (Join-Path $PSScriptRoot 'manifest.json') | ConvertFrom-Json
$destination=Join-Path $env:LOCALAPPDATA 'GearsTacticsInsaneDBNO\Mod'
$files=@{'InsaneDBNO.dll'=$manifest.dllSHA256;'InsaneDBNO-Launch.exe'=$manifest.launcherSHA256}
foreach($name in $files.Keys){
 $payload=Join-Path $PSScriptRoot $name
 if((Get-FileHash -LiteralPath $payload -Algorithm SHA256).Hash.ToLowerInvariant() -ne $files[$name]){throw "Package hash mismatch: $name"}
}
New-Item -ItemType Directory -Path $destination -Force | Out-Null
foreach($name in $files.Keys){
 $installed=Join-Path $destination $name
 Copy-Item -LiteralPath (Join-Path $PSScriptRoot $name) -Destination $installed -Force
 if((Get-FileHash -LiteralPath $installed -Algorithm SHA256).Hash.ToLowerInvariant() -ne $files[$name]){throw "Installed hash mismatch: $name"}
}
$option='"'+(Join-Path $destination 'InsaneDBNO-Launch.exe')+'" %command%'
$option | Set-Content -LiteralPath (Join-Path $destination 'Steam-Launch-Options.txt') -Encoding UTF8
Write-Host 'Installed outside the game directory. No game DLL or cache changed.'
Write-Host 'Steam > Gears Tactics > Properties > Launch Options:'
Write-Host $option

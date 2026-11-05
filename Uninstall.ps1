param()
$ErrorActionPreference='Stop'
if(Get-CimInstance Win32_Process -Filter "name='GearsTactics.exe'"){throw 'Close Gears Tactics before uninstalling.'}
Write-Host 'First remove only the InsaneDBNO-Launch.exe prefix from Steam Launch Options, retaining any original game arguments.'
$destination=Join-Path $env:LOCALAPPDATA 'GearsTacticsInsaneDBNO\Mod'
$manifest=Get-Content -Raw -LiteralPath (Join-Path $PSScriptRoot 'manifest.json') | ConvertFrom-Json
$files=@{'InsaneDBNO.dll'=$manifest.dllSHA256;'InsaneDBNO-Launch.exe'=$manifest.launcherSHA256}
foreach($name in $files.Keys){
 $installed=Join-Path $destination $name
 if((Test-Path -LiteralPath $installed) -and (Get-FileHash -LiteralPath $installed).Hash.ToLowerInvariant() -ne $files[$name]){throw "Installed file differs; nothing removed: $name"}
}
foreach($name in $files.Keys){$installed=Join-Path $destination $name;if(Test-Path -LiteralPath $installed){Remove-Item -LiteralPath $installed}}
Write-Host 'Mod payload removed. Game files, logs, settings backup and saves retained.'

$ErrorActionPreference = 'Stop'
$repo = Split-Path $PSScriptRoot -Parent
$project = Join-Path $repo '26311027-MoonSeongyoon-VN'
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vs = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $vs) { throw 'Visual Studio C++ tools not found.' }
$vcvars = Join-Path $vs 'VC/Auxiliary/Build/vcvars64.bat'
$lines = & cmd /d /c ('"{0}" >nul && set' -f $vcvars)
if ($LASTEXITCODE -ne 0) { throw 'vcvars64 failed.' }
foreach ($line in $lines) {
    if ($line -match '^([^=]+)=(.*)$') {
        [Environment]::SetEnvironmentVariable($matches[1], $matches[2], 'Process')
    }
}
$build = Join-Path ([IO.Path]::GetTempPath()) ('last-clue-smoke-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $build | Out-Null
Push-Location $build
try {
    $compiler = Join-Path ([Environment]::GetEnvironmentVariable('VCToolsInstallDir', 'Process')) 'bin/Hostx64/x64/cl.exe'
    & $compiler /nologo /EHsc /std:c++17 /Od "/I$project" "/I$project/packages/glc2d.0.1.0.7/build/native/include" "/I$project/packages/Microsoft.DXSDK.D3DX.9.29.952.8/build/native/include" "$PSScriptRoot/ScenePlaySmoke.cpp" "$project/ScenceGamePlay.cpp" "$project/Player.cpp" /Fe:smoke.exe
    if ($LASTEXITCODE -ne 0) { throw 'Smoke test compilation failed.' }
    & ./smoke.exe
    if ($LASTEXITCODE -ne 0) { throw 'Smoke tests failed.' }
} finally { Pop-Location }

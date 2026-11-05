@echo off
setlocal
cd /d "%~dp0"
if not defined VSCMD_VER (
 for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "DBNO_VS=%%i"
)
if not defined VSCMD_VER call "%DBNO_VS%\VC\Auxiliary\Build\vcvars64.bat" -vcvars_ver=14.44 >nul
if errorlevel 1 exit /b 1
if not exist build mkdir build
cl /nologo /LD /std:c++17 /O2 /MT /W4 /WX /EHsc mod.cpp /Fo:build\late_mod.obj /Fe:build\InsaneDBNO.dll /link /DYNAMICBASE /NXCOMPAT /INCREMENTAL:NO /Brepro
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /O2 /MT /W4 /WX /EHsc tests\core_test.cpp /Fe:build\core_test.exe /Fo:build\core_test.obj
if errorlevel 1 exit /b 1
build\core_test.exe
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /O2 /MT /W4 /WX /EHsc launcher.cpp shell32.lib user32.lib /Fo:build\launcher.obj /Fe:build\InsaneDBNO-Launch.exe /link /SUBSYSTEM:WINDOWS /DYNAMICBASE /NXCOMPAT /INCREMENTAL:NO /Brepro
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /O2 /MT /W4 /WX /EHsc tests\load_test.cpp /Fo:build\load_test.obj /Fe:build\load_test.exe
if errorlevel 1 exit /b 1
build\load_test.exe

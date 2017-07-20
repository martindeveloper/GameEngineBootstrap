@echo off

SET Root=%~dp0
SET Platform=%1
SET Configuration=%2
SET Language=%3

echo Compiling %Language% shaders

if "%Configuration%"=="" (
	echo Configration was not set!
	goto Halt
)

if "%Platform%"=="" (
	echo Platform was not set!
	goto Halt
)

if "%Language%"=="" (
	echo Shader language was not set! Should be GLSL or HLSL
	goto Halt
)

SET FxcExecutable="C:\Program Files (x86)\Windows Kits\10\bin\x64\fxc.exe"
SET ShadersDirectory="%Root%..\Binaries\%Platform%\%Configuration%\Shaders\."

for /f %%f in ('dir /b %ShadersDirectory%') do (
	echo %%f

	if "%Configuration%"=="Debug" (
		fxc /Od /Zi /T ps_5_1 /Fo PixelShader1.fxc PixelShader1.hlsl
	)
	if "%Configuration%"=="Release" (
		fxc /T ps_5_1 /Fo PixelShader1.fxc PixelShader1.hlsl
	)
)

goto Halt

:Halt
pause
exit


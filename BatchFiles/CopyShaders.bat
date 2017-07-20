@echo off

SET Root=%~dp0
SET Platform=%1
SET Configuration=%2
SET Language=%3

echo Copying %Language% shaders

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

xcopy /s "%Root%..\Source\Shaders\%Language%\." "%Root%..\Binaries\%Platform%\%Configuration%\Shaders\."

goto Halt

:Halt
pause
exit

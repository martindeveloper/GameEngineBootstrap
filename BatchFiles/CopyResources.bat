@echo off

SET Root=%~dp0
SET Platform=%1
SET Configuration=%2

echo Copying Resources

if "%Configuration%"=="" (
	echo Configration was not set!
	goto Halt
)

if "%Platform%"=="" (
	echo Platform was not set!
	goto Halt
)

xcopy /s "%Root%..\Resources\." "%Root%..\Binaries\%Platform%\%Configuration%\Resources\."

goto Halt

:Halt
pause
exit

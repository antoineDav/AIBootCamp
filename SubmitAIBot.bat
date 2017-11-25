@echo off

set buildTargetArg=%1

echo You are about to create a job and send it to the AIBootCamp game server
echo.
pause
echo.
echo.

REM CLEAN VARIABLES

set GOOGLE_DRIVE_DIR=
set TEAMNAME=
set SEVENZIP_PATH=
set TARGET=

@echo off
call CheckInstall.bat

if ErrorLevel == 1 (
    goto fail
)

call :RetrieveGoogleDrivePath
call :RetrieveTeamName
call :RetrieveBuildTarget

"%PYTHON_PATH%/python" SubmitAIBot.py %TEAMNAME% %TARGET% "%GOOGLE_DRIVE_DIR%" "%SEVENZIP_PATH%"

if ErrorLevel == 1 (
    goto fail
)

goto success


:RetrieveTeamName
    set TEAMDRIVE=%GOOGLE_DRIVE_DIR%\Results\Team.txt
    IF EXIST "%TEAMDRIVE%" (
        FOR /F "usebackq delims== tokens=1,2" %%G IN ("%TEAMDRIVE%") DO set %%G=%%H
    )
    exit /B 0

:RetrieveGoogleDrivePath
    IF EXIST "TeamInfo.team" (
        for /f "delims== tokens=1,2" %%G in (TeamInfo.team) do set %%G=%%H
    )
    exit /B 0

:RetrieveBuildTarget
    
    set TARGET=RELEASE
    if "%buildTargetArg%" == "DEBUG" (
        set TARGET=DEBUG
    )
    if "%buildTargetArg%" == "debug" (
        set TARGET=DEBUG
    )

    rem echo Retrieving Build Target...Success  [%TARGET%]

    exit /B 0

:success
    echo.
    echo SUCCESS : Job queued
    goto end

:fail
    echo.
    echo FAILED : Job creation process failed
    goto end

:end
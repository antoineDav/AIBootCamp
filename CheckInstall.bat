@echo off
REM CLEAN VARIABLES

set targetCheck=%1

set SEVENZIP_PATH=
set GOOGLE_DRIVE_DIR=
set PrerequisitesFound=
set googleDriveWellConfigured=
set TEAMNAME=
set AIBOOTCAMP_ENGINE_DIR=
set AIBOOTCAMP_ENGINE_VERSION=
set PYTHON_PATH=
set CMAKE_PATH=

@echo off
call :RetrieveGoogleDrivePath

echo %targetCheck%

if not defined targetCheck (call :ValidateAllPrerequisites) else (call :ValidateCMakeOnly)

if ErrorLevel == 1 (
    goto fail
)

goto success

:ValidateCMakeOnly
    echo Checking CMAKE Prerequisite

    set PrerequisitesFound=0

    @echo off
    IF NOT EXIST "%CMAKE_PATH%" (
       for /f "delims=" %%a in ('where cmake') do @set CMAKE_PATH=%%a
    )
    IF NOT EXIST "%CMAKE_PATH%" (
       echo    CMake.......................Error
       set PrerequisitesFound=1
    ) else (
       echo    CMake.......................Found   ["%CMAKE_PATH%"]
    )

    exit /B %PrerequisitesFound%

:ValidateAllPrerequisites
    echo Checking Prerequisites

    set PrerequisitesFound=0

    IF NOT EXIST "TeamInfo.team" (
        echo    TeamInfo.info...............Error 
        set PrerequisitesFound=1
    ) else (
        echo    TeamInfo.info...............Found   ["%~dp0TeamInfo.team"]
    )

    @echo off
    IF NOT EXIST "%SEVENZIP_PATH%" (
       for /f "delims=" %%a in ('where 7z') do @set SEVENZIP_PATH=%%a
    )
    IF NOT EXIST "%SEVENZIP_PATH%" (
       echo    7z..........................Error
       set PrerequisitesFound=1
    ) else (
       echo    7z..........................Found   ["%SEVENZIP_PATH%"]
    )

     @echo off
    IF NOT EXIST "%PYTHON_PATH%" (
      for /f "delims=" %%a in ('where python') do @set PYTHON_PATH=%%a
    )
    IF NOT EXIST "%PYTHON_PATH%" (
       echo    Python......................Error
       set PrerequisitesFound=1
    ) else (
       echo    Python......................Found   ["%PYTHON_PATH%"]
    )

    @echo off
    IF NOT EXIST "%CMAKE_PATH%" (
       for /f "delims=" %%a in ('where cmake') do @set CMAKE_PATH=%%a
    )
    IF NOT EXIST "%CMAKE_PATH%" (
       echo    CMake.......................Error
       set PrerequisitesFound=1
    ) else (
       echo    CMake.......................Found   ["%CMAKE_PATH%"]
    )

    set googleDriveWellConfigured=0
    IF EXIST "%GOOGLE_DRIVE_DIR%" (
       IF NOT EXIST "%GOOGLE_DRIVE_DIR%/Results" (
              echo    GOOGLE_DRIVE_DIR.................Error   Results Folder Not Found
              set PrerequisitesFound=1
              set googleDriveWellConfigured=1
       ) ELSE (
            IF NOT EXIST "%GOOGLE_DRIVE_DIR%/Results/Team.txt" (
                  echo    GOOGLE_DRIVE_DIR.................Error   Team.txt File Not Found in Results Folder
                  set PrerequisitesFound=1
                  set googleDriveWellConfigured=1
           )
       )

       IF NOT EXIST "%GOOGLE_DRIVE_DIR%/Jobs" (
              echo    GOOGLE_DRIVE_DIR............Error   Jobs Folder Not Found
              set PrerequisitesFound=1
              set googleDriveWellConfigured=1
       )
    ) ELSE (
       echo    GOOGLE_DRIVE_DIR............Error   Folder Not Found
    )
       
     IF %googleDriveWellConfigured% == 0 (
       echo    GOOGLE_DRIVE_DIR............Found   ["%GOOGLE_DRIVE_DIR%"]
       call :RetrieveTeamName
    ) 

    IF "%TEAMNAME%" == "" (
       echo    TEAMNAME....................Error   Team Name Not Found
       set PrerequisitesFound=1
    ) ELSE (
       echo    TEAMNAME....................Found   ["%TEAMNAME%"]
    )

    IF "%AIBOOTCAMP_ENGINE_DIR%" == "" (
       echo    AIBOOTCAMP_ENGINE_DIR.......Error   AIBOOTCAMP Engine Not Found
       set PrerequisitesFound=1
    ) ELSE (
       echo    AIBOOTCAMP_ENGINE_DIR.......Found   ["%AIBOOTCAMP_ENGINE_DIR%"]
    )

    call :RetrieveEngineInfo
    IF "%AIBOOTCAMP_ENGINE_VERSION%" == "" (
       echo    AIBOOTCAMP_ENGINE_VERSION...Error   AIBOOTCAMP Engine Not Found
       set PrerequisitesFound=1
    ) ELSE (
       echo    AIBOOTCAMP_ENGINE_VERSION...Found   ["%AIBOOTCAMP_ENGINE_VERSION%"]
    )

    exit /B %PrerequisitesFound%

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

:RetrieveEngineInfo
    set AIBOOTCAMP_ENGINE_VERSION_FILE=%AIBOOTCAMP_ENGINE_DIR%\Version.txt
    IF EXIST "%AIBOOTCAMP_ENGINE_VERSION_FILE%" (
        FOR /F "usebackq delims== tokens=1,2" %%G IN ("%AIBOOTCAMP_ENGINE_VERSION_FILE%") DO set %%G=%%H
    )
    exit /B 0

:success
    echo.
    echo    Checking Prerequisites...SUCCESS
    goto end

:fail
    echo.
    echo    Checking Prerequisites...FAILED
    goto end

:end
@echo off
title RealArcade Wrapper Killer v%rawkver%    (.-+'~^-+ Checking Override Flags +-^~`+-.)     [...cRypTiCwaRe 2o16...]

::-----------------------------------------------------------------------------------
:: Zylom Main Override Menu
::-----------------------------------------------------------------------------------


:menu

cls
echo -------------------------------------------------------
echo Press one of the keys to select a new option if needed
echo -------------------------------------------------------
echo.
echo.
echo Press (O) to restore the ORIGINAL wrapped EXE
echo.
echo Press (C) to Clear RAWK Cache (cleans old updates and INI files)
echo.
echo Press (S) to Skip The Default INI File
echo.
echo.
echo Press (W) to Launch the Web Downloader (Requires .NET 4.0)
echo.
echo.
echo.
echo Press (A) for ADVANCED Loading Options
echo.
echo Press (D) to Enable Debug Output To Text File
echo.

if %os%==XP choice /c:nqocsaduw /t:n,5 /n
if %os%==VISTA choice /c nqocsaduw /d n /n /t 5
if errorlevel 9 goto uradl
if errorlevel 8 goto NEWOPTION
if errorlevel 7 goto dbgon
if errorlevel 6 goto advon
if errorlevel 5 goto inion
if errorlevel 4 goto cacheon
if errorlevel 3 goto restoreon
if errorlevel 2 goto quwon
if errorlevel 1 goto end


:advon
set advancedloadoptions=1
goto end

:::rauninst
::set rauninstall=1
::call "core_config_ra_player.cmd"
::goto end

:NEWOPTION

goto end

:quwon
set quw=1
goto end

:dbgon
set debug=1
goto end

:inion
set iniskip=1
goto end

:::updateon
::set updatedb=1
::goto end

:restoreon
set wrestore=1
goto end

:rgspreon
set rgspre=1
goto end

:cacheon
set clearcache=1
goto end

:: URA Web Downloader (added 20130611)
:uradl
set launchwebdl=1
goto end


:end

:: FORCE DEBUG MODE (USED ONLY FOR DEBUG RELEASE)
::set debug=1



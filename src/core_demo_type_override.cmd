@echo off
title RealArcade Wrapper Killer v%rawkver%    (.-+'~^-+ Demo Type Override +-^~`+-.)     [...cRypTiCwaRe 2o16...]


::-----------------------------------------------------------------------------------
:: Forcing the CMD window to focus on top!!
::-----------------------------------------------------------------------------------

%show% "Demo Type Override"
%waitfor% 2000
%show% "Demo Type Override"
%show% "Demo Type Override"


::-----------------------------------------------------------------------------------
:: Menu for selecting Demo Type
::-----------------------------------------------------------------------------------

cls
echo Current Demo Type: %demotype%
echo.
echo.
echo Enter The New Demo Type Below, Then Press Enter 
echo.
echo Valid Entries: [M] [0] [1] [2] [3] [4] [5] [6] [G]
echo                        [1a]    [3a]
echo.
echo.
echo.
echo.
echo A value of "M" will force manual unpacking.
echo.
echo A value of "G" forces a Gamehouse 2.1 detection
echo.
echo A value of "0" will force automatic detection.
echo.
echo.
set /p demotype=
echo.
echo.

::-----------------------------------------------------------------------------------
:: added 20110817
:: If manual mode is selected from here only, the "manualdto" flag is set to "1"
::-----------------------------------------------------------------------------------

if %demotype%==m set manualdto=1
if %demotype%==M set manualdto=1

goto end


:end

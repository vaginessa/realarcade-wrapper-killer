@echo off

title RealArcade Wrapper Killer v%rawkver%    (.-+'~^-+ Shortcut Maker +-^~`+-.)     [...cRypTiCwaRe 2o16...]

color 02


:scstart

goto end



:debugok
::-----------------------------------------------------------------------------------
:: Debug Output (USED FOR TESTING ONLY!)
::-----------------------------------------------------------------------------------

cls
call "core_debug.bat"

goto end2


:end


::Cleaning temp INI files for patches and cheats
call "core_clean_ini.bat"

::Added autolaunch select on 11-13-2010
::if %unpackfail%==0 call "core_autolaunch.bat"
::if %unpackfail%==1 call "demoGH.bat"








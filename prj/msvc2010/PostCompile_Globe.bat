@echo off
call PostCompile_Common.bat %*

:: publish library
copy /Y "%~dp0\%buildtype%\*.lib" "%dir_lib%"
copy /Y "%~dp0\%buildtype%\*.dll" "%dir_lib%"

@echo off
call PostCompile_Common.bat %*

:: publish library
if exist "%~dp0\%buildtype%\*.lib" copy /Y "%~dp0\%buildtype%\*.lib" "%dir_lib%"
if exist "%~dp0\%buildtype%\*.dll" copy /Y "%~dp0\%buildtype%\*.dll" "%dir_lib%"

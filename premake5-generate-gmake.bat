@echo off
call "premake5.exe" gmake
ping -n 2 127.0.0.1>nul
@echo off
@call "premake5.exe" vs2022
@ping -n 2 127.0.0.1>nul
@echo off
cd C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.37.32822\bin\Hostx64\x64
dumpbin.exe /dependents C:\dev\BattleFriends\bin\x64\Debug\BattleFriends.exe
dumpbin.exe /dependents C:\dev\BattleFriends\bin\x64\Release\BattleFriends.exe
pause
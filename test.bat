::Batch testing file
::Created by Daniel Brenot and John Pilon for testing scanner.exe
@echo off

"./compiled/scanner.exe" a2empty.pls> ./output/a2empty.out 2> ./error/a2empty.out
"./compiled/scanner.exe" a2error.pls> ./output/a2error.out 2> ./error/a2error.out
"./compiled/scanner.exe" ass2r.pls>  ./output/ass2r.out 2> ./error/ass2r.out
"./compiled/scanner.exe" ass2w.pls> ./output/ass2w.out 2> ./error/ass2w.out

fc /B "%cd%\output\a2empty.out" "%cd%\expected\a2empty.out" >nul
if %errorlevel%==1 (echo a2empty.out: Fail)
if %errorlevel%==0 (echo a2empty.out: Pass)
fc /B "%cd%\output\a2error.out" "%cd%\expected\a2error.out" >nul
if %errorlevel%==1 (echo a2error.out: Fail)
if %errorlevel%==0 (echo a2error.out: Pass)
fc /B "%cd%\output\ass2r.out" "%cd%\expected\ass2r.out" >nul
if %errorlevel%==1 (echo ass2r.out: Fail)
if %errorlevel%==0 (echo ass2r.out: Pass)
fc /B "%cd%\output\ass2w.out" "%cd%\expected\ass2w.out" >nul
if %errorlevel%==1 (echo ass2w.out: Fail)
if %errorlevel%==0 (echo ass2w.out: Pass)
pause
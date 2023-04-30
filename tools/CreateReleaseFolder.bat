
rem xcopy /s c:\source d:\target

rem lets start by deleting the whole CursorLockRelease folder
rmdir /s /q CursorLockerRelease\

rem lets create the folders we need for the release
mkdir CursorLockerRelease\

rem copy the release data to the release folder
xcopy /y ..\build\Release\CursorLocker.exe CursorLockerRelease\
copy NUL CursorLockerRelease\exesConfig.ecfg
xcopy /y ..\README.md CursorLockerRelease\
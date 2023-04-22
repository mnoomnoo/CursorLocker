
rem xcopy /s c:\source d:\target

rem lets start by deleting the whole CursorLockRelease folder
rmdir /s /q CursorLockerRelease\

rem lets create the folders we need for the release
mkdir CursorLockerRelease\

rem copy the release data to the release folder
xcopy /y x64\Release\CursorLocker.exe CursorLockerRelease\
xcopy /y README.md CursorLockerRelease\
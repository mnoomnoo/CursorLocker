# CursorLocker

I wrote this program after playing Rome Total War (2004) and Rome Total War Remastered (2021) on 
a multi-monitor system. In the campaign and battle map when you attempt to move your mouse to the left 
or right of the game screen to scroll the map, sometimes the cursor will go out of bounds of the primary monitor
and show up on the Windows Desktop. If a click occurs while the cursor is out of bounds then Windows
will interact with the click instead of RTW. This makes Windows minimize RTW and from my experience RTW doesn't
like that very much. 

This program fixes this by never allowing the cursor to escape the primary monitor when
playing a specified game. Once the specified game quits then the cursor is unlocked.
 
## How to use
```
USAGE:
  CursorLocker.exe [OPTIONS]
  
OPTIONS:
  -exe "<Executable file>"	Locks the cursor when this exe file becomes a process
  -h, -help					Print version and help info and exits 
```

## Usage example
```
CursorLocker.exe -exe "RomeTW.exe"
```
The above will start CursorLocker.exe and see if RomeTW.exe is currently a process. If it's currently a process then the cursor will be locked to the primary monitor.
If RomeTW.exe isn't currently a process then CursorLocker.exe will wait to lock the cursor until RomeTW.exe is a process

## How to compile and release
Visual Studio 2019 is the IDE used for debug / release builds.
1. Open the Solution file (.sln) in VS 2019
2. Build | Batch Build...
3. Check the Build checkbox for the debug and release configurations for the x64 platform
4. Click Rebuild
5. Go to the project directory and double click CreateReleaseFolder.bat
6. CursorLockerRelease is now the program release folder. Zip and ship this folder

## Behind the scenes
This program uses [ClipCursor](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-clipcursor?f1url=%3FappId%3DDev16IDEF1%26l%3DEN-US%26k%3Dk(WINUSER%252FClipCursor);k(ClipCursor);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue), [OpenWindowStation](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-openwindowstationa?redirectedfrom=MSDN&f1url=%3FappId%3DDev16IDEF1%26l%3DEN-US%26k%3Dk(WINUSER%252FOpenWindowStation);k(OpenWindowStation);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue) and [SetProcessWindowStation](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setprocesswindowstation)

[OpenWindowStation](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-openwindowstationa?redirectedfrom=MSDN&f1url=%3FappId%3DDev16IDEF1%26l%3DEN-US%26k%3Dk(WINUSER%252FOpenWindowStation);k(OpenWindowStation);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue) and [SetProcessWindowStation](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setprocesswindowstation) are used because the CursorLocker process needs the WINSTA_WRITEATTRIBUTES access mask for the current Windows Station so that [ClipCursor](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-clipcursor?f1url=%3FappId%3DDev16IDEF1%26l%3DEN-US%26k%3Dk(WINUSER%252FClipCursor);k(ClipCursor);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue) works.

## Thanks
The CursorLocker icon is derived from an icon found on game-icons.net and made by Lorc, http://lorcblog.blogspot.com
 
## License
MIT

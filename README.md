# CursorLocker

I wrote this program after playing Rome Total War (2004) and Rome Total War Remastered (2021) on 
a multi-monitor system. In the campaign and battle map when you attempt to move your mouse to the left 
or right of the game screen to scroll the map, sometimes the cursor will go out of bounds of the primary monitor
and show up on the Windows Desktop. If a click occurs while the cursor is out of bounds then Windows
will interact with the click instead of RTW. This makes Windows minimize RTW and from my experience RTW doesn't
like that very much. 

This program fixes this by never allowing the cursor to escape the primary monitor when
playing a specified game. Once the specified program quits then the cursor is unlocked.
 
## How to use
```
USAGE:
  CursorLocker.exe [OPTIONS]
  
OPTIONS:
  -exe "<Executable file>"	Locks the cursor when this exe file becomes a process
  -h, -help					Print version and help info and exits 
```
 
## Thanks
The CursorLocker icon is derived from an icon found on game-icons.net and made by Lorc, http://lorcblog.blogspot.com
 
## License
MIT

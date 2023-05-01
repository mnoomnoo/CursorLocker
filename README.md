# CursorLocker

This program locks the cursor to the the primary monitor when the specified executable starts running. Once the specified executable quits then the cursor is unlocked.

On multi-monitor systems the primary monitor shares a vertical or horizontal edge with another monitor. In some fullscreen applications its possible to move your mouse cursor outside the boundary of the primary monitor. If a click occurs while the cursor is outside the boundary of the primary monitor then Windows will sometimes interact with the click instead of the fullscreen application and cause the application to minimize or worse.
 
## Usage:
`CursorLocker.exe <executable_file.exe> [OPTIONS]`

## Options:	
_**-h, -help**_ := Print version and help info then exits  
_**-ec, -exesConfig**_ := Loads the exesConfigs.ecfg file which contains a list of executable files to search for. First exe in that list that becomes a process or is already a process triggers the cursor to lock to the primary monitor. Ignores the <executable_file.exe> passed in.  
_**-sleepTime \<milliseconds\>**_ := How long to sleep before a CursorLocker processing iteration. Default is 800 ms

## Usage example:
`CursorLocker.exe SomeFullscreenApplication.exe`

The above will start CursorLocker.exe and see if SomeFullscreenApplication.exe is currently a process. If it's currently a process then the cursor will immediately be locked to the primary monitor. If SomeFullscreenApplication.exe isn't a process then CursorLocker.exe will wait to lock the cursor until SomeFullscreenApplication.exe becomes a process

## How to build
1. Open the repo folder in your favorite IDE
2. Run cmake to configure the project
3. Build All

## Thanks
The CursorLocker icon is derived from an icon found on game-icons.net and made by Lorc, http://lorcblog.blogspot.com
 
## License
MIT

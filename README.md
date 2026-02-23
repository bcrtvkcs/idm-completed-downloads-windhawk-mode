# IDM Completed Downloads Cleaner

A [Windhawk](https://windhawk.net/) mod that automatically cleans up completed downloads every time you open the Internet Download Manager (IDM) window.

## What Does It Do?

When IDM's main window opens, the mod kicks in and automatically performs the following:

1. **Deletes completed downloads** - Triggers IDM's "Delete completed downloads" command
2. **Auto-dismisses the confirmation dialog** - Hides the confirmation popup and clicks "Yes"
3. **Targets only the main window** - Ignores other IDM windows like Settings, download dialogs, etc.

All of this happens in the background within milliseconds.

## How It Works

The mod hooks the `ShowWindow` function in the Windows API. When IDM makes a window visible:

- Checks if the window's class name is `#32770` (standard dialog class)
- Verifies the window is **ownerless** (no owner) and **has a menu bar**
- Only the IDM main window satisfies both conditions, so other windows are filtered out
- If conditions are met, the cleanup task is launched in a separate thread

## Installation

1. Download and install [Windhawk](https://windhawk.net/)
2. Click the **"New mod"** button in Windhawk
3. Paste the contents of `idm-completed-downloads-cleaner.wh.cpp` into the code area
4. Click the **"Compile"** button
5. Enable the mod

## Requirements

- [Windhawk](https://windhawk.net/)
- [Internet Download Manager (IDM)](https://www.internetdownloadmanager.com/)
- Windows operating system

## Notes

- The mod only runs inside the `idman.exe` process
- The confirmation dialog title is set to English (`Confirm deleting downloads`)
- Users with a different IDM language should change the `POPUP_TITLE` value to match their language

## License

This project is open source. You are free to use and modify it as you wish.

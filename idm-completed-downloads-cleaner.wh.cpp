// ==WindhawkMod==
// @id              idm-completed-downloads-cleaner
// @name            IDM Completed Downloads Cleaner
// @description     This mode cleans up as soon as the IDM window appears on the screen.
// @version         0.5.2
// @author          BCRTVKCS
// @github          https://github.com/bcrtvkcs
// @twitter         https://x.com/bcrtvkcs
// @homepage        https://grdigital.pro
// @include         idman.exe
// @compilerOptions -luser32
// ==/WindhawkMod==

#include <windows.h>
#include <wchar.h>

// ---------------------------------------------------------
// SETTINGS
// ---------------------------------------------------------
#define IDM_CMD_DELETE_COMPLETED 32794
#define IDYES 6  // Standard Windows IDYES button ID

// ---------------------------------------------------------
// HELPER: Find a dialog owned by a specific window
// ---------------------------------------------------------
struct PopupSearch {
    HWND hOwner;
    HWND hResult;
};

BOOL CALLBACK FindOwnedDialog(HWND hWnd, LPARAM lParam) {
    PopupSearch* search = (PopupSearch*)lParam;

    if (GetWindow(hWnd, GW_OWNER) == search->hOwner) {
        char className[64];
        GetClassNameA(hWnd, className, sizeof(className));
        if (strcmp(className, "#32770") == 0) {
            search->hResult = hWnd;
            return FALSE;
        }
    }
    return TRUE;
}

// ---------------------------------------------------------
// HELPER THREAD: Handles Cleanup and Confirmation
// ---------------------------------------------------------
// This function runs right after the window is shown.
DWORD WINAPI CleanupTask(LPVOID lpParam) {
    HWND hMainWnd = (HWND)lpParam;

    // 1. Short delay to let the window fully render
    Sleep(50);

    // 2. Send the cleanup command
    PostMessageA(hMainWnd, WM_COMMAND, IDM_CMD_DELETE_COMPLETED, 0);

    // 3. Find the confirmation dialog by ownership (language-independent)
    for (int i = 0; i < 20; i++) {
        Sleep(50);

        PopupSearch search = { hMainWnd, NULL };
        EnumWindows(FindOwnedDialog, (LPARAM)&search);

        if (search.hResult != NULL) {
            // Hide the popup immediately to keep the UI clean
            ShowWindow(search.hResult, SW_HIDE);

            // Click the "Yes" button
            SendMessageW(search.hResult, WM_COMMAND, IDYES, 0);

            Wh_Log(L"Window opened -> Cleanup done -> Confirmation dismissed.");
            break;
        }
    }
    return 0;
}

// ---------------------------------------------------------
// HOOK: ShowWindow
// ---------------------------------------------------------
// This runs whenever IDM tries to make a window visible.
typedef BOOL (WINAPI *ShowWindow_t)(HWND hWnd, int nCmdShow);
ShowWindow_t ShowWindow_Original;

BOOL WINAPI ShowWindow_Hook(HWND hWnd, int nCmdShow) {
    // First, execute the original operation (let the window open)
    BOOL result = ShowWindow_Original(hWnd, nCmdShow);

    // If the window is being made visible (excluding SW_HIDE and SW_MINIMIZE)
    if (nCmdShow != SW_HIDE && nCmdShow != SW_MINIMIZE) {

        // Is this window the IDM main window?
        char className[256];
        GetClassNameA(hWnd, className, sizeof(className));

        if (strcmp(className, "#32770") == 0) {
            // IDM main window check:
            // 1. Must be ownerless - dialog windows are owned by the main window
            // 2. Must have a menu bar - the main window has Tasks/File/Downloads menus
            HWND hOwner = GetWindow(hWnd, GW_OWNER);
            HMENU hMenu = GetMenu(hWnd);

            if (hOwner == NULL && hMenu != NULL) {
                // Start the cleanup in a separate thread to avoid freezing the UI
                CreateThread(NULL, 0, CleanupTask, (LPVOID)hWnd, 0, NULL);
                Wh_Log(L"IDM main window detected, starting cleanup.");
            }
        }
    }

    return result;
}

// ---------------------------------------------------------
// INITIALIZATION
// ---------------------------------------------------------
BOOL Wh_ModInit() {
    Wh_Log(L"IDM Completed Downloads Cleaner initialized.");

    Wh_SetFunctionHook(
        (void*)ShowWindow,
        (void*)ShowWindow_Hook,
        (void**)&ShowWindow_Original
    );

    return TRUE;
}


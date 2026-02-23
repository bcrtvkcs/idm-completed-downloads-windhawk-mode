// ==WindhawkMod==
// @id              idm-completed-downloads-cleaner
// @name            IDM Completed Downloads Cleaner
// @description     This mode cleans up as soon as the IDM window appears on the screen.
// @version         0.5.1
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
// AYARLAR
// ---------------------------------------------------------
#define IDM_CMD_DELETE_COMPLETED 32794
#define CONFIRM_BTN_ID 6  // Loglardan bulduğumuz ID (Evet)
#define POPUP_TITLE L"İndirmelerin silinmesini onayla"

// ---------------------------------------------------------
// YARDIMCI THREAD: Temizliği ve Onayı Yönetir
// ---------------------------------------------------------
// Bu fonksiyon, pencere açıldıktan hemen sonra çalışır.
DWORD WINAPI CleanupTask(LPVOID lpParam) {
    HWND hMainWnd = (HWND)lpParam;

    // 1. Biraz bekle (Pencere tam çizilsin)
    Sleep(50);

    // 2. Temizleme komutunu gönder
    PostMessageA(hMainWnd, WM_COMMAND, IDM_CMD_DELETE_COMPLETED, 0);

    // 3. Onay kutusu çıkarsa yakala (1 saniye boyunca bekle)
    for (int i = 0; i < 20; i++) {
        Sleep(50);
        HWND hPopup = FindWindowW(NULL, POPUP_TITLE);
        
        if (hPopup != NULL) {
            // Pencereyi hemen gizle (Göz zevkini bozmasın)
            ShowWindow(hPopup, SW_HIDE);
            
            // "Evet" butonuna bas
            SendMessageW(hPopup, WM_COMMAND, CONFIRM_BTN_ID, 0);
            
            Wh_Log(L"Pencere açıldı -> Temizlik yapıldı -> Onay geçildi.");
            break;
        }
    }
    return 0;
}

// ---------------------------------------------------------
// HOOK: ShowWindow
// ---------------------------------------------------------
// IDM ne zaman bir pencereyi "Görünür" yapmaya çalışsa burası çalışır.
typedef BOOL (WINAPI *ShowWindow_t)(HWND hWnd, int nCmdShow);
ShowWindow_t ShowWindow_Original;

BOOL WINAPI ShowWindow_Hook(HWND hWnd, int nCmdShow) {
    // Önce orijinal işlemi yap (Pencere açılsın)
    BOOL result = ShowWindow_Original(hWnd, nCmdShow);

    // Eğer pencere görünür yapılıyorsa (SW_HIDE hariç durumlar)
    if (nCmdShow != SW_HIDE && nCmdShow != SW_MINIMIZE) {

        // Bu pencere IDM'in ana penceresi mi?
        char className[256];
        GetClassNameA(hWnd, className, sizeof(className));

        if (strcmp(className, "#32770") == 0) {
            // IDM ana penceresi kontrolü:
            // 1. Sahipsiz olmalı (owner yok) - diyalog pencereleri ana pencereye aittir
            // 2. Menü çubuğu olmalı - ana pencerede Görevler/Dosya/İndirmeler menüsü var
            HWND hOwner = GetWindow(hWnd, GW_OWNER);
            HMENU hMenu = GetMenu(hWnd);

            if (hOwner == NULL && hMenu != NULL) {
                // Temizlik işlemini ayrı bir thread'de başlat (Arayüzü dondurmamak için)
                CreateThread(NULL, 0, CleanupTask, (LPVOID)hWnd, 0, NULL);
                Wh_Log(L"IDM ana penceresi tespit edildi, temizlik başlatılıyor.");
            }
        }
    }

    return result;
}

// ---------------------------------------------------------
// BAŞLATMA
// ---------------------------------------------------------
BOOL Wh_ModInit() {
    Wh_Log(L"IDM Açılışta Temizleyici Başlatıldı.");

    Wh_SetFunctionHook(
        (void*)ShowWindow,
        (void*)ShowWindow_Hook,
        (void**)&ShowWindow_Original
    );

    return TRUE;
}
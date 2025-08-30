#include "Functions.h"
#include "Melodies.h"
#include <richedit.h>
#include <iostream>
#include <algorithm>
#include <commdlg.h>
#include <cstdio>
#include <cmath>
#include <cwchar>
#include <mmsystem.h>
#include <d2d1.h>
#include <commctrl.h>  // тут оголошення SetWindowSubclass, DefSubclassProc, RemoveWindowSubclass
#pragma comment(lib, "comctl32.lib") // щоб компоновник знайшов код функції
#pragma comment(linker, "/ENTRY:WinMainCRTStartup")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")

// Define global variables
// Source.cpp - Actual definitions
/*
HWND MEdit, VievParamTXT;
RECT clientRect;
OPENFILENAMEW ofn;
wchar_t filename[MAX_PATH] = { 0 };
HBRUSH hEditBrush = NULL, ViewParamBkcl = NULL;
HFONT MEditFont, NPSFont;
bool isVisibleDV = false;
HMENU RootMenu = CreateMenu();
HMENU SubMenuF = CreateMenu();
HMENU SubMenuM = CreateMenu();
HMENU InputMenu = CreateMenu();
HMENU SubMenuC = CreateMenu();
POINT CurPoint;
LPARAM MLp;
const int TXTBufferSize = 65536;
wchar_t Buffer[TXTBufferSize];*/
/*
COLORREF InterpolateColor(COLORREF target, COLORREF current, float t) {
    int r1 = GetRValue(target);
    int g1 = GetGValue(target);
    int b1 = GetBValue(target);

    int r2 = GetRValue(current);
    int g2 = GetGValue(current);
    int b2 = GetBValue(current);

    int r = r2 + (int)((r1 - r2) * t);
    int g = g2 + (int)((g1 - g2) * t);
    int b = b2 + (int)((b1 - b2) * t);

    return RGB(r, g, b);
}
ID2D1Factory* pD2DFactory = nullptr;
ID2D1HwndRenderTarget* pRT = nullptr;
ID2D1SolidColorBrush* pBrush = nullptr;
*/
using namespace std;
//using namespace npos;






wstring replace(const std::wstring& Mtext, const std::wstring& text1, const std::wstring& text2 = NULL) {
    std::wstring result = Mtext;
    size_t pos = result.find(text1);
    while (pos != std::wstring::npos) {
        result.replace(pos, text1.length(), text2);
        pos = result.find(text1, pos + text2.length());
    }
    return result;
}






COLORREF chooseColor(HWND hwndOwner = NULL)
{
    CHOOSECOLOR cc;
    static COLORREF acrCustColors[16];
    ZeroMemory(&cc, sizeof(cc));

    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwndOwner;
    cc.lpCustColors = acrCustColors;
    cc.rgbResult = RGB(0, 0, 0); // Початковий колір
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc)) {
        return cc.rgbResult;
    }
    else {
        return CLR_INVALID; // стандартна константа для помилки
    }
}

void SetOpenFileParams(HWND hwnd) {
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename) / sizeof(wchar_t);
    ofn.lpstrFilter = L"Текстові файли\0*.txt\0Усі файли\0*.*\0";
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

void SaveFile(LPWSTR Path) {
    HANDLE FileToSave = CreateFileW(
        Path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (FileToSave == INVALID_HANDLE_VALUE) {
        MessageBoxW(NULL, L"\u041d\u0435 \u0432\u0434\u0430\u043b\u043e\u0441\u044f \u0441\u0442\u0432\u043e\u0440\u0438\u0442\u0438 \u0444\u0430\u0439\u043b", L"\u041f\u043e\u043c\u0438\u043b\u043a\u0430", MB_OK | MB_ICONERROR);
        return;
    }

    GetWindowTextW(MEdit, Buffer, TXTBufferSize);

    int utf8Length = WideCharToMultiByte(CP_UTF8, 0, Buffer, -1, NULL, 0, NULL, NULL);
    if (utf8Length == 0) {
        MessageBoxW(NULL, L"\u041f\u043e\u043c\u0438\u043b\u043a\u0430 \u043a\u043e\u043d\u0432\u0435\u0440\u0442\u0430\u0446\u0456\u0457 \u0442\u0435\u043a\u0441\u0442\u0443", L"\u041f\u043e\u043c\u0438\u043b\u043a\u0430", MB_OK | MB_ICONERROR);
        CloseHandle(FileToSave);
        return;
    }

    char* utf8Buffer = new char[utf8Length];
    WideCharToMultiByte(CP_UTF8, 0, Buffer, -1, utf8Buffer, utf8Length, NULL, NULL);

    DWORD bytesWritten;
    WriteFile(FileToSave, utf8Buffer, utf8Length - 1, &bytesWritten, NULL);
    CloseHandle(FileToSave);

    delete[] utf8Buffer;
    ProjectPath = Path;
    MessageBoxW(NULL, L"\u0424\u0430\u0439\u043b \u0437\u0431\u0435\u0440\u0435\u0436\u0435\u043d\u043e \u0443\u0441\u043f\u0456\u0448\u043d\u043e!", L"\u0423\u0441\u043f\u0456\u0445", MB_OK | MB_ICONINFORMATION);
    MessageBoxW(NULL, ProjectPath, Path, MB_USERICON);
}



void OpenFile(LPWSTR Path) {
    HANDLE FileToOpen = CreateFileW(
        Path,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (FileToOpen == INVALID_HANDLE_VALUE) {
        MessageBoxW(NULL, L"Не вдалося відкрити файл", L"Помилка", MB_OK | MB_ICONERROR);
        return;
    }

    DWORD fileSize = GetFileSize(FileToOpen, NULL);
    if (fileSize == INVALID_FILE_SIZE || fileSize == 0 || fileSize > 1000000) {
        MessageBoxW(NULL, L"Файл надто великий або помилка розміру", L"Помилка", MB_OK | MB_ICONERROR);
        CloseHandle(FileToOpen);
        return;
    }

    char* rawData = new char[fileSize + 1] {};
    DWORD bytesRead;
    ReadFile(FileToOpen, (LPVOID)rawData, (DWORD)fileSize, &bytesRead, NULL);
    rawData[bytesRead] = '\0';
    CloseHandle(FileToOpen);

    // Конвертація з UTF-8 → UTF-16
    int wcharCount = MultiByteToWideChar(CP_UTF8, 0, rawData, -1, NULL, 0);
    if (wcharCount > 0 && wcharCount < TXTBufferSize) {
        MultiByteToWideChar(CP_UTF8, 0, rawData, -1, Buffer, TXTBufferSize);
        SetWindowTextW(MEdit, Buffer);
    }
    else {
        MessageBoxW(NULL, L"Не вдалося конвертувати текст", L"Помилка", MB_OK | MB_ICONERROR);
    }

    delete[] rawData;
    ProjectPath = Path;
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    // Створення шрифтів
    MEditFont = CreateFontW(22, 10, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FF_DECORATIVE, L"Consolas");

    NPSFont = CreateFontW(18, 9, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FF_DECORATIVE, L"Times New Roman");

    SegouUIFont = CreateFontW(
        16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, FF_SWISS, L"Segoe UI"
    );


    /*KorFont = CreateFontW(19, 19, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FF_DECORATIVE, L"Impact");
*/

    SpeedViewFont = CreateFontW(18, 9, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FF_DECORATIVE, L"Cascadia Code");

    // hBitmapIconCross = (HBITMAP)LoadImageW(NULL, L"Cross_DstWnd.bpm", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);


     /*wstring HHHHJ = replace(L"Hello, World", L"World", L"Fakime!");
     MessageBoxW(NULL, HHHHJ.c_str(), L"MWhhjh", MB_OK);*/

     // Реєстрація класу вікна
    WNDCLASS SoftWareMainClass = NewWindowClass(
        (HBRUSH)COLOR_WINDOW,
        LoadCursor(NULL, IDC_ARROW),
        hInst,
        LoadIcon(NULL, IDI_APPLICATION),  // Виправлено тут
        L"MainWndClass",
        SoftwareMainProcedure
    );

    if (!RegisterClassW(&SoftWareMainClass)) {
        MessageBoxW(NULL, L"Помилка реєстрації класу вікна N", L"Помилка", MB_ICONERROR);
        return -1;
    }

    // Створення головного вікна
    HWND hWnd = CreateWindowExW(WS_EX_ACCEPTFILES,
        L"MainWndClass", L"Noti.p @User |Path",
        WS_POPUP | WS_VISIBLE,
        100, 100, 500, 300,
        NULL, NULL, hInst, NULL);

    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);

    RegisterHotKey(hWnd, 1, 0, VK_F1);  // F1
    RegisterHotKey(hWnd, 2, 0, VK_F2);  // F2
    RegisterHotKey(hWnd, 3, 0, VK_F3);  // F3
    RegisterHotKey(hWnd, 4, 0, VK_F4);  // F4
    RegisterHotKey(hWnd, 5, 0, VK_F5);  // F5
    RegisterHotKey(hWnd, 6, 0, VK_F11);  // F11
    RegisterHotKey(hWnd, 10, MOD_CONTROL, 'A');  // Ctrl + A
    RegisterHotKey(hWnd, 11, MOD_SHIFT, 'C');  // Shift + C

    MSG msg = { 0 };
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
        UpdateWindow(hWnd);         // Оновлює вікно (відправляє WM_PAINT)
    }

    return (int)msg.wParam;

}


void MEditEdited(HWND hWnd) {
    //PlayMP3(L"C:\\Users\\Admin\\OneDrive\\Рабочий стол\\Noti.p\\Noti.p\\G.mp3"); 
            //SmoothBeep(1000, 100, 0.1);
    GetClientRect(hWnd, &clientRect);

    if (isVisibleDV) {
        MoveWindow(MEdit, 0, 30, clientRect.right, clientRect.bottom - 40, TRUE);
        int labelWidth = clientRect.right;
        int labelHeight = 30;

        int x = clientRect.right - labelWidth;
        int y = clientRect.bottom - labelHeight;
        MoveWindow(VievParamTXT, x, y, labelWidth, labelHeight, TRUE);
        ShowWindow(VievParamTXT, SW_SHOW);
    }
    else {
        MoveWindow(MEdit, 0, 30, clientRect.right, clientRect.bottom - 10, TRUE);
        int labelWidth = clientRect.right;
        int labelHeight = 30;

        int x = clientRect.right - labelWidth;
        int y = clientRect.bottom - labelHeight;
        MoveWindow(VievParamTXT, x, y, labelWidth, labelHeight, FALSE);
        ShowWindow(VievParamTXT, SW_HIDE);
    }
    std::wstring TextMEdit = GetWindowTextW(MEdit);
    size_t pos = 0;
    while ((pos = TextMEdit.find(L"\n", pos)) != wstring::npos) {
        TextMEdit.replace(pos, 1, L" ");
        pos += 1;
    }
    std::wstring cb = GetWindowTextW(MEdit); // Assuming MEdit is the HWND for the edit control.  
    std::wstring replacedText = replace(cb, L"\n", L"");
    int currentLength = replacedText.length(); // Use the length of the replaced text instead of calling GetWindowTextLengthW.
    /*if (currentLength > TXTBufferSize) {
        MessageBoxW(hWnd, L"Текст перевищує максимальний розмір буфера!", L"Помилка", MB_OK | MB_ICONERROR);
        return 0;
    }*/
    std::wstring OutputMEdit;

    if (currentLength < 8) {
        OutputMEdit = L"Менше 8 символів.";
    }
    else if (currentLength < 1000) {
        OutputMEdit = L"Кількість символів в Текстовому полі: " + std::to_wstring(currentLength) +
            L" | " + std::to_wstring(currentLength / 8) + L" Байт";
    }
    else if (currentLength < 8000) {
        OutputMEdit = L"Кількість символів в Текстовому полі: " + std::to_wstring(currentLength) +
            L" | " + std::to_wstring(currentLength / 1000) + L" Кілобіт";
    }
    else if (currentLength < 8192) {
        OutputMEdit = L"Кількість символів в Текстовому полі: " + std::to_wstring(currentLength) +
            L" | " + std::to_wstring(currentLength / 8000) + L" Кілобайт";
    }
    else if (currentLength < 1000000) {
        OutputMEdit = L"Кількість символів в Текстовому полі: " + std::to_wstring(currentLength) +
            L" | " + std::to_wstring(currentLength / 8192) + L" Кібібайт";
    }
    else if (currentLength < 1048576) {
        OutputMEdit = L"Кількість символів в Текстовому полі: " + std::to_wstring(currentLength) +
            L" | " + std::to_wstring(currentLength / 1000000) + L" Мегабіт";
    }
    else if (currentLength < 8000000) {
        OutputMEdit = L"Кількість символів в Текстовому полі: " + std::to_wstring(currentLength) +
            L" | " + std::to_wstring(currentLength / 1048576) + L" Мебібіт";
    }
    else if (currentLength < 8388608) {
        OutputMEdit = L"Кількість символів в Текстовому полі: " + std::to_wstring(currentLength) +
            L" | " + std::to_wstring(currentLength / 8000000) + L" Мегабайт";
    }
    else {
        OutputMEdit = L"Кількість символів в Текстовому полі: " + std::to_wstring(currentLength) +
            L" | " + std::to_wstring(currentLength / 1000) + L" Кілобайт (перевищено ліміт)";
    }


    SetWindowTextW(VievParamTXT, OutputMEdit.c_str());
    // MMM
    // MMM
}

// Реалізації функцій
wstring GetWindowTextW(HWND hWnd) {
    int length = ::GetWindowTextLengthW(hWnd);
    if (length <= 0) return L"";

    std::wstring buffer;
    buffer.resize(length + 1);
    ::GetWindowTextW(hWnd, &buffer[0], length + 1);
    buffer.resize(length);
    return buffer;
}

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
    WNDCLASS wc = { 0 };
    wc.hbrBackground = BGColor;
    wc.hCursor = Cursor;
    wc.hInstance = hInst;
    wc.hIcon = Icon;
    wc.lpszClassName = Name;
    wc.lpfnWndProc = Procedure;
    return wc;
}

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    COLORREF EditBackColor = RGB(230, 230, 230);
    COLORREF ViewTextBackColor = RGB(230, 230, 230);
    //  COLORREF ViewParamBkcl = RGB(2, 9, 3);
    switch (msg) {
    case WM_COMMAND:
        if (HIWORD(wp) == EN_CHANGE && (HWND)lp == MEdit) {
            MEditEdited(hWnd); // Виклик функції для обробки змін в MEdit
            return 0; // Додано повернення 0, щоб уникнути подальшої обробки
        }
        else {
            switch (wp) {
                /*COMANDS*/
            case ONSaveFile:
                if (GetSaveFileNameW(&ofn) && filename) { // Додано перевірку filename
                    SaveFile(filename);
                }
                break;
            case ONOpenFile:
                if (GetOpenFileNameW(&ofn)) {
                    OpenFile(filename);
                }
                break;
            case ONMoreMenu:
                MessageBeep(MB_OK);
                if (isVisibleDV) {
                    isVisibleDV = false;
                    InvalidateRect(hWnd, NULL, TRUE); // Оновлюємо вікно
                    ShowWindow(VievParamTXT, SW_HIDE);
                }
                else {
                    isVisibleDV = true;
                    InvalidateRect(hWnd, NULL, TRUE); // Оновлюємо вікно
                    ShowWindow(VievParamTXT, SW_SHOW);
                }
                break;
            case ONClear:
                SetWindowTextW(MEdit, L"");
                break;
            /*case ONShifr: {
                wstring text = GetWindowTextW(MEdit);   
                if (text.empty()) {
                    MessageBoxW(hWnd, L"Поле тексту порожнє!", L"Помилка", MB_OK | MB_ICONERROR);
                }
                else {
                    CriptTextFile(text, 1); // Шифрування
                }
                break;
            }*/
            case ONSteplerEncrypter: {
                wstring text = GetWindowTextW(MEdit);
                if (text.empty()) {
                    MessageBoxW(hWnd, L"Поле тексту порожнє!", L"Помилка", MB_OK | MB_ICONERROR);
                }
                else {
                    CriptTextFile(text, 1, 0); // Шифрування
                }
                break;
            }case ON1213MethodEncrypter: {
                wstring text = GetWindowTextW(MEdit);
                if (text.empty()) {
                    MessageBoxW(hWnd, L"Поле тексту порожнє!", L"Помилка", MB_OK | MB_ICONERROR);
                }
                else {
                    CriptTextFile(text, 2, 0); // Шифрування
                }
                break;
            }case ON562312MethodEncrypter: {
                wstring text = GetWindowTextW(MEdit);
                if (text.empty()) {
                    MessageBoxW(hWnd, L"Поле тексту порожнє!", L"Помилка", MB_OK | MB_ICONERROR);
                }
                else {
                    CriptTextFile(text, 3, 0); // Шифрування
                }
                break;
            }
            
            //Декрипрори

            case ONSteplerDecrypter: {
                wstring text = GetWindowTextW(MEdit);
                if (text.empty()) {
                    MessageBoxW(hWnd, L"Поле тексту порожнє!", L"Помилка", MB_OK | MB_ICONERROR);
                }
                else {
                    CriptTextFile(text, 1, 1); // Декодування
                }
                break;
            }case ON1213MethodDecrypter: {
                wstring text = GetWindowTextW(MEdit);
                if (text.empty()) {
                    MessageBoxW(hWnd, L"Поле тексту порожнє!", L"Помилка", MB_OK | MB_ICONERROR);
                }
                else {
                    CriptTextFile(text, 2, 1); // Декодування
                }
                break;
            }case ON562312MethodDecrypter: {
                wstring text = GetWindowTextW(MEdit);
                if (text.empty()) {
                    MessageBoxW(hWnd, L"Поле тексту порожнє!", L"Помилка", MB_OK | MB_ICONERROR);
                }
                else {
                    CriptTextFile(text, 3, 1); // Декодування
                }
                break;
            }
            case ON_DESTROY_WINDOW_BTN: {
                DestroyWindow(hWnd);
                PostQuitMessage(0);
                return 0;
                break;
            }
            case ON_OVER_WINDOW_BTN: {
                if (isFullScreen) {
                    SetWindowPos(hWnd, HWND_TOP, 100, 100, 500, 300, SWP_SHOWWINDOW);
                    isFullScreen = false;
                }
                else {
                    SetWindowPos(hWnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW);
                    isFullScreen = true;
                }
                InvalidateRect(hWnd, NULL, TRUE); // Оновлюємо вікно
                break;
            }
            case ON_HIDE_WINDOW_BTN: {
                if (IsWindowVisible(hWnd)) {
                    ShowWindow(hWnd, SW_HIDE);
                }
                else {
                    ShowWindow(hWnd, SW_SHOW);
                }
                InvalidateRect(hWnd, NULL, TRUE); // Оновлюємо вікно
                break;
            }
            case ONSelectFileMenu: {
                HMENU hMenu = GetSubMenu(RootMenu, 0);
                if (hMenu) {
                    POINT pt;
                    GetCursorPos(&pt);
                    SetForegroundWindow(hWnd);
                    TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
                }
                break;
            }
            case ONSelectInputMenu: {
                HMENU hMenu = GetSubMenu(RootMenu, 1);
                if (hMenu) {
                    POINT pt;
                    GetCursorPos(&pt);
                    SetForegroundWindow(hWnd);
                    TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
                }
                break;
            }
            case ONSelectCommandsMenu: {
                HMENU hMenu = GetSubMenu(RootMenu, 2);
                if (hMenu) {
                    POINT pt;
                    GetCursorPos(&pt);
                    SetForegroundWindow(hWnd);
                    TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
                }
                break;
            }

            case 203:
                MessageBoxW(hWnd, L"Data it`s not work!", L"Noti.p", MB_OK | MB_ICONERROR);

            case ONSelectAll:
                SendMessageW(MEdit, EM_SETSEL, 0, -1);
                break;
            case ONCopy:
                SendMessageW(MEdit, WM_COPY, 0, 0);
                break;
            case ONCut:
                SendMessageW(MEdit, WM_CUT, 0, 0);
                break;
            case ONPaste:
                SendMessageW(MEdit, WM_PASTE, 0, 0);
                break;
            case ONUndo:
                SendMessageW(MEdit, EM_UNDO, 0, 0);
                break;
            case ONRedo:
                SendMessageW(MEdit, EM_REDO, 0, 0);
                break;
            case ONAbout:
                MessageBoxW(hWnd, L"Noti.p - це текстовий редактор, створений для зручності користувачів.\n\nРозробник: Your Name", L"Про програму", MB_OK | MB_ICONINFORMATION);
                break;
            }


        }
        break;
    case WM_MOUSEMOVE: {
        POINT pt;
        RECT rc;
        GetCursorPos(&pt);
        ScreenToClient(hWnd, &pt);
        GetClientRect(hWnd, &rc);
        if (pt.x < 5 || pt.y < 30 || pt.x > rc.right - 5 || pt.y > rc.bottom - 30) {
            //MessageBoxW(hWnd, L"Ви вийшли за межі вікна!", L"Попередження", MB_OK | MB_ICONWARNING);
        }
        if (pt.x < rc.right - 5
            ||
            pt.y < rc.bottom
            ||
            pt.x > rc.right - 5
            ||
            pt.y > rc.bottom
            ) {
        }
        break;
    }

    case WM_CTLCOLOREDIT:
        if ((HWND)lp == MEdit && hEditBrush) {
            HDC hdc = (HDC)wp;
            SetTextColor(hdc, RGB(2, 9, 5));
            SetBkColor(hdc, EditBackColor);
            return (INT_PTR)hEditBrush;
        }
        return DefWindowProc(hWnd, msg, wp, lp); // Додано повернення за замовчуванням

    case WM_CONTEXTMENU:
    {
        HWND hEdit = GetDlgItem(hWnd, 200);
        if ((HWND)wp == hEdit)
        {
            // ігноруємо стандартне меню
            return 0;
        }
    }
    break;

    case WM_MEASUREITEM: {
        LPMEASUREITEMSTRUCT mis = (LPMEASUREITEMSTRUCT)lp;
        if (mis->CtlType == ODT_MENU) {
            mis->itemWidth = 150;
            mis->itemHeight = 28;
            return TRUE;
        }
        break;
    }
    case WM_DRAWITEM: {
        DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lp;
        if (dis->CtlType == ODT_MENU) {
            MenuItemData* data = (MenuItemData*)dis->itemData;
            if (!data) {
                MessageBoxW(hWnd, L"Data it`s not work!", L"Noti.p", MB_OK | MB_ICONERROR);
                break;
            }

            COLORREF bgMenuColor, textMenuColor;

            if (dis->itemState & ODS_SELECTED) {
                if (std::wstring(data->text).find(L" ️") != std::wstring::npos) {
                    bgMenuColor = RGB(113, 67, 250);
                    textMenuColor = RGB(255, 255, 255);
                }
                else {
                    bgMenuColor = RGB(50, 50, 50);
                    textMenuColor = RGB(255, 255, 255);
                }
 
            }
            else {
                bgMenuColor = RGB(230, 230, 230);
                textMenuColor = RGB(50, 50, 50);
            }

            SetBkMode(dis->hDC, OPAQUE);
            SetBkColor(dis->hDC, bgMenuColor);
            SetTextColor(dis->hDC, textMenuColor);

            HBRUSH bgBrush = CreateSolidBrush(bgMenuColor);
            FillRect(dis->hDC, &dis->rcItem, bgBrush);
            DeleteObject(bgBrush);

            DrawTextW(dis->hDC, data->text, -1, &dis->rcItem, DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);
        }

        if (dis->CtlID == ON_DESTROY_WINDOW_BTN) {
            // Отримаємо прямокутник кнопки (вже в клієнтських координатах!)
            RECT rc = dis->rcItem;

            // Вибір кольору фону
            HBRUSH hBrush = CreateSolidBrush((dis->itemState & ODS_SELECTED)
                ? RGB(150, 0, 0)
                : RGB(255, 0, 0));
            // Заливка фону
            FillRect(dis->hDC, &rc, hBrush);
            DeleteObject(hBrush);

            // Малюємо закруглений прямокутник поверх (можна просто замінити FillRect)
            //RoundRect(dis->hDC, rc.left, rc.top, rc.right, rc.bottom, 10, 10);

            // Налаштування тексту
            SetBkMode(dis->hDC, TRANSPARENT);
            SetTextColor(dis->hDC, RGB(255, 255, 255));

            // Малюємо X по центру кнопки
            DrawText(dis->hDC, L"X", -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            return TRUE;
        }


        if (dis->CtlID == ON_OVER_WINDOW_BTN) {

            if (dis->itemState & ODS_SELECTED) {
                hBrushWindowYellow = CreateSolidBrush(RGB(150, 50, 0)); // Темно-червоний, якщо натиснута
            }
            else {
                hBrushWindowYellow = CreateSolidBrush(RGB(250, 165, 5)); // Червона, якщо звичайна
            }

            FillRect(dis->hDC, &dis->rcItem, hBrushWindowYellow);
            DeleteObject(hBrushWindowYellow);

            SetBkMode(dis->hDC, TRANSPARENT);
            SetTextColor(dis->hDC, RGB(255, 255, 255));

            DrawText(dis->hDC, L"☼", -1, &dis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            return TRUE;
        }
        if (dis->CtlID == ON_HIDE_WINDOW_BTN) {

            if (dis->itemState & ODS_SELECTED) {
                hBrushHideBluePGr = CreateSolidBrush(RGB(0, 150, 150)); // Темно-червоний, якщо натиснута
            }
            else {
                hBrushHideBluePGr = CreateSolidBrush(RGB(0, 255, 255)); // Червона, якщо звичайна
            }

            FillRect(dis->hDC, &dis->rcItem, hBrushHideBluePGr);
            DeleteObject(hBrushHideBluePGr);

            SetBkMode(dis->hDC, TRANSPARENT);
            SetTextColor(dis->hDC, RGB(255, 255, 255));

            DrawText(dis->hDC, L"―", -1, &dis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            return TRUE;
        }

        if (dis->CtlID == ONSelectFileMenu) {

            if (dis->itemState & ODS_SELECTED) {
                hBrushTextMenuWhite = CreateSolidBrush(RGB(50, 50, 50)); // Червона, якщо звичайна
                SetTextColor(dis->hDC, RGB(230, 230, 230));
            }
            else {
                hBrushTextMenuWhite = CreateSolidBrush(RGB(230, 230, 230)); // Темно-червоний, якщо натиснута
                SetTextColor(dis->hDC, RGB(50, 50, 50));
            }

            FillRect(dis->hDC, &dis->rcItem, hBrushTextMenuWhite);
            DeleteObject(hBrushTextMenuWhite);

            SetBkMode(dis->hDC, TRANSPARENT);

            DrawText(dis->hDC, GetWindowTextW(MenuFile_Btn).c_str(), -1, &dis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            return TRUE;
        }
        if (dis->CtlID == ONSelectInputMenu) {

            if (dis->itemState & ODS_SELECTED) {
                hBrushTextMenuWhite = CreateSolidBrush(RGB(50, 50, 50)); // Червона, якщо звичайна
                SetTextColor(dis->hDC, RGB(230, 230, 230));
            }
            else {
                hBrushTextMenuWhite = CreateSolidBrush(RGB(230, 230, 230)); // Темно-червоний, якщо натиснута
                SetTextColor(dis->hDC, RGB(50, 50, 50));
            }

            FillRect(dis->hDC, &dis->rcItem, hBrushTextMenuWhite);
            DeleteObject(hBrushTextMenuWhite);

            SetBkMode(dis->hDC, TRANSPARENT);

            DrawText(dis->hDC, GetWindowTextW(MenuInputCh_Btn).c_str(), -1, &dis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            return TRUE;
        }
        if (dis->CtlID == ONSelectCommandsMenu) {

            if (dis->itemState & ODS_SELECTED) {
                hBrushTextMenuWhite = CreateSolidBrush(RGB(50, 50, 50)); // Червона, якщо звичайна
                SetTextColor(dis->hDC, RGB(230, 230, 230));
            }
            else {
                hBrushTextMenuWhite = CreateSolidBrush(RGB(230, 230, 230)); // Темно-червоний, якщо натиснута
                SetTextColor(dis->hDC, RGB(50, 50, 50));
            }

            FillRect(dis->hDC, &dis->rcItem, hBrushTextMenuWhite);
            DeleteObject(hBrushTextMenuWhite);

            SetBkMode(dis->hDC, TRANSPARENT);

            DrawText(dis->hDC, GetWindowTextW(MenuCommands_Btn).c_str(), -1, &dis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            return TRUE;
        }

        break;
    }

    case WM_ERASEBKGND:
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT rect;
        GetClientRect(hWnd, &rect);
        HBRUSH hBrush = CreateSolidBrush(RGB(245, 245, 255)); // колір фону
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_HOTKEY:
        switch (LOWORD(wp))
        {
        case 1: // F1 - Показати/приховати текстові параметри
            MessageBeep(MB_OK);
            if (isVisibleDV) {
                isVisibleDV = false;
                InvalidateRect(hWnd, NULL, TRUE); // Оновлюємо вікно
                ShowWindow(VievParamTXT, SW_HIDE);
            }
            else {
                isVisibleDV = true;
                InvalidateRect(hWnd, NULL, TRUE); // Оновлюємо вікно
                ShowWindow(VievParamTXT, SW_SHOW);
            }

            InvalidateRect(hWnd, NULL, TRUE); // Оновлюємо вікно
            break;
        case 2: // F2 - Зберегти файл як
            if (GetSaveFileNameW(&ofn) && filename) { // Додано перевірку filename
                SaveFile(filename);
            }
            break;
        case 3: // F3 - Відкрити файл
            if (GetOpenFileNameW(&ofn)) {
                OpenFile(filename);
            }
            break;
        case 4: // F4 - Exit application
            PostMessageW(hWnd, WM_CLOSE, wp, lp);
            break;
        case 5:
            Dovix(ProjectPath);
            break;
        case 6: // Фулл екран
        {
            if (isFullScreen) {
                // Отримуємо розміри екрана
                int width = GetSystemMetrics(SM_CXSCREEN);
                int height = GetSystemMetrics(SM_CYSCREEN);

                // Встановлюємо розміри вікна на весь екран
                SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
                SetWindowPos(hWnd, NULL, 0, 0, width, height, MF_POPUP);
                isFullScreen = false; // Змінюємо стан на повноекранний
            }
            else {
                // Повертаємося до попередніх розмірів вікна
                SetWindowPos(hWnd, HWND_TOP, 100, 100, 500, 300, SWP_SHOWWINDOW);
                SetWindowPos(hWnd, NULL, 100, 100, 500, 300, MF_POPUP);
                isFullScreen = true; // Змінюємо стан на звичайний
            }
        }
        break;
        case 10:
            SendMessageW(MEdit, EM_SETSEL, 0, -1);
            break;
        case 11:
            SetWindowText(MEdit, L"");
            break;
        default:
            break;
        }
        break;

    case WM_SIZE:
    {
        RECT rcWindow;
        GetWindowRect(hWnd, &rcWindow); // Отримуємо координати вікна (екранні)

        // Перетворюємо координати на клієнтські (якщо потрібно)
        POINT pt = { rcWindow.right - 30, rcWindow.top };
        ScreenToClient(hWnd, &pt);

        SetWindowPos(
            MenuFile_Btn,
            NULL,
            0,  // X позиція відносно клієнтської області
            0,  // Y позиція
            70,    // Ширина
            25,    // Висота
            SWP_NOZORDER
        );

        SetWindowPos(
            MenuInputCh_Btn,
            NULL,
            90,  // X позиція відносно клієнтської області
            0,  // Y позиція
            70,    // Ширина
            25,    // Висота
            SWP_NOZORDER
        );

        SetWindowPos(
            MenuCommands_Btn,
            NULL,
            180,  // X позиція відносно клієнтської області
            0,  // Y позиція
            70,    // Ширина
            25,    // Висота
            SWP_NOZORDER
        );

        SetWindowPos(
            Destroy_Window_Btn,
            NULL,
            pt.x,  // X позиція відносно клієнтської області
            pt.y,  // Y позиція
            30,    // Ширина
            30,    // Висота
            SWP_NOZORDER
        );

        SetWindowPos(
            Overoopendstr_Window_Btn,
            NULL,
            pt.x - 40, 0, 30, 25,
            SWP_NOZORDER
        );

        SetWindowPos(
            Hide_Window_Btn,
            NULL,
            pt.x - 80, 0, 30, 25,    // Висота
            SWP_NOZORDER
        );

        if (MEdit && IsWindow(MEdit)) {
            // Займає всю клієнтську область
            SetWindowPos(MEdit, NULL,
                5, 30,
                rcWindow.right, rcWindow.bottom - 10,
                SWP_NOZORDER | SWP_SHOWWINDOW);
        }

        DeleteObject(hBorderWndPen);
        break;
    }
    case WM_NCCALCSIZE: {
        if (wp) {
            NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)lp;
            params->rgrc[0].left += 5;   // Відступ для рамки
            params->rgrc[0].top += 5;
            params->rgrc[0].right -= 5;
            params->rgrc[0].bottom -= 5;
            return 0;
        }
    } break;
    case WM_LBUTTONDOWN: {
        // Перевіряємо, чи клік був у області рамки
        POINT pt = { LOWORD(lp), HIWORD(lp) };
        RECT rc;
        GetClientRect(hWnd, &rc);

        if (pt.x < 5 || pt.y < 30 || pt.x > rc.right - 5 || pt.y > rc.bottom - 30) {
            ReleaseCapture();
            SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        }
    } break;
    case WM_CREATE: {
        ShowWindow(hWnd, SW_SHOW);  // Без цього вікно не буде видимим
        // Set text color (works for edit controls in newer Windows versions)

        RootMenu = CreateMenu();
        SubMenuF = CreateMenu();
        SubMenuM = CreateMenu();
        SubMenuС = CreateMenu();
        SubCriptMenu = CreateMenu();
        SubInputMenu = CreateMenu();
        SubENCriptMenu = CreateMenu();
        SubDECriptMenu = CreateMenu();




        // hBrushCrossRed = CreateSolidBrush(RGB(255, 0, 0));
        UpdateWindow(hWnd);         // Оновлює вікно (відправляє WM_PAINT)
        SetTimer(hWnd, 1, 100, NULL);

        hEditBrush = CreateSolidBrush(EditBackColor);
        //ViewParamBkcl = CreateSolidBrush(EditBackColor);

        LoadLibraryW(L"riched20.dll");
        LoadLibraryW(L"Msftedit.dll");

        MLp = lp;

        Translate();

        MainWndAddMenu(hWnd);
        MainAddCreateWitgets(hWnd);

        SendMessageW(MEdit, WM_SETFONT, (WPARAM)MEditFont, TRUE);

        SendMessageW(Destroy_Window_Btn, WM_SETFONT, (WPARAM)KorFont, TRUE);
        SendMessageW(Overoopendstr_Window_Btn, WM_SETFONT, (WPARAM)KorFont, TRUE);
        SendMessageW(Hide_Window_Btn, WM_SETFONT, (WPARAM)KorFont, TRUE);
        SendMessageW(VievParamTXT, WM_SETFONT, (WPARAM)SpeedViewFont, TRUE);
        SendMessageW(MenuFile_Btn, WM_SETFONT, (WPARAM)SegouUIFont, TRUE);
        SendMessageW(MenuInputCh_Btn, WM_SETFONT, (WPARAM)SegouUIFont, TRUE);
        SendMessageW(MenuCommands_Btn, WM_SETFONT, (WPARAM)SegouUIFont, TRUE);

        SendMessage(Destroy_Window_Btn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapIconCross);

        SendMessageW(Hide_Window_Btn, WM_SETFONT, (WPARAM)KorFont, TRUE);
        SendMessageW(MEdit, EM_SETEVENTMASK, 0, ENM_CHANGE);
        // SendMessageW(VievParamTXT, EM_SETBKCOLOR, 0, RGB(0, 0, 0));
        //SendMessageW(VievParamTXT, WM_SETFONT, (WPARAM)NPSFont, TRUE);
        /*while (true) {
            UpdateWindow(hWnd);
        }*/


        SetOpenFileParams(hWnd);

        break;
    }

                  /*case WM_CTLCOLORBTN: {
                      HDC hdc = (HDC)wp;
                      HWND hCtrl = (HWND)lp;
                      DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lp;
              /*
                      if (hCtrl == Destroy_Window_Btn) {

                      }*
                      if (hCtrl == Destroy_Window_Btn && dis && (dis->itemState & ODS_SELECTED)) {
                          // Якщо кнопка натиснута, змінюємо колір фону
                          SetBkColor(hdc, RGB(255, 0, 0)); // Червоний колір для натиснутої кнопки
                      }
                      else {
                          return (INT_PTR)hBrushCrossRed;            // Фон кнопки
                      }*
                      if (hCtrl == Overoopendstr_Window_Btn) {
                          return (INT_PTR)hBrushWindowYellow;            // Фон кнопки
                      }
                      if (hCtrl == Hide_Window_Btn) {
                          return (INT_PTR)hBrushHideBluePGr;            // Фон кнопки
                      }
                      break;
                  }*/





    case WM_DESTROY:
    {

        PlayMelody(1); //ByeByeMelody

        if (RootMenu)
        {
            MENUITEMINFOW miInfo = { sizeof(MENUITEMINFOW) };
            miInfo.fMask = MIIM_DATA;
            if (GetMenuItemInfoW(SubInputMenu, ONClear, FALSE, &miInfo) && miInfo.dwItemData)
            {
                delete (MenuItemData*)miInfo.dwItemData;
            }
        }
        DeleteObject(hBrushCrossRed);

        PostQuitMessage(0);
        break;
    }

    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    return 0;
}


void MainAddCreateWitgets(HWND hWnd)
{
    GetClientRect(hWnd, &clientRect);

    RECT Rect = { 0, 0, 200, 100 }; // Initialize with appropriate coordinates
    HDC hdc = GetDC(hWnd);        // Get device context

    DrawCaption(hWnd, hdc, &Rect, DC_ACTIVE | DC_TEXT | DC_GRADIENT);

    ReleaseDC(hWnd, hdc);         // Release the device context

    MEdit = CreateWindowExW(
        WS_EX_CLIENTEDGE,           // Розширений стиль - бордюр
        L"EDIT",                    // Клас контролу - стандартне поле введення
        L"",                        // Початковий текст
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
        0, 30,                       // Позиція (x,y)
        100, 100,                   // Ширина і висота
        hWnd,                       // Дескриптор батьківського вікна
        (HMENU)IDC_MEDIT,           // Ідентифікатор контролу
        ((LPCREATESTRUCT)MLp)->hInstance,  // Дескриптор екземпляра програми
        NULL
    );
    //SetWindowSubclass(MEdit, 200, 0, 0); // Підкласування для обробки змін в MEdit
    SendMessageW(MEdit, EM_LIMITTEXT, 0, TRUE); // Усуваемо ліміт :) 0 = без обмежень


    Destroy_Window_Btn = CreateWindowW(
        L"button", L"X", WS_CHILD | WS_VISIBLE /*| BS_DEFPUSHBUTTON*/ | BS_OWNERDRAW | BS_FLAT,
        clientRect.right - 30, 0, 30, 30,
        hWnd, (HMENU)ON_DESTROY_WINDOW_BTN,
        ((LPCREATESTRUCT)MLp)->hInstance, NULL);

    Overoopendstr_Window_Btn = CreateWindowW(
        L"button", L"⌂", WS_CHILD | WS_VISIBLE | BS_FLAT | BS_OWNERDRAW,
        clientRect.right - 70, 0, 30, 25,
        hWnd, (HMENU)ON_OVER_WINDOW_BTN,
        ((LPCREATESTRUCT)MLp)->hInstance, NULL);

    Hide_Window_Btn = CreateWindowW(
        L"button", L"−", WS_CHILD | WS_VISIBLE | BS_FLAT | BS_OWNERDRAW,
        clientRect.right - 110, 0, 30, 25,
        hWnd, (HMENU)ON_HIDE_WINDOW_BTN,
        ((LPCREATESTRUCT)MLp)->hInstance, NULL);
    //wstring FW = file_word.c_str();

    //MenuFile_Btn = CreateWindowW(
    //    L"button", L"File", WS_CHILD | WS_VISIBLE | BS_FLAT | BS_OWNERDRAW,
    //    /*clientRect.left + 50*/ 50, 0, 100, 30,
    //    hWnd, NULL,
    //    ((LPCREATESTRUCT)MLp)->hInstance, NULL);



    /*Title_Window = CreateWindowW(
        L"static", L"Noti.p @User |Path", WS_CHILD | WS_VISIBLE | BS_FLAT | BS_OWNERDRAW,
        clientRect.left + 50, 0, 1000, 30,
        hWnd, NULL,
        ((LPCREATESTRUCT)MLp)->hInstance, NULL);*/

    int labelWidth = clientRect.right;
    int labelHeight = 10;

    int x = clientRect.right - labelWidth;
    int y = clientRect.bottom - labelHeight;

    VievParamTXT = CreateWindowW(
        L"static", L"*-* Loading *-*", WS_CHILD,  // SS_CENTER замість ES_CENTER — бо це STATIC, не EDIT
        x, y, labelWidth, labelHeight,
        hWnd, (HMENU)203,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
    ShowWindow(VievParamTXT, SW_HIDE);  // приховати
    ReturnProblems(hWnd);
}

void MainWndAddMenu(HWND hWnd)
{
    
    /*FILES*/
    MenuItemData* saveData = new MenuItemData();
    wcscpy_s(saveData->text, save_as_word.c_str()); // Чому у мене не було тексту
    AppendMenuW(SubMenuF, MF_STRING | MF_OWNERDRAW, ONSaveFile, (LPCWSTR)saveData);

    MenuItemData* openData = new MenuItemData();
    wcscpy_s(openData->text, open_word.c_str());
    AppendMenuW(SubMenuF, MF_STRING | MF_OWNERDRAW, ONOpenFile, (LPCWSTR)openData);

    MenuItemData* savefppData = new MenuItemData();
    wcscpy_s(savefppData->text, save_word.c_str());
    AppendMenuW(SubMenuF, MF_STRING | MF_OWNERDRAW, ONSaveFPPFile, (LPCWSTR)savefppData);
    /*INPUT CHANGE!!!*/
    MenuItemData* clearData = new MenuItemData();
    wcscpy_s(clearData->text, clear_word.c_str());
    AppendMenuW(SubInputMenu, MF_STRING | MF_OWNERDRAW, ONClear, (LPCWSTR)clearData);

    // Підменю "Шифрування"
    HMENU SubCryptMenu = CreatePopupMenu();
    MenuItemData* cryptData = new MenuItemData();
    wcscpy_s(cryptData->text, to_cipher_word.c_str());
    AppendMenuW(SubInputMenu, MF_POPUP | MF_OWNERDRAW, (UINT_PTR)SubCryptMenu, (LPCWSTR)cryptData);

    // Підменю "Розшифрувати"
    HMENU SubDecryptMenu = CreatePopupMenu();
    MenuItemData* decryptData = new MenuItemData();
    wcscpy_s(decryptData->text, to_decipher_word.c_str());
    AppendMenuW(SubCryptMenu, MF_POPUP | MF_OWNERDRAW, (UINT_PTR)SubDecryptMenu, (LPCWSTR)decryptData);
    // Пункт у "Розшифрувати"
    MenuItemData* steplerDecryptData = new MenuItemData();
    wcscpy_s(steplerDecryptData->text, L"Stepler ️decrypter");
    AppendMenuW(SubDecryptMenu, MF_STRING | MF_OWNERDRAW, ONSteplerDecrypter, (LPCWSTR)steplerDecryptData);
    MenuItemData* Method1213Decrypt = new MenuItemData();

    wcscpy_s(Method1213Decrypt->text, method_word.c_str());
    wcscat_s(Method1213Decrypt->text, L" 1213");
    AppendMenuW(SubDecryptMenu, MF_STRING | MF_OWNERDRAW, ON1213MethodDecrypter, (LPCWSTR)Method1213Decrypt);

    MenuItemData* Method56212Decrypt = new MenuItemData();
    wcscpy_s(Method56212Decrypt->text, method_word.c_str());
    wcscat_s(Method56212Decrypt->text, L" 56217");
    AppendMenuW(SubDecryptMenu, MF_STRING | MF_OWNERDRAW, ON562312MethodDecrypter, (LPCWSTR)Method56212Decrypt);
    
    // Підменю "Зашифрувати"
    HMENU SubEncryptMenu = CreatePopupMenu();
    MenuItemData* encryptData = new MenuItemData();
    wcscpy_s(encryptData->text, to_encrypt_word.c_str());
    AppendMenuW(SubCryptMenu, MF_POPUP | MF_OWNERDRAW, (UINT_PTR)SubEncryptMenu, (LPCWSTR)encryptData);

    // Пункт у "Зашифрувати"
    MenuItemData* steplerData = new MenuItemData();
    wcscpy_s(steplerData->text, L"Stepler ️encrypter");
    AppendMenuW(SubEncryptMenu, MF_STRING | MF_OWNERDRAW, ONSteplerEncrypter, (LPCWSTR)steplerData);
    
    MenuItemData* Method1213 = new MenuItemData();
    wcscpy_s(Method1213->text, method_word.c_str());
    wcscat_s(Method1213->text, L" 1213");
    AppendMenuW(SubEncryptMenu, MF_STRING | MF_OWNERDRAW, ON1213MethodEncrypter, (LPCWSTR)Method1213);

    MenuItemData* Method56212 = new MenuItemData();
    wcscpy_s(Method56212->text, method_word.c_str());
    wcscat_s(Method56212->text, L" 56217");
    AppendMenuW(SubEncryptMenu, MF_STRING | MF_OWNERDRAW, ON562312MethodEncrypter, (LPCWSTR)Method56212);

    /*COMANDS!!!!!*/
    MenuItemData* upData = new MenuItemData();
    wcscpy_s(upData->text, redo_word.c_str());
    AppendMenuW(SubMenuС, MF_STRING | MF_OWNERDRAW, ONRedo, (LPCWSTR)upData);
    MenuItemData* undoData = new MenuItemData();
    wcscpy_s(undoData->text, undo_word.c_str());
    AppendMenuW(SubMenuС, MF_STRING | MF_OWNERDRAW, ONUndo, (LPCWSTR)undoData);
    MenuItemData* copyData = new MenuItemData();
    wcscpy_s(copyData->text, copy_word.c_str());
    AppendMenuW(SubMenuС, MF_STRING | MF_OWNERDRAW, ONCopy, (LPCWSTR)copyData);
    MenuItemData* cutData = new MenuItemData();
    wcscpy_s(cutData->text, cut_word.c_str());
    AppendMenuW(SubMenuС, MF_STRING | MF_OWNERDRAW, ONCut, (LPCWSTR)cutData);
    MenuItemData* pasteData = new MenuItemData();
    wcscpy_s(pasteData->text, paste_word.c_str());
    AppendMenuW(SubMenuС, MF_STRING | MF_OWNERDRAW, ONPaste, (LPCWSTR)pasteData);
    MenuItemData* selectAllData = new MenuItemData();
    wcscpy_s(selectAllData->text, select_all_word.c_str());
    AppendMenuW(SubMenuС, MF_STRING | MF_OWNERDRAW, ONSelectAll, (LPCWSTR)selectAllData);
    /*MORE DESK*/
    AppendMenuW(SubMenuM, MF_STRING | MF_OWNERDRAW, ONMoreMenu, L"Нижній посібник тексту");
    AppendMenuW(SubMenuM, MF_STRING | MF_OWNERDRAW, ONSpawnParametress, parametres_word.c_str());
    AppendMenuW(SubMenuM, MF_POPUP | MF_OWNERDRAW, (UINT_PTR)SubInputMenu, L"Текстовий браузер");
    // AppendMenuW(SubMenuF, MF_STRING, ONOpenFile, L"Відкрити");
     /*POPAPES*/
    MenuItemData* fileMenuData = new MenuItemData();
    wcscpy_s(fileMenuData->text, FILE_word.c_str());
    wcscat_s(fileMenuData->text, L" ①");
    AppendMenuW(RootMenu, MF_POPUP | MF_OWNERDRAW | WS_BORDER, (UINT_PTR)SubMenuF, (LPCWSTR)fileMenuData);

    MenuItemData* moreMenuData = new MenuItemData();
    wcscpy_s(moreMenuData->text, more_word.c_str());
    wcscat_s(moreMenuData->text, L" ②");
    AppendMenuW(RootMenu, MF_POPUP | MF_OWNERDRAW | WS_BORDER, (UINT_PTR)SubMenuM, (LPCWSTR)moreMenuData);

    MenuItemData* forvMenuData = new MenuItemData();
    wcscpy_s(forvMenuData->text, forward_word.c_str());
    wcscat_s(forvMenuData->text, L" ③");
    AppendMenuW(RootMenu, MF_POPUP | MF_OWNERDRAW | WS_BORDER, (UINT_PTR)SubMenuС, (LPCWSTR)forvMenuData);

    SetMenu(hWnd, RootMenu);




    MenuFile_Btn = CreateWindowW(
        L"button", L"FILE", WS_CHILD | WS_VISIBLE | BS_FLAT | BS_OWNERDRAW,
        0, 0, 100, 30,
        hWnd, (HMENU)ONSelectFileMenu,
        ((LPCREATESTRUCT)MLp)->hInstance, NULL);
    


    MenuInputCh_Btn = CreateWindowW(
        L"button", L"INPUT", WS_CHILD | WS_VISIBLE | BS_FLAT | BS_OWNERDRAW,
        clientRect.left + 50, 0, 100, 30,
        hWnd, (HMENU)ONSelectInputMenu,
        ((LPCREATESTRUCT)MLp)->hInstance, NULL);


    MenuCommands_Btn = CreateWindowW(
        L"button", L"COMMANDS", WS_CHILD | WS_VISIBLE | BS_FLAT | BS_OWNERDRAW,
        clientRect.left + 150, 0, 100, 30,
        hWnd, (HMENU)ONSelectCommandsMenu,
        ((LPCREATESTRUCT)MLp)->hInstance, NULL);

}
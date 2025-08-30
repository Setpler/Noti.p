#include "Functions.h"
#include <Windows.h>
#include <commdlg.h>
#include <algorithm>
#include <string>
#include <cmath>
#include <cwchar>
#include <ctime>
#include <chrono>
#include <iomanip> // For std::put_time
#include <sstream>
#include <time.h>
#include <random>



#pragma comment(lib, "winmm.lib")

using std::wstring;


// Functions.cpp
HWND MEdit, VievParamTXT;
RECT clientRect;
OPENFILENAMEW ofn;
wchar_t filename[MAX_PATH];
HBRUSH hEditBrush, ViewParamBkcl;
HFONT MEditFont, NPSFont, KorFont, SpeedViewFont, SegouUIFont;
bool isVisibleDV;
HPEN hBorderWndPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0)); // Товщина 3 пікселі, червоний
HWND Destroy_Window_Btn, Overoopendstr_Window_Btn, Hide_Window_Btn, Title_Window, MenuFile_Btn, MenuInputCh_Btn, MenuCommands_Btn, FrSaveFile_BtmMenu;
HBITMAP hBitmapIconCross;
HBRUSH hBrushTextMenuWhite = CreateSolidBrush(RGB(240, 240, 240)), hBrushCrossRed = CreateSolidBrush(RGB(255, 0, 0)), hBrushWindowYellow = CreateSolidBrush(RGB(255, 255, 0)), hBrushHideBluePGr = CreateSolidBrush(RGB(0, 255, 255)); // Червоний колір для кнопки закриття
bool isFullScreen;
bool hoverOverBtn;

int screenWidth;
int screenHeight;

// Single definition for all variables







wstring error_word;
wstring cannot_open_file_word;
wstring cannot_save_file_word;
wstring file_is_big_or_error_size_word;
wstring error_convert_text_word;
wstring error_regist_class_error_word;
wstring count_symbols_txt_pole_word;
wstring select_all_word;
wstring copy_word;
wstring cut_word;
wstring redo_word;
wstring undo_word;
wstring paste_word;
wstring save_as_word;
wstring save_word;
wstring open_word;
wstring all_files_word;
wstring txt_files_word;
wstring more_word;
wstring to_cipher_word;
wstring parametres_word;
wstring forward_word;
wstring clear_word;
wstring FILE_word;
wstring to_encrypt_word;
wstring to_decipher_word;
wstring method_word;

HMENU RootMenu, SubMenuF, SubMenuM, SubMenuС, SubInputMenu, SubCriptMenu, SubENCriptMenu, SubDECriptMenu;
POINT CurPoint;
LPARAM MLp;
const int TXTBufferSize = 8589934;
wchar_t Buffer[TXTBufferSize];
LPWSTR ProjectPath;



// Smooth beep function
void SmoothBeep(int freq, int duration, double volumeLevel) {
    const int sampleRate = 44100;
    const int maxAmplitude = 30000;
    const double twoPi = 6.283185307179586;

    volumeLevel = max(0.0, min(1.0, volumeLevel));
    int amplitude = static_cast<int>(maxAmplitude * volumeLevel);

    int bufferSize = duration * sampleRate / 1000;
    short* buffer = new short[bufferSize];

    for (int i = 0; i < bufferSize; i++) {
        double envelope = 1.0;
        if (i < 1000)
            envelope = i / 1000.0;
        else if (i > bufferSize - 1000)
            envelope = (bufferSize - i) / 1000.0;

        buffer[i] = static_cast<short>(amplitude * envelope * sin(twoPi * freq * i / sampleRate));
    }

    HWAVEOUT hWaveOut;
    WAVEFORMATEX wfx = {
        WAVE_FORMAT_PCM,
        1,
        sampleRate,
        sampleRate * sizeof(short),
        sizeof(short),
        16,
        0
    };

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    WAVEHDR header = {
        (LPSTR)buffer,
        bufferSize * sizeof(short),
        0, 0, 0, 0, 0, 0
    };

    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(duration + 1);

    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

// Other function implementations...
VOID ReturnProblems(HWND hwnd) {
    if (!MEdit) {
        MessageBoxW(hwnd, L"Помилка створення MEdit!", L"Помилка", MB_ICONERROR);
    }
    if (!LoadLibraryW(L"riched20.dll")) {
        MessageBoxW(hwnd, L"Не вдалося завантажити riched20.dll", L"Помилка", MB_ICONERROR);
    }
}

VOID LoadWCommands(HWND hwnd, WPARAM wps, LPARAM lps) {
    if (HIWORD(wps) == EN_CHANGE && (HWND)wps == MEdit) {
        MessageBoxW(hwnd, L"Текст змінено!", L"Повідомлення", MB_OK);
        SmoothBeep(400, 100, 1.0);
    }
}


void PlayMP3(const wchar_t* filename) {
    mciSendStringW((L"open \"" + wstring(filename) + L"\" type mpegvideo alias mp3").c_str(), NULL, 0, NULL);
    mciSendStringW(L"play mp3", NULL, 0, NULL);
}

void PlaySteplerMusic(wchar_t* filename, int version) {
    // Implementation here
}

void FullScreen(HWND hWnd, bool isFullScreen1) {
    if (isFullScreen1) {
        // Отримуємо розміри екрана
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        // Встановлюємо розміри вікна на весь екран
        SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
        SetWindowPos(hWnd, NULL, 0, 0, width, height, MF_POPUP);
        isFullScreen1 = false; // Змінюємо стан на повноекранний
    }
    else {
        // Повертаємося до попередніх розмірів вікна
        SetWindowPos(hWnd, HWND_TOP, 100, 100, 500, 300, SWP_SHOWWINDOW);
        SetWindowPos(hWnd, NULL, 100, 100, 500, 300, MF_POPUP);
        isFullScreen1 = true; // Змінюємо стан на звичайний
    }
    isFullScreen1 = !isFullScreen1; // Змінюємо стан на протилежний
}

void CustomMenuHower(bool isWisible)
{
    if (isWisible) {
        
    }
    else {
        ShowWindow(VievParamTXT, SW_HIDE);
    }
}



void Dovix(LPWSTR Path) {
    if (!Path || Path[0] == L'\0') return;
    if (!MEdit) return;

    WCHAR Buffer[65536] = { 0 };
    GetWindowTextW(MEdit, Buffer, TXTBufferSize);

    int utf8Length = WideCharToMultiByte(CP_UTF8, 0, Buffer, -1, NULL, 0, NULL, NULL);
    if (utf8Length <= 0) return;

    char* utf8Buffer = new char[utf8Length];
    WideCharToMultiByte(CP_UTF8, 0, Buffer, -1, utf8Buffer, utf8Length, NULL, NULL);

    HANDLE hFile = CreateFileW(Path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD bytesWritten;
        WriteFile(hFile, utf8Buffer, utf8Length - 1, &bytesWritten, NULL);
        CloseHandle(hFile);
    }

    delete[] utf8Buffer;
}

VOID CriptTextFile(wstring text, int type, bool mode /* mode = 0 - Encrypt | mode = 1 - Decrypt */) {
    if (!mode) {
        switch (type)
        {

        case 1: { // Stepler Method
            bool e = MessageBoxW(NULL, L"Шифрувати текст?", L"Підтвердження", MB_YESNO | MB_ICONQUESTION) == IDYES;
            if (!e) return;

            int len = static_cast<int>(text.length());

            bool mod2 = (len % 2) != 0;
            bool mod5 = (len % 5) != 0;

            for (int i = len - 1; i >= 0; --i) {
                if (mod2) text[i] += 2;

                int r = 0; // сюди треба підставити збережене значення з шифрування
                if (i > 0) text[i - 1] += r;

                if (mod5) {
                    for (int j = std::min(199, len - 1); j >= 0; --j) {
                        if (j + 2 < len) text[j + 2] -= text[j] + 7;
                        if (j + 9 < len) text[j + 9] -= text[j] - 3;
                    }
                    text[i] += 13;
                }

                if (mod2) text[i] += 7;
            }

            SetWindowTextW(MEdit, text.c_str());
            break;
        }
        

        case 2: // +1 %2 +13
        {
            bool e = MessageBoxW(NULL, L"Шифрувати текст?", L"Підтвердження", MB_YESNO | MB_ICONQUESTION) == IDYES;
            if (!e) return;
            else {
                for (size_t i = 0; i < text.length(); ++i) {
                    text[i] = text[i] + 1; // Simple encryption by shifting characters
                    if (text.length() % 2) {
                        text[i] = text[i] + 13; // Additional shift for odd-length strings
                    }
                }
                SetWindowTextW(MEdit, text.c_str());
                break;
            }
        }
        case 3: // +3 %6 -1 %3 +2 36132
        {
            bool e = MessageBoxW(NULL, L"Шифрувати текст?", L"Підтвердження", MB_YESNO | MB_ICONQUESTION) == IDYES;
            if (!e) return;
            else {
                for (size_t i = 0; i < text.length(); ++i) {
                    text[i] = text[i] - 3; // Simple encryption by shifting characters
                    if (text.length() % 2)
                        text[i] = text[i] + 1; // Additional shift for odd-length strings

                    if (text.length() % 4)
                        text[i] = text[i] - 3;
                }
                SetWindowTextW(MEdit, text.c_str());
                break;
            }
        }
    }


    }
    else {
        switch (type)
        {
        case 1: { // Stepler Method
            bool e = MessageBoxW(NULL, L"Шифрувати текст?", L"Підтвердження", MB_YESNO | MB_ICONQUESTION) == IDYES;
            if (!e) return;
            else {
                size_t len = text.length();
                bool mod2 = (len % 2) != 0;
                bool mod5 = (len % 5) != 0;

                if (mod2) {
                    text[len / 2] = L' '; // краще пробіл, ніж NULL
                }

                for (size_t i = 0; i < len; ++i) {
                    text[i] -= 7;

                    if (mod5) {
                        text[i] -= 13;

                        // цикл обмежений довжиною рядка
                        for (size_t j = 0; j < std::min<size_t>(200, len); ++j) {
                            if (j + 9 < len)
                                text[j + 9] += text[j] - 3;
                            if (j + 2 < len)
                                text[j + 2] += text[j] + 7;

                            text[j] = L' '; // заміна NULL на пробіл
                        }
                    }

                    if (mod2) {
                        text[i] -= 2;
                        if (mod5)
                            text[i] -= 17;
                    }
                }

                SetWindowTextW(MEdit, text.c_str());
                break;
            }

        }






        case 2: { // -1 %2 -13
            for (size_t i = 0; i < text.length(); ++i) {
                text[i] = text[i] - 1; // Simple decryption by reversing the shift
                if (text.length() % 2) {
                    text[i] = text[i] - 13; // Reverse additional shift for odd-length strings
                }
            }
            SetWindowTextW(MEdit, text.c_str());
            break;
        }
        case 3: // -5 %6 +2 %3 -12 562312
        {
            bool e = MessageBoxW(NULL, L"Шифрувати текст?", L"Підтвердження", MB_YESNO | MB_ICONQUESTION) == IDYES;
            if (!e) return;
            else {
                for (size_t i = 0; i < text.length(); ++i) {
                    text[i] = text[i] + 3; // Simple encryption by shifting characters
                    if (text.length() % 2)
                        text[i] = text[i - 1]; // Additional shift for odd-length strings

                    if (text.length() % 4)
                        text[i] = text[i + 3];
                }
                SetWindowTextW(MEdit, text.c_str());
                break;
            }
        }
        default:
            break;

        }
    }
}
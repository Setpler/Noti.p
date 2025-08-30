#pragma once

#include <Windows.h>
#include <string>
#include <cwchar>

using namespace std;

#define IDC_MEDIT 200

// Menu command IDs

#define ONSelectCommandsMenu 998
#define ONSelectInputMenu 999
#define ONSelectFileMenu 1000
#define ONSaveFile 1001
#define ONOpenFile 1002
#define ONMoreMenu 1003
#define ONClear 1004
#define ONSpawnParametress 1005
#define ONShifr 1006
#define ONCompileForC 1007
#define ONCompileForCPP 1008
#define ONSaveFPPFile 1009


#define ONSteplerEncrypter 1310 // This is a duplicate, should be removed or renamed
#define ON1213MethodEncrypter 1311
#define ON562312MethodEncrypter 1312
#define ONSteplerDecrypter 1313 // This is a duplicate, should be removed or renamed
#define ON1213MethodDecrypter 1314
#define ON562312MethodDecrypter 1315

#define ONRedo 1200
#define ONUndo 1201
#define ONCopy 1202
#define ONCut 1203
#define ONPaste 1204
#define ONSelectAll 1205
#define ONAbout 1206



#define ON_DESTROY_WINDOW_BTN 2000
#define ON_OVER_WINDOW_BTN 2001
#define ON_HIDE_WINDOW_BTN 2002

// Functions.h
extern HWND MEdit, VievParamTXT;
extern RECT clientRect;
extern OPENFILENAMEW ofn;
extern wchar_t filename[MAX_PATH];
extern HBRUSH hEditBrush, ViewParamBkcl;
extern HFONT MEditFont, NPSFont, KorFont, SpeedViewFont, SegouUIFont;
extern bool isVisibleDV;
extern HMENU RootMenu, SubMenuF, SubMenuM, SubMenuС, SubInputMenu, SubCriptMenu, SubENCriptMenu, SubDECriptMenu;
extern HWND Destroy_Window_Btn, Overoopendstr_Window_Btn, Hide_Window_Btn, Title_Window, MenuFile_Btn, MenuInputCh_Btn, MenuCommands_Btn;
extern POINT CurPoint;
extern LPARAM MLp;
extern const int TXTBufferSize;
extern wchar_t Buffer[];
const wstring ProgName = L"Noti.p"; 
extern int screenWidth;
extern int screenHeight;
extern HPEN hBorderWndPen;
extern HBITMAP hBitmapIconCross;
extern HBRUSH hBrushTextMenuWhite, hBrushCrossRed, hBrushWindowYellow, hBrushHideBluePGr;
extern bool isFullScreen;
extern bool hoverOverBtn;
//extern COLOREF BorderColor;

//extern wstring error_word, cannot_open_file_word, cannot_save_file_word, file_is_big_or_error_size_word, error_convert_text_word, error_regist_class_error_word, count_symbols_txt_pole_word, select_all_word, copy_word, cut_word, up_word, down_word, paste_word;
extern std::wstring error_word;
extern std::wstring cannot_open_file_word;
extern std::wstring cannot_save_file_word;
extern std::wstring file_is_big_or_error_size_word;
extern std::wstring error_convert_text_word;
extern std::wstring error_regist_class_error_word;
extern std::wstring count_symbols_txt_pole_word;
extern std::wstring select_all_word;
extern std::wstring copy_word;
extern std::wstring cut_word;
extern std::wstring redo_word;
extern std::wstring undo_word;
extern std::wstring paste_word;
extern std::wstring file_word;
extern std::wstring save_as_word;
extern std::wstring save_word;
extern std::wstring open_word;
extern std::wstring all_files_word;
extern std::wstring txt_files_word;
extern std::wstring clear_word;
extern std::wstring to_cipher_word;
extern std::wstring parametres_word;
extern std::wstring more_word;
extern std::wstring forward_word;
extern std::wstring FILE_word;
extern std::wstring to_encrypt_word;
extern std::wstring to_decipher_word;
extern std::wstring method_word;
extern LPWSTR ProjectPath;

struct MenuItemData {
    wchar_t text[256];
};

extern const int TXTBufferSize;
extern wchar_t Buffer[];

// Window class and procedure
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

wstring GetWindowTextW(HWND hWnd);

// Functions
void MainAddCreateWitgets(HWND hWnd);
void MainWndAddMenu(HWND hWnd);
void SetOpenFileParams(HWND hwnd);
void Translate();
void SmoothBeep(int freq, int duration, double volumeLevel);
void PlayMP3(const wchar_t* filename);
void PlaySteplerMusic(wchar_t* filename, int version);
void Dovix(LPWSTR Path);
void FullScreen(HWND hWnd, bool isFullScreen);
void CustomMenuHower(bool isWisible);
void MEditEdited(HWND hWnd);
VOID ReturnProblems(HWND hwnd);
VOID LoadWCommands(HWND hwnd, WPARAM wps, LPARAM lps);
VOID CriptTextFile(wstring text = L"", int type = 0, bool mode = 0 /* mode = 0 - Encrypt | mode = 1 - Decrypt */);

// WinMain entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow);
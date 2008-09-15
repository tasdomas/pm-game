/*
 * Protmusio zaidimo valdymo programa.
 * 
 * Domas Monkus, 2008.09
 */
 
#include "pm_app.h"
#include "pm_main_frame.h"

extern HWND window;
LRESULT CALLBACK LowLevelKeyboard(int nCode, WPARAM wParam, LPARAM lParam);
 
bool PMApp::OnInit()
{
    PMMainFrame *frame = new PMMainFrame(
        _T("Protmusis"),
        wxPoint(50,50),
        wxSize(450,340),
        wxCLOSE_BOX | wxSYSTEM_MENU | wxMINIMIZE_BOX
            | wxCAPTION | wxFRAME_NO_TASKBAR
            | wxRESIZE_BORDER | wxSTAY_ON_TOP);
    frame->Show(TRUE);
    SetTopWindow(frame);

    window = (HWND)frame->GetHWND();

    HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

    hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboard, hInstance, 0);

    return TRUE;
}

int PMApp::OnExit() {
    UnhookWindowsHookEx(hook);
    wxMessageBox("Baigiamas programos darbas.", "Protmusis");
    return 1;
}

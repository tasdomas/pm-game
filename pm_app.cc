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
        wxDefaultSize,
        wxCLOSE_BOX | wxFRAME_NO_TASKBAR
            | wxRESIZE_BORDER | wxSTAY_ON_TOP);
    frame->Show(TRUE);
    SetTopWindow(frame);

    window = (HWND)frame->GetHWND();

    HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

    hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboard, hInstance, 0);
    if (hook == NULL) {
        wxMessageBox(wxT("Nepavyko perimti klaviaturos valdymo. Programa neveiks."));
    }
    return TRUE;
}

int PMApp::OnExit() {
    UnhookWindowsHookEx(hook);
    //wxMessageBox(wxT("Baigiamas programos darbas."), wxT("Protmusis"));
    return 1;
}

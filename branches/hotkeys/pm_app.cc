/*
 * Protmusio zaidimo valdymo programa.
 * 
 * Domas Monkus, 2008.09
 */
 
#include "pm_app.h"
#include "pm_main_frame.h"

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

    return TRUE;
}

int PMApp::OnExit() {
    return 1;
}

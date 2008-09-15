/*
 * Protmusio zaidimo valdymo programa.
 *
 * Domas Monkus, 2008.09
 */
 
#include "pm_main_frame.h"

PMMainFrame::PMMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxFrame((wxFrame *)NULL, -1, title, pos, size, style)
{
}

void PMMainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void PMMainFrame::OnTest(wxEvent& WXUNUSED(event))
{
    wxMessageBox("OK", "Protmusis");
}


WXLRESULT PMMainFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    WXLRESULT rc = 0;
    if (message == WM_USER + 1) {
        wxMessageBox("OK", "Protmusis");
    } else {
//    if ( !processed )
        rc = wxFrame::MSWWindowProc(message, wParam, lParam);
    }

    return rc;
}

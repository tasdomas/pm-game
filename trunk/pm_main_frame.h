#ifndef PM_GAME_MAIN_FRAME
#define PM_GAME_MAIN_FRAME

#include "wx/wx.h" 

class PMMainFrame: public wxFrame
{
public:

    PMMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style);

    void OnQuit(wxCommandEvent& event);
    void OnTest(wxEvent& event);
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

    DECLARE_EVENT_TABLE()
};

#endif

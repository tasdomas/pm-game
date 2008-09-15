#ifndef PMGAME_MAIN
#define PMGAME_MAIN

enum
{
    ID_Quit = 1,
    ID_About,
};

BEGIN_EVENT_TABLE(PMMainFrame, wxFrame)
    EVT_MENU(ID_Quit, PMMainFrame::OnQuit)
    EVT_CUSTOM(WM_USER+1, 0, PMMainFrame::OnTest)
END_EVENT_TABLE()

#endif

#ifndef PM_GAME_MAIN_FRAME
#define PM_GAME_MAIN_FRAME

#include "wx/wx.h" 

#define MAX_TEAMS 4
#define BEEP_NOTICE 500
#define TIMER_INTERVAL 10
#define BEEP_START 55 

struct ui_row {
    wxPanel * panel;
    wxStaticText * name;
    wxStaticText * points;
};

class PMMainFrame: public wxFrame
{
private:
    ui_row rows[MAX_TEAMS]; //main_frame interface elements
    int teamCount; //rodomu komandu sk.
    wxBoxSizer * topSizer;
    wxStaticText * timerDisplay;
    wxTimer * timer;
    wxStopWatch * watch;
    wxPoint dragStart;
    wxPoint startPos;    
    int dragMode;
    long msCount;
    bool beeps[5];
        
public:

    PMMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style);

    void OnQuit(wxCommandEvent& event);
    void OnTest(wxEvent& event);
    void OnMouseEvent(wxMouseEvent& event);
    void OnTimer(wxTimerEvent& event);
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
    
    void SetTeams(int count);
    void SetColour(int pos, wxColour colour);
    
    //timer control
    void TimerStart(bool reset = true);
    void TimerStop();
    void TimerPause();

    DECLARE_EVENT_TABLE()
};

#endif

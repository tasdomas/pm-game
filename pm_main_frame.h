#ifndef PM_GAME_MAIN_FRAME
#define PM_GAME_MAIN_FRAME

#include "wx/wx.h" 

#define MAX_TEAMS 4
#define BEEP_NOTICE 500
#define BEEP_GAMESTART 600
#define BEEP_END 350
#define TIMER_INTERVAL 10
#define BEEP_START 5 
#define START_TIME 60*1000

#define BEEP_RANDOM true
#define BEEP_MIN 600
#define BEEP_MAX 2000

enum
{
    ID_Quit = 1,
    ID_About,
    ID_Timer,
    ID_Beeper,
};

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_LOCAL_EVENT_TYPE( EVT_BEEPER, wxNewEventType() )
END_DECLARE_EVENT_TYPES()

/* vienos komandos irasai */
struct ui_row {
    wxPanel * panel;
    wxStaticText * nameDisplay;
    wxStaticText * pointsDisplay;
};

class PMMainFrame: public wxFrame
{
private:
    ui_row rows[MAX_TEAMS]; //main_frame interface elements
    int teamCount; //rodomu komandu sk.
    
    bool beepRandom;
    int beepStart;
    int beepEnd;
    
    int state;
    
    bool resetOnStart; 
    
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
    void CreateControls();

    void OnQuit(wxCommandEvent& event);
    void OnTest(wxEvent& event);
    void OnMouseEvent(wxMouseEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnBeeper(wxEvent & event);
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
    
    void SetTeams(int count);
    void SetColour(int pos, wxColour colour);
    
    //timer control
    void TimerStart(bool reset = true);
    void TimerStop();
    void TimerPause();
    
    void EditSettings();
    
    //game control
    void StartGame();
    void PauseGame();
    void ResetGame();
    
    void SetTeam(int pos, wxString & name);
    
    void ShowTime(long timeMs);
    
    //event handlers
    void OnKey(wxKeyEvent & event);

    DECLARE_EVENT_TABLE()
};

#endif

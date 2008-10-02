#ifndef PM_GAME_MAIN_FRAME
#define PM_GAME_MAIN_FRAME

#include "wx/wx.h" 

#include <iostream>
#include <fstream>
using namespace std;

#define MAX_TEAMS 4

/****************
 garsinis signalas
*****************/
//paskutiniu sekundziu signalu daznis 
#define BEEP_NOTICE 500
//pradinio signalo daznis
#define BEEP_GAMESTART 600
//pabaigos signalo daznis
#define BEEP_END 350
//komanda paspaude pirma
#define BEEP_CLICK 300
#define BEEP_CLICK_LEN 600
//lygiosios
#define BEEP_DRAW 400
//kas kiek laiko atnaujinamas laikmatis (ms)
#define TIMER_INTERVAL 10
//kiek sekundziu likus iki pabaigos pradedama signalizuoti
#define BEEP_START 5 
//pradinis laikas (1 min)
#define START_TIME 60*1000

#define BEEP_RANDOM true
//minimali signalo trukme
#define BEEP_MIN 600
//maksimali signalo trukme
#define BEEP_MAX 2000

//komandu busenos
#define TEAM_WAITING 0
#define TEAM_ANSWERING 1
#define TEAM_BLOCKED 2
#define TEAM_DRAW 3

enum
{
    ID_Quit = 1,
    ID_About,
    ID_Timer,
    ID_Beeper,
    ID_Team1,
    ID_Team2,
    ID_Team3,
    ID_Team4,

    ID_Team12,
    ID_Team13,
    ID_Team14,
    ID_Team23,
    ID_Team24,
    ID_Team34,

    ID_Team123,
    ID_Team134,
    ID_Team124,
    ID_Team234,

    ID_Team1234,
    
    ID_StartGame,
    ID_PauseGame,
    ID_ResetGame,
    
    ID_Points1,
    ID_Points2,
    ID_Points3,
    ID_Points4,
    ID_Points5,
    ID_Points6,
    ID_Points7,
    ID_Points8
    
};

struct key_def {
    int id;
    int keycode;
    int mod;
};

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_LOCAL_EVENT_TYPE( EVT_BEEPER, wxNewEventType() )
END_DECLARE_EVENT_TYPES()

/* vienos komandos irasai */
struct ui_row {
    wxPanel * panel;
    wxStaticText * nameDisplay;
    wxStaticText * pointsDisplay;
    int state;
    int score;
};

class PMMainFrame: public wxFrame
{
private:
    ui_row rows[MAX_TEAMS]; //main_frame interface elements
    int teamCount; //rodomu komandu sk.
    
    bool beepRandom;
    int beepStart;
    int beepEnd;

    ofstream t;

    long msCount;
    bool beeps[5];
    int beepCount;
    
    int state;
    
    bool resetOnStart; 
    
    wxBoxSizer * topSizer;
    wxStaticText * timerDisplay;
    wxTimer * timer;
    wxStopWatch * watch;
    wxPoint dragStart;
    wxPoint startPos;    
    int dragMode;
    
    wxColour defaultBackground;
        
public:

    PMMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style);
    void CreateControls();

    void OnQuit(wxCommandEvent& event);
    void OnTest(wxEvent& event);
    void OnMouseEvent(wxMouseEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnBeeper(wxEvent & event);
    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
    
    void ProcessGameKey(long team, long alt);
    void SetScore(long ch);
    void SetScore(int team, int diff);
    
    void SetTeams(int count);
    void SetColour(int pos, wxColour colour);
    bool SetTeamState(int pos, int newState);
    
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
    void UpdateTeams();
    
    //event handlers
    void OnKey(wxKeyEvent & event);
    void RegisterHotKeys();
    void UnregisterHotKeys();
    void OnHotKey(wxKeyEvent & event);    

    DECLARE_EVENT_TABLE()
};

#endif

/*
 * Protmusio zaidimo valdymo programa.
 *
 * Domas Monkus, 2008.09
 */
 
#include "pm_main_frame.h"
//#include "cust_event.h"

enum
{
    ID_Quit = 1,
    ID_About,
    ID_Timer
};

BEGIN_EVENT_TABLE(PMMainFrame, wxFrame)
    EVT_MENU(ID_Quit, PMMainFrame::OnQuit)
    EVT_CUSTOM(WM_USER+1, 0, PMMainFrame::OnTest)
    EVT_MOUSE_EVENTS(PMMainFrame::OnMouseEvent)
    EVT_TIMER(ID_Timer, PMMainFrame::OnTimer)
END_EVENT_TABLE()

PMMainFrame::PMMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxFrame((wxFrame *)NULL, -1, title, pos, size, style)
{

    dragMode = 0;
    wxPanel * topPanel = new wxPanel(this);
    topSizer = new wxBoxSizer(wxVERTICAL);
    wxFont font(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);    
    
    //laikmacio isvedimas
    timerDisplay = new wxStaticText(topPanel,
        wxID_ANY,
        wxT("00:00:00"),
        wxDefaultPosition,
        wxDefaultSize,
        wxALIGN_CENTER);
    timerDisplay->SetFont(wxFont(font));        
    topSizer->Add(timerDisplay, 1, wxFIXED_MINSIZE | wxALIGN_CENTER);        
    

    for (int i = 0; i < MAX_TEAMS; i++) {
        wxPanel * panel = new wxPanel(topPanel);

        wxBoxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
        rows[i].panel = panel;

        rows[i].name = new wxStaticText(panel, 
            wxID_ANY, 
            wxT("Komanda"),
            wxDefaultPosition,
            wxDefaultSize,
            wxALIGN_LEFT);
        rows[i].name->SetFont(wxFont(font));
        rows[i].name->SetEventHandler(this);        

        rows[i].points = new wxStaticText(panel, 
            wxID_ANY, 
            wxT("0"),
            wxDefaultPosition,
            wxDefaultSize,
            wxALIGN_RIGHT);
        rows[i].points->SetFont(wxFont(font));        
        rows[i].points->SetEventHandler(this);

        sizer->Add(rows[i].name, 9, wxEXPAND | wxALIGN_LEFT);
        sizer->Add(rows[i].points, 1, wxEXPAND | wxALIGN_RIGHT);
        
        panel->SetSizer(sizer);

        topSizer->Add(panel, 1, wxEXPAND);
    }

    topPanel->SetSizer(topSizer);
    topSizer->Fit(this);
    topSizer->Layout();
    
    this->SetTransparent(200);
    
    teamCount = MAX_TEAMS;
    
    //laikmatis
    timer = new wxTimer(this, ID_Timer);
    watch = new wxStopWatch();
}

void PMMainFrame::SetColour(int pos, wxColour colour) {
    if ((pos > 0) && (pos <= MAX_TEAMS)) {
        rows[pos].name->SetBackgroundColour(colour);
        rows[pos].points->SetBackgroundColour(colour);        
        this->Refresh();
    }    
}

void PMMainFrame::SetTeams (int count) {
   
    if ((count > 0) && (count <= MAX_TEAMS)) {
       if (teamCount > count) { //REMOVE teams

            for (int i = teamCount; i > count; i--) {
                topSizer->Show(i, false);
            }
        } else if (teamCount < count) { //add teams
            for (int i = teamCount; i < count; i++) {
                topSizer->Show(i, true);
            }
        }
        topSizer->Layout();
        topSizer->Fit(this);
        
        teamCount = count;
    }
    
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
        wxMessageBox("ok");
//        this->SetColour(1, wxColour(0xff, 0, 0));        
        if (!this->timer->IsRunning()) {
            TimerStart();
        } else {
            TimerStop();
        }
    } else {
//    if ( !processed )
        rc = wxFrame::MSWWindowProc(message, wParam, lParam);
    }

    return rc;
}

//mouse event handling
void PMMainFrame::OnMouseEvent(wxMouseEvent& event) {

    if (event.LeftDown())
    {
        dragMode = 1;
        dragStart = event.GetPosition();
        startPos = this->GetScreenPosition();
    }
    else if (event.LeftUp() && dragMode != 0)
    {
        dragMode = 0;
        
        wxPoint current = event.GetPosition();
        current = current - dragStart;
        wxPoint window = this->GetScreenPosition();        
        this->Move(window + current);

        Refresh(true);
    }
    else if (event.Dragging() && dragMode != 0)
    {
        wxPoint current = event.GetPosition();
        current = current - dragStart;
        wxPoint window = this->GetScreenPosition();                
        this->Move(window + current);

        Refresh(true);

    }
}
        
void PMMainFrame::OnTimer(wxTimerEvent& event) {
    long elapse = watch->Time();
    int milliseconds = (elapse % 1000) / 10;
    int seconds = (elapse / 1000) % 60;
    int minutes = elapse / (60*1000);
    
    if (seconds >= BEEP_START) {
       if (beeps[seconds - BEEP_START]) {
            Beep(BEEP_NOTICE, 50);
            beeps[seconds - BEEP_START] = false;
        }
    }
        
    if (minutes == 1) {
        TimerStop();
        seconds = 0;
        milliseconds = 0;
    }

    wxString time;
    time.Printf("%02d:%02d:%02d", minutes, seconds, milliseconds);
    timerDisplay->SetLabel(time);
}

//timer functions
void PMMainFrame::TimerStart(bool reset) {
    if (reset) {
        for (int i = 0; i < 5; i++) {
            beeps[i] = true;
        }
        watch->Start();
    }
    if (!timer->IsRunning()) {
        timer->Start(TIMER_INTERVAL);
    }
}

void PMMainFrame::TimerStop() {
    for (int i = 0; i < 5; i++) {
        beeps[i] = true;
    }
    watch->Pause();
    if (timer->IsRunning()) {
        timer->Stop();
    }
}

void PMMainFrame::TimerPause() {
    timer->Stop();
}

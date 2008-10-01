/*
 * Protmusio zaidimo valdymo programa.
 *
 * Domas Monkus, 2008.09
 */
#define wxUSE_UNICODE 1
 
#include "pm_main_frame.h"
#include "beep_thread.h"
#include "pm_settings_frame.h"
#include "msgs.h"

#include <cstdlib> 
#include <ctime> 

DEFINE_LOCAL_EVENT_TYPE( EVT_BEEPER )

BEGIN_EVENT_TABLE(PMMainFrame, wxFrame)
    EVT_MOUSE_EVENTS(PMMainFrame::OnMouseEvent)
    EVT_TIMER(ID_Timer, PMMainFrame::OnTimer)
    //settings dialog
    EVT_CHAR(PMMainFrame::OnKey)
    //paleisti laikmati (gaunama is signalo gijos)
    EVT_CUSTOM( EVT_BEEPER, wxID_ANY, PMMainFrame::OnBeeper )
END_EVENT_TABLE()

PMMainFrame::PMMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxFrame((wxFrame *)NULL, -1, title, pos, size, style)
{

    CreateControls();
    
    defaultBackground = rows[0].panel->GetBackgroundColour();
    
    //parametrai
    dragMode = 0;
    resetOnStart = true;
    beepRandom = BEEP_RANDOM;
    beepStart = BEEP_MIN;
    beepEnd = BEEP_MAX;
    
    teamCount = MAX_TEAMS;
    
    srand((unsigned)time(0)); 
    
    state = STATE_NOT_RUNNING;
    
    for (int i = 0; i < teamCount; i++) {
        rows[i].state = TEAM_WAITING;
        rows[i].score = 0;
    }
   
    //laikmatis
    timer = new wxTimer(this, ID_Timer);
    watch = new wxStopWatch();
    
    hookActive = 1;
    
    t.open("t.txt");

}

void PMMainFrame::CreateControls() {
    wxPanel * topPanel = new wxPanel(this);
    topSizer = new wxBoxSizer(wxVERTICAL);
    wxFont font(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    //laikmacio isvedimas
    timerDisplay = new wxStaticText(topPanel,
        wxID_ANY,
        wxT("01:00:00"),
        wxDefaultPosition,
        wxDefaultSize,
        wxALIGN_CENTER);
    timerDisplay->SetFont(wxFont(font));
    timerDisplay->SetEventHandler(this);
    topSizer->Add(timerDisplay, 1, wxFIXED_MINSIZE | wxALIGN_CENTER);


    for (int i = 0; i < MAX_TEAMS; i++) {
        wxPanel * panel = new wxPanel(topPanel);

        wxBoxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
        rows[i].panel = panel;

        rows[i].nameDisplay = new wxStaticText(panel,
            wxID_ANY,
            wxString(wxT("Komanda")),
            wxDefaultPosition,
            wxDefaultSize,
            wxALIGN_LEFT);
        rows[i].nameDisplay->SetFont(wxFont(font));
        rows[i].nameDisplay->SetEventHandler(this);

        rows[i].pointsDisplay = new wxStaticText(panel,
            wxID_ANY,
            wxT("0"),
            wxDefaultPosition,
            wxDefaultSize,
            wxALIGN_RIGHT);
        rows[i].pointsDisplay->SetFont(wxFont(font));
        rows[i].pointsDisplay->SetEventHandler(this);

        sizer->Add(rows[i].nameDisplay, 1, wxEXPAND | wxALIGN_LEFT | wxRIGHT, 10);
        sizer->Add(rows[i].pointsDisplay, 0, wxEXPAND | wxALIGN_RIGHT);

        panel->SetSizer(sizer);

        topSizer->Add(panel, 1, wxEXPAND);
    }

    topPanel->SetSizer(topSizer);
    topSizer->Layout();
    topSizer->Fit(this);


    this->SetTransparent(200);
}

void PMMainFrame::SetColour(int pos, wxColour colour) {
    if ((pos >= 0) && (pos < MAX_TEAMS)) {
        rows[pos].nameDisplay->SetBackgroundColour(colour);
        rows[pos].pointsDisplay->SetBackgroundColour(colour);        
        this->Refresh();
    }    
}

void PMMainFrame::SetTeam(int pos, wxString & name) {
    if ((pos >= 0) && (pos < MAX_TEAMS)) {
        rows[pos].nameDisplay->SetLabel(name);
    }
    topSizer->Layout();
    topSizer->Fit(this);
}

void PMMainFrame::SetTeams (int count) {
   
    if ((count > 0) && (count <= MAX_TEAMS)) {
       if (teamCount > count) { //REMOVE teams

            for (int i = teamCount; i > count; i--) {
                topSizer->Show(i, false);
            }
        } else if (teamCount < count) { //add teams
            for (int i = teamCount; i <= count; i++) {
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
    t.close();
    Close(TRUE);
}

void PMMainFrame::OnTest(wxEvent& WXUNUSED(event))
{

}


WXLRESULT PMMainFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    WXLRESULT rc = 0;
    switch (message) {
        case MSG_SCORE:
            SetScore(wParam);
            break;
        case MSG_START:
            StartGame();
            break;
        case MSG_PAUSE :
            PauseGame();
            break;
        case MSG_RESET:
            ResetGame();
            break;
        case MSG_GAMEKEY:
            
            if (state != STATE_NOT_RUNNING) {
                ProcessGameKey(wParam, lParam);
            }
            break;
        default:
        rc = wxFrame::MSWWindowProc(message, wParam, lParam);
    }
    

    return rc;
}

void PMMainFrame::ProcessGameKey(long team, long alt) {
    t << "team: " << team << " alt " << alt << endl;
    if ((alt == 0) && (rows[team].state == TEAM_WAITING)) {
        if (state == STATE_RUNNING) {
            SetTeamState(team, TEAM_ANSWERING);
            PauseGame();
            BeepThread * beep = new BeepThread(BEEP_CLICK, BEEP_CLICK_LEN);
            if (beep->Create() == wxTHREAD_NO_ERROR) {
                beep->Run();
            }
            
        } else if (state == STATE_BEEPING) {
            SetTeamState(team, TEAM_BLOCKED);            
        }
        UpdateTeams();
        
    } else if (alt != 0) { //lygiosios
        int draw = 0;
        int newState = TEAM_DRAW;
        if (state == STATE_BEEPING) {
            newState = TEAM_BLOCKED;
        }
        if (alt == KS_LSHIFT) {
            switch (team) {
                case 3:
                    if (SetTeamState(2, newState)) {
                        draw++;
                    }
                    if (SetTeamState(1, newState)) {
                        draw++;
                    }
                    break;
                case 2:
                    if (SetTeamState(2, newState)) {
                        draw++;
                    }
                    if (SetTeamState(0, newState)) {
                        draw++;
                    }

                    break;
                case 1:
                    if (SetTeamState(0, newState)) {
                        draw++;
                    }
                    if (SetTeamState(1, newState)) {
                        draw++;
                    }
                    break;
                case 0:
                    if (SetTeamState(2, newState)) {
                        draw++;
                    }
                    if (SetTeamState(3, newState)) {
                        draw++;
                    }

            }
        } else if (alt == KS_LALT) {
            switch(team) {
                case 3:
                    if (SetTeamState(3, newState)) {
                        draw++;
                    }
                    if (SetTeamState(1, newState)) {
                        draw++;
                    }
                    break;
                case 2:
                    if (SetTeamState(3, newState)) {
                        draw++;
                    }
                    if (SetTeamState(0, newState)) {
                        draw++;
                    }
                    break;
                case 1:
                    if (SetTeamState(0, newState)) {
                        draw++;
                    }
                    if (SetTeamState(1, newState)) {
                        draw++;
                    }
                    if (SetTeamState(2, newState)) {
                        draw++;
                    }
                    if (SetTeamState(3, newState)) {
                        draw++;
                    }
                    break;
            }
        } else if (alt == KS_LCTRL) {
            switch(team) {
                case 3:
                    if (SetTeamState(1, newState)) {
                        draw++;
                    }
                    if (SetTeamState(2, newState)) {
                        draw++;
                    }
                    if (SetTeamState(3, newState)) {
                        draw++;
                    }
                    break;
                case 2:
                    if (SetTeamState(3, newState)) {
                        draw++;
                    }
                    if (SetTeamState(2, newState)) {
                        draw++;
                    }
                    if (SetTeamState(0, newState)) {
                        draw++;
                    }
                    break;
                case 1:
                    if (SetTeamState(3, newState)) {
                        draw++;
                    }
                    if (SetTeamState(1, newState)) {
                        draw++;
                    }
                    if (SetTeamState(0, newState)) {
                        draw++;
                    }
                    break;
                case 0:
                    if (SetTeamState(2, newState)) {
                        draw++;
                    }
                    if (SetTeamState(1, newState)) {
                        draw++;
                    }
                    if (SetTeamState(0, newState)) {
                        draw++;
                    }
                    break;
            }
        }
        if ((newState == TEAM_DRAW) && (draw > 0)) {
            PauseGame();
            
            BeepThread * beep;
            if (draw > 1) {
                beep = new BeepThread(BEEP_DRAW, 100);
            } else {
                beep = new BeepThread(BEEP_CLICK, BEEP_CLICK_LEN);

                for (int i = 0; i < teamCount; i++) {
                    if (rows[i].state == TEAM_DRAW) {
                        rows[i].state = TEAM_ANSWERING;
                    }
                }
              
            }
            
            if (beep->Create() == wxTHREAD_NO_ERROR) {
                beep->Run();
            }

            
        }
        
    }
    UpdateTeams();

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
        if ((abs(current.x) + abs(current.y)) > 0) {
            
            wxPoint window = this->GetScreenPosition();                
            this->Move(window + current);

            Refresh(true);
        }

    }
}
        
void PMMainFrame::OnTimer(wxTimerEvent& event) {
    long elapse = START_TIME - watch->Time();
    
    int milliseconds = (elapse % 1000) / 10;
    int seconds = (elapse / 1000) % 60;
    int minutes = elapse / (60*1000);
    
    if ((seconds > 0) && (seconds <= BEEP_START)) {
        if (((milliseconds == 0) && (seconds - 1 == beepCount))
            || ((milliseconds != 0) && (seconds == beepCount))) {
                beepCount--;
            BeepThread * beep = new BeepThread(BEEP_NOTICE, 50);
            if (beep->Create() == wxTHREAD_NO_ERROR) {
                beep->Run();
            }

        }
        
    }
        
    if ((minutes <= 0) && (seconds <= 0) && (milliseconds <= 0)) {
        TimerStop();
        BeepThread * beep = new BeepThread(BEEP_END, 500);
        if (beep->Create() == wxTHREAD_NO_ERROR) {
            beep->Run();
        }
        ResetGame();
        
        elapse = 0;        
    }


    ShowTime(elapse);
}

//timer functions
void PMMainFrame::TimerStart(bool reset) {
    if (reset) {
        beepCount = BEEP_START - 1;
        for (int i = 0; i < 5; i++) {
            beeps[i] = true;
        }
        watch->Start(); 
    } else {
        watch->Resume();
        
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
    watch->Pause();
}

void PMMainFrame::ShowTime(long timeMs) {
    
    int milliseconds = (timeMs % 1000) / 10;
    int seconds = (timeMs / 1000) % 60;
    int minutes = timeMs / (60*1000);
    
    wxString time;
    time.Printf(wxT("%02d:%02d:%02d"), minutes, seconds, milliseconds);
    timerDisplay->SetLabel(time);
}

void PMMainFrame::EditSettings() {
    hookActive = 0;
    PMSettings dialog(this);
    
    dialog.SetCount(teamCount, true);
    for (int i = 0; i < teamCount; i++) {
        wxString name = rows[i].nameDisplay->GetLabel();
        dialog.SetName(i, name);
    }
    
    dialog.SetRandom(beepRandom);
    dialog.SetBeepLimits(beepStart, beepEnd);
    
    if (dialog.ShowModal() == wxID_OK) {
        SetTeams(dialog.GetCount());
        for (int i=0; i < teamCount; i++) {
            wxString label = dialog.GetName(i);
            SetTeam(i, label);
        }
    }
    
    beepRandom = dialog.GetRandom();
    dialog.GetBeepLimits(beepStart, beepEnd);
    hookActive = 1;
}

void PMMainFrame::OnKey(wxKeyEvent & event) {
    int code = event.GetKeyCode();
    switch(code) {
        case ((int)'g'):
            EditSettings();
            break;
        default: 
            event.Skip(false);
            break;
    }
}

void PMMainFrame::StartGame() {

    if (state == STATE_NOT_RUNNING) {
            
        if (resetOnStart) {
            for (int i = 0; i < teamCount; i++) {
                SetTeamState(i, TEAM_WAITING);
            }
        }
        for (int i = 0; i < teamCount; i++) {
            if (rows[i].state == TEAM_DRAW) {
                rows[i].state = TEAM_WAITING;
            }
        }
        UpdateTeams();
        
        int beep = beepStart;
        if (beepRandom) {
            int diff = beepEnd - beepStart;
            double random = (double)rand() / RAND_MAX; 
            beep = beepStart + (int)(diff * random);
        }
        
    
        BeepThread * beepT = new BeepThread(BEEP_GAMESTART, beep, &state, this);
        if (beepT->Create() == wxTHREAD_NO_ERROR) {
            beepT->Run();
        }
    }
}

void PMMainFrame::PauseGame() {
    TimerPause();
    state = STATE_NOT_RUNNING;
}

void PMMainFrame::ResetGame() {
    TimerStop();
    ShowTime(START_TIME);
    resetOnStart = true;
    //komandu bukles i pradzia
    for (int i = 0; i < teamCount; i++) {
        rows[i].state = TEAM_WAITING;
    }
    UpdateTeams();
    state = STATE_NOT_RUNNING;
}

void PMMainFrame::OnBeeper(wxEvent & event) {
    TimerStart(resetOnStart);
    resetOnStart = false;
    
}

void PMMainFrame::UpdateTeams() {
    for (int i = 0; i < teamCount; i++) {
        switch (rows[i].state) {
            case TEAM_WAITING:
                rows[i].panel->SetBackgroundColour(defaultBackground);
                break;
            case TEAM_ANSWERING:
                rows[i].panel->SetBackgroundColour(wxColour(0, 0xaa, 0));
                break;
            case TEAM_BLOCKED:
                rows[i].panel->SetBackgroundColour(wxColour(0xaa, 0, 0));
                break;
            case TEAM_DRAW:
                rows[i].panel->SetBackgroundColour(wxColour(0xaa, 0xaa, 0));
                break;
                
        }
        rows[i].pointsDisplay->SetLabel(wxString::Format(wxT("%d"), rows[i].score));
    }
    this->Refresh();
}
  
bool PMMainFrame::SetTeamState(int pos, int newState) {
    if ((pos >= 0) && (pos < MAX_TEAMS)) {
        if ((rows[pos].state == TEAM_WAITING) || (newState == TEAM_WAITING)) {
            rows[pos].state = newState;
            return true;
        } 
    }
    return false;
}

void PMMainFrame::SetScore(long ch) {
    long code = ch - (long)'1';
    int diff = 1;
    if (code % 2) {
        diff = -1;
    }
    int team = code / 2;
    SetScore(team, diff);
    UpdateTeams();
    topSizer->Fit(this);
}

void PMMainFrame::SetScore(int team, int diff) {
    if ((team >= 0) && (team < teamCount)) {
        if (rows[team].score + diff >= 0) {
            rows[team].score += diff;
        }
    }
}
        

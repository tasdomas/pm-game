#include "pm_settings_frame.h"
#include "wx/spinctrl.h"
#include "wx/textctrl.h"

enum
{
    ID_Spin = 1,
    ID_Cancel,
    ID_Ok,
};

BEGIN_EVENT_TABLE(PMSettings, wxDialog)
    EVT_SPIN_UP(ID_Spin, PMSettings::OnCountChange)
    EVT_SPIN_DOWN(ID_Spin, PMSettings::OnCountChange)
    EVT_BUTTON(ID_Cancel, PMSettings::OnCancel)
    EVT_BUTTON(ID_Ok, PMSettings::OnOk)
END_EVENT_TABLE()

PMSettings::PMSettings(wxWindow * parent, wxWindowID)
    : wxDialog(parent, (wxWindowID)wxID_ANY, wxString(wxT("Nustatymai"))) {
    
    CreateControls();
}

void PMSettings::CreateControls() {
    wxPanel * topPanel = new wxPanel(this);
    topSizer = new wxBoxSizer(wxVERTICAL);
    topPanel->SetSizer(topSizer);
    
    // komandu sk.
    wxPanel * panel = new wxPanel(topPanel);
    wxBoxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
    panel->SetSizer(sizer);
    wxStaticText * text = new wxStaticText(panel, wxID_ANY, wxString("Sk."));
    sizer->Add(text, 0, wxRIGHT | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 10);
    
    countDisplay = new wxSpinCtrl(panel, ID_Spin, wxT("4"), 
        wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 
        1, MAX_TEAMS, 4, wxT("teamCount"));
    sizer->Add(countDisplay, 3, wxEXPAND | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    
    topSizer->Add(panel, 1, wxEXPAND);
    
   
    for (int i = 0; i < MAX_TEAMS; i++) {
        teamsDisplay[i] = new wxTextCtrl(topPanel, wxID_ANY);
        topSizer->Add(teamsDisplay[i], 1, wxEXPAND | wxALIGN_CENTER);
    }
    teamCount = MAX_TEAMS;
    
    panel = new wxPanel(topPanel);
    sizer = new wxBoxSizer(wxHORIZONTAL);
    panel->SetSizer(sizer);
    
    wxButton * btnCancel = new wxButton(panel, ID_Cancel, _T("Atsaukti"));
    sizer->Add(btnCancel, 1, wxEXPAND | wxALIGN_LEFT);

    wxButton * btnOk = new wxButton(panel, ID_Ok, _T("Saugoti"));
    sizer->Add(btnOk, 1, wxEXPAND | wxALIGN_RIGHT);
    
    topSizer->Add(panel);
    
    
    topSizer->Fit(this);
    topSizer->Layout();
}

void PMSettings::SetCount(int count, bool set) {
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
        this->Refresh();

        teamCount = count;
        if (set) {
            countDisplay->SetValue(count);        
        }
    }    
}

void PMSettings::OnCountChange(wxSpinEvent & event) {
    int cnt = 0;
    cnt = event.GetPosition();
    cnt = cnt;
    if (cnt != teamCount) {
        SetCount(cnt);
    }
}

int PMSettings::GetCount() {
    return teamCount;
}

void PMSettings::SetName(int pos, wxString & label) {
    if ((pos > 0) && (pos < MAX_TEAMS)) {
        teamsDisplay[pos]->SetValue(label);
    }
}
    
wxString PMSettings::GetName(int pos) {
    if ((pos > 0) && (pos < teamCount)) {
        return teamsDisplay[pos]->GetValue();
    }
    return wxT("");
}    

void PMSettings::OnOk(wxCommandEvent & event) {
    if (IsModal()) {
        EndModal(wxID_OK);
    } else {
        SetReturnCode(wxID_OK);
        this->Show(false);
    }
}

void PMSettings::OnCancel(wxCommandEvent & event) {
    if (IsModal()) {
        EndModal(wxID_CANCEL);
    } else {
        SetReturnCode(wxID_CANCEL);
        this->Show(false);
    }
}    

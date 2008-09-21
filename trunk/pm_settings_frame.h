#ifndef PM_GAME_SETTINGS
#define PM_GAME_SETTINGS

#include "wx/dialog.h"
#include "pm_main_frame.h"
#include "wx/textctrl.h"
#include "wx/spinctrl.h"


class PMSettings : public wxDialog {
private:
    int teamCount;
    wxString teams[MAX_TEAMS];
    wxTextCtrl * teamsDisplay[MAX_TEAMS];
    wxSizer * topSizer;
    
public:
    PMSettings(wxWindow * parent, wxWindowID = wxID_ANY);  
    void CreateControls();
    void SetCount(int count);
    void OnCountChange(wxSpinEvent & event);
    
    DECLARE_EVENT_TABLE()
};

#endif

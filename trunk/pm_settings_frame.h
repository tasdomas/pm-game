#ifndef PM_GAME_SETTINGS
#define PM_GAME_SETTINGS

#include "wx/dialog.h"
#include "pm_main_frame.h"
#include "wx/textctrl.h"
#include "wx/spinctrl.h"


class PMSettings : public wxDialog {
private:
    int teamCount;
    wxTextCtrl * teamsDisplay[MAX_TEAMS];
    wxSizer * topSizer;
    wxSpinCtrl * countDisplay;
    
public:
    PMSettings(wxWindow * parent, wxWindowID = wxID_ANY);  
    void CreateControls();
    
    //events
    void OnCountChange(wxSpinEvent & event);
    void OnCancel(wxCommandEvent & event);
    void OnOk(wxCommandEvent & event);
    
    //setters & getters
    void SetCount(int count, bool set = false);    
    int GetCount();
    
    void SetName(int pos, wxString & label);
    wxString GetName(int pos);
    
    
    DECLARE_EVENT_TABLE()
};

#endif

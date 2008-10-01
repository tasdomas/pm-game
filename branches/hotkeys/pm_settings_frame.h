#ifndef PM_GAME_SETTINGS
#define PM_GAME_SETTINGS

#include "wx/dialog.h"
#include "pm_main_frame.h"
#include "wx/textctrl.h"
#include "wx/spinctrl.h"

class PMSettings : public wxDialog {
private:
    int teamCount;
    wxSizer * topSizer;
    wxSpinCtrl * countDisplay; //komandu sk.
    wxTextCtrl * teamsDisplay[MAX_TEAMS]; //komandu pavadinimai    
    //signalo ilgio ribos
    wxTextCtrl * beepIntStart; 
    wxTextCtrl * beepIntEnd;
    wxCheckBox * beepRandom; //ar signalo trukmë parenkama atsitiktinai?
    
public:
    PMSettings(wxWindow * parent, wxWindowID = wxID_ANY);  
    void CreateControls();
    
    //events
    void OnCountChange(wxSpinEvent & event);
    void OnCancel(wxCommandEvent & event);
    void OnOk(wxCommandEvent & event);
    void OnRandomChange(wxCommandEvent & event); 
    
    void UpdateRandom();
    
    //setters & getters
    void SetCount(int count, bool set = false);    
    int GetCount();
    
    bool GetRandom();
    void SetRandom(bool value);
    void GetBeepLimits(int& min, int& max);
    void SetBeepLimits(int min, int max);
    
    void SetName(int pos, wxString & label);
    wxString GetName(int pos);
    
    
    DECLARE_EVENT_TABLE()
};

#endif

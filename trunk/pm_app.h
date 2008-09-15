#ifndef PM_GAME_APP
#define PM_GAME_APP

#include <windows.h>
#include "wx/wx.h" 

class PMApp: public wxApp
{
private:
    HHOOK hook;

public:
    virtual bool OnInit();
    virtual int OnExit();

};

#endif

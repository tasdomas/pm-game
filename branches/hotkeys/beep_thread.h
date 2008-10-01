#ifndef PM_GAME_BEEP_THREAD
#define PM_GAME_BEEP_THREAD

#include <windows.h>

#include "wx/thread.h"
#include "pm_main_frame.h"

class BeepThread : public wxThread {
    public:
        BeepThread(long freq, long dur);
        BeepThread(long freq, long dur, int * stateVar, PMMainFrame * pparent);
        virtual void * Entry();
        PMMainFrame * parent;
    private:
        int * state;
        long frequency;
        long duration;
};           

#endif

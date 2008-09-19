#ifndef PM_GAME_BEEP_THREAD
#define PM_GAME_BEEP_THREAD

#include <windows.h>

#include "wx/thread.h"

class BeepThread : public wxThread {
    public:
        BeepThread(long freq, long dur);
        virtual void * Entry();
    private:
        long frequency;
        long duration;
};           

#endif

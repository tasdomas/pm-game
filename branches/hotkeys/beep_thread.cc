#include "beep_thread.h"
#include "msgs.h"
#include "pm_main_frame.h"

BeepThread::BeepThread(long freq, long dur) 
: wxThread(wxTHREAD_DETACHED) {
    frequency = freq;
    duration = dur;
    state = NULL;
    parent = NULL;
}

BeepThread::BeepThread(long freq, long dur, int * stateVar, PMMainFrame * pparent)
: wxThread(wxTHREAD_DETACHED) {
    state = stateVar;
    frequency = freq;
    duration = dur;
    parent = pparent;
}    

void *BeepThread::Entry() {
    if (state != NULL) {
        *state = STATE_BEEPING;
    }
    Beep(frequency, duration);
    if (state != NULL) {
        *state = STATE_RUNNING;
    }
    if (parent != NULL) {
        wxCommandEvent evt(EVT_BEEPER);
        wxPostEvent(parent, evt);
    }
    
    return NULL;
}
    

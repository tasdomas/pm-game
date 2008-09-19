#include "beep_thread.h"

BeepThread::BeepThread(long freq, long dur) 
: wxThread(wxTHREAD_DETACHED) {
    frequency = freq;
    duration = dur;
}

void *BeepThread::Entry() {
    Beep(frequency, duration);
    return NULL;
}
    

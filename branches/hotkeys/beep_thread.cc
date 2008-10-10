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

   WAVEFORMATEX waveFormat;
    ZeroMemory(&waveFormat, sizeof(waveFormat));

    waveFormat.cbSize = sizeof(waveFormat);
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 1;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 8;
    waveFormat.nBlockAlign = waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

    HWAVEOUT waveOut = NULL;
    MMRESULT openRes = waveOutOpen(&waveOut, WAVE_MAPPER, &waveFormat, 0, 0,
        CALLBACK_NULL | WAVE_FORMAT_DIRECT);

    long count = (long)(waveFormat.nSamplesPerSec * duration / 1000);

    if(openRes == MMSYSERR_NOERROR)
    {
        WAVEHDR hdr;
        ZeroMemory(&hdr, sizeof(hdr));
        unsigned char *data = new unsigned char[count];
        if(data != NULL)
        {
            double x;
            for(long i = 0; i < count; i++)
            {
               x = sin(i*2.0*3.14*(frequency)/(double)waveFormat.nSamplesPerSec);

               data[i] = (char)(127*x+128);

            }
            hdr.dwBufferLength = sizeof(unsigned char) * count;
            hdr.lpData = (LPSTR)data;
            hdr.dwLoops = 1;

            MMRESULT prepareRes = waveOutPrepareHeader(waveOut, &hdr, sizeof(hdr));
            hdr.dwFlags |= WHDR_BEGINLOOP | WHDR_ENDLOOP;

            if(prepareRes == MMSYSERR_NOERROR)
            {
                MMRESULT playRes = waveOutWrite(waveOut, &hdr, sizeof(hdr));
                if(playRes == MMSYSERR_NOERROR) {
                    while(waveOutUnprepareHeader(waveOut, &hdr, sizeof(hdr)) == WAVERR_STILLPLAYING)
                    {
                        this->Sleep(100);
                    }
                }
            }
            delete[] data;
        }

    }


    if (state != NULL) {
        *state = STATE_RUNNING;
    }
    if (parent != NULL) {
        wxCommandEvent evt(EVT_BEEPER);
        wxPostEvent(parent, evt);
    }
    
    return NULL;
}
    

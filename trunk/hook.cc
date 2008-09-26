#define _WIN32_WINNT 0x0500
#include <windows.h>
#include "msgs.h"

extern HWND window;

/*
 * Keyboard hook - perima klaviaturos paspaudimus ir
 * persiuncia musu programai.
 */
LRESULT CALLBACK LowLevelKeyboard(int nCode, WPARAM wParam, LPARAM lParam)
{ PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
  //
if (nCode==HC_ACTION
    //&& (GetKeyState(VK_RCONTROL) & 0x8000)
    && (wParam==WM_KEYDOWN || wParam==WM_SYSKEYDOWN))
  { 
        
    BYTE ks[256];
    GetKeyboardState(ks);

    WORD w =  0;
    //UINT scan=0;
    // apdorojam paprastus klavisus
    ToAscii(p->vkCode,p->scanCode,ks,&w,0);
    if (w > 0) {
        char ch = char(w); 
           
        switch (ch) {
            case KEY_START:
                PostMessage(window, MSG_START, 0, 0);
                return 1;
                break;
            case KEY_PAUSE:
                PostMessage(window, MSG_PAUSE, 0, 0);
                return 1;
                break;
            case KEY_RESET:
                PostMessage(window, MSG_RESET, 0, 0);
                return 1;
                break;
        }
    } else {
        
        switch(p->vkCode) {
            case VK_F9:
            case VK_F10:
            case VK_F11:
            case VK_F12:    
                long flag = 0;
                if (GetKeyState(VK_LCONTROL) & 0x8000) {
                    flag = flag | KS_LCTRL;
                }
                if (GetKeyState(VK_LSHIFT) & 0x8000) {
                    flag = flag | KS_LSHIFT;
                }
                if (GetKeyState(VK_LMENU) & 0x8000) {
                    flag = flag | KS_LALT;
                }
                PostMessage(window, MSG_GAMEKEY, flag, 0);
                return 1;
                break;
        }
    }            

  }
  /*
  if (nCode==HC_ACTION && p->vkCode==VK_RMENU && (wParam==WM_KEYUP || wParam==WM_SYSKEYUP) && keydown)
  { INPUT ip; ZeroMemory(&ip,sizeof(ip)); ip.type=INPUT_MOUSE; ip.mi.dwFlags=MOUSEEVENTF_RIGHTUP;
    SendInput(1,&ip,sizeof(ip));
    return 1;
  }
  */
  return CallNextHookEx(0,nCode,wParam,lParam);
}

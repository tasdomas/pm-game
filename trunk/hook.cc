#define _WIN32_WINNT 0x0500
#include <windows.h>

#include "msgs.h"

extern HWND window;
extern int hookActive;

/*
 * Keyboard hook - perima klaviaturos paspaudimus ir
 * persiuncia musu programai.
 */
LRESULT CALLBACK LowLevelKeyboard(int nCode, WPARAM wParam, LPARAM lParam)
{ PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
  //
if ((hookActive) && nCode==HC_ACTION
    //&& (GetKeyState(VK_RCONTROL) & 0x8000)
    && (wParam==WM_KEYDOWN || wParam==WM_SYSKEYDOWN))
  { 
        
    BYTE ks[256];
    GetKeyboardState(ks);

    WORD w =  0;
    //UINT scan=0;
    // apdorojam paprastus klavisus
    if (ToAscii(p->vkCode,p->scanCode,ks,&w,0) == 1) {

        char ch = char(w);
        //tasku redagavimas
        if ((ch >= '1') && (ch <= '8')) {
            PostMessage(window, MSG_SCORE, (long)ch, 0);
            return 1;
        }
    } else {
        
        switch(p->vkCode) {
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
            
            
            case VK_F9:
            case VK_F10:
            case VK_F11:
            case VK_F12:    
                long team = 0;
                if (p->vkCode == VK_F9) {
                    team = 3;
                } else if (p->vkCode == VK_F10) {
                    team = 2;
                } else if (p->vkCode == VK_F11) {
                    team = 1;
                } else if (p->vkCode == VK_F12) {
                    team = 0;
                };
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
                PostMessage(window, MSG_GAMEKEY, team, flag);
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

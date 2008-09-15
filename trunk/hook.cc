#define _WIN32_WINNT 0x0500
#include <windows.h>

extern HWND window;

/*
 * Keyboard hook - perima klaviaturos paspaudimus ir
 * persiuncia musu programai.
 */
LRESULT CALLBACK LowLevelKeyboard(int nCode, WPARAM wParam, LPARAM lParam)
{ PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
  //



  static bool keydown=false;
  if (nCode==HC_ACTION && p->vkCode==VK_RMENU && (wParam==WM_KEYDOWN || wParam==WM_SYSKEYDOWN))
  { keydown=true;
    //INPUT ip; ZeroMemory(&ip,sizeof(ip)); ip.type=INPUT_MOUSE; ip.mi.dwFlags=MOUSEEVENTF_RIGHTDOWN;
    //SendInput(1,&ip,sizeof(ip));
    PostMessage(window, WM_USER+1, 0, 0);
    return 1;
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

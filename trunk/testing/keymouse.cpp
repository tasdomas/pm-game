#define _WIN32_WINNT 0x0500
#include <windows.h>

// This program installs a "low level keyboard hook".
// Whenever the right alt key (VK_RMENU) goes down, we generate a right-mouse-down event.
// When the right alt key goes up, we generate a right-mouse-up event.
// To respond to other keypresses instead, look in winuser.h
//
// Generation of events is by SendInput(..), a Windows NT/2k/XP function.
//
// The keyboard hook is installed with SetWindowsHookEx(WH_KEYBOARD_LL,...),
// which is also specific to NT/2k/XP. This hook has the nice feature that it
// can "gobble up" keyboard events, so they're not seen by the rest of the system.
// Note: WH_KEYBOARD_LL always sets a global hook and so works for all applications.
// But unlike most global hooks (which have to reside in a DLL), the WH_KEYBOARD_LL
// can instead reside in an EXE. What Windows does is, whenever it receives a keypress,
// it switches to the context of the EXE that contained the hook, then invokes the hook
// function, then restores context back to what it was before.
//
// The main routine sets the hook, then creates an invisible top-level window, and when
// the top-level window gets closed (by ending it in the TaskManager), we unhook the hook.
// Note: it might seem nicer not to have any window at all, and just to enter an infinite
// loop. But (1) with a window we can exit more gracefully (and we can also choose to
// appear in the Process List of the taskmanager). And (2) keyboard hooks fail to work
// when there's just a tight infinite loop of Sleep(). It just wasn't working. Then when
// I created the window and did a message-loop it magically started working. My guess
// is that the context-switching might internally use the message-loop mechanism,
// or something like that.


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{ PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
  //
  static bool keydown=false;
  if (nCode==HC_ACTION && p->vkCode==VK_RMENU && (wParam==WM_KEYDOWN || wParam==WM_SYSKEYDOWN))
  { keydown=true;
    INPUT ip; ZeroMemory(&ip,sizeof(ip)); ip.type=INPUT_MOUSE; ip.mi.dwFlags=MOUSEEVENTF_RIGHTDOWN;
    SendInput(1,&ip,sizeof(ip));
    return 1;
  }
  if (nCode==HC_ACTION && p->vkCode==VK_RMENU && (wParam==WM_KEYUP || wParam==WM_SYSKEYUP) && keydown)
  { INPUT ip; ZeroMemory(&ip,sizeof(ip)); ip.type=INPUT_MOUSE; ip.mi.dwFlags=MOUSEEVENTF_RIGHTUP;
    SendInput(1,&ip,sizeof(ip));
    return 1;
  }
  return CallNextHookEx(0,nCode,wParam,lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{ if (msg==WM_DESTROY) PostQuitMessage(0);
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{ HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);
  //  
  WNDCLASSEX wcex; ZeroMemory(&wcex,sizeof(wcex)); wcex.cbSize=sizeof(WNDCLASSEX);
  wcex.lpfnWndProc=(WNDPROC)WndProc; 
  wcex.hInstance=hInstance; 
  wcex.lpszClassName="KeyMouseClass";
  RegisterClassEx(&wcex);
  CreateWindowEx(WS_EX_NOACTIVATE,"KeyMouseClass", "KeyMouse",WS_POPUP|WS_VISIBLE,0,0,0,0,0,0,hInstance,0);
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {TranslateMessage(&msg); DispatchMessage(&msg);}
  //
  MessageBeep(0);
  UnhookWindowsHookEx(hook);
  return (int)msg.wParam;
}




#define _WIN32_WINNT 0x0500
#include <windows.h>

#pragma comment(linker, "/SECTION:.shared,RWS")
#pragma data_seg(".shared")
    HWND window;
#pragma data_seg()

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
  window = 0;
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




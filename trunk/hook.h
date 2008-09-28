#ifndef PM_GAME_HOOK

#define PM_GAME_HOOK

#define _WIN32_WINNT 0x0500
#include <windows.h>

// shared data
HWND window __attribute__((section ("shared"), shared)) = 0;
int hookActive __attribute__((section ("shared"), shared)) = 0;

/*
 * Keyboard Hook - forward declaration
 * (zr. hook.cc)
 */
LRESULT CALLBACK LowLevelKeyboard(int nCode, WPARAM wParam, LPARAM lParam);

#endif

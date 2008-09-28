#ifndef PM_GAME_MSGS
#define PM_GAME_MSGS

#define MSG_START WM_USER + 1
#define MSG_PAUSE WM_USER + 2
#define MSG_RESET WM_USER + 3
#define MSG_GAMEKEY WM_USER + 4
#define MSG_SCORE WM_USER + 5

//key states (sent from the hook)
#define KS_LALT 0x8
#define KS_LSHIFT 0x4
#define KS_LCTRL 0x2


//pradeti zaidima
#define KEY_START VK_F1
//sustabdyti zaidima
#define KEY_PAUSE VK_F2
//atstatyti
#define KEY_RESET VK_F3


#define STATE_NOT_RUNNING 0
#define STATE_BEEPING 1
#define STATE_RUNNING 2

#endif

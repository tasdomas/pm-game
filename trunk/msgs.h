#ifndef PM_GAME_MSGS
#define PM_GAME_MSGS

#define MSG_START WM_USER + 1
#define MSG_PAUSE WM_USER + 2
#define MSG_RESET WM_USER + 3
#define MSG_GAMEKEY WM_USER + 4

//key states (sent from the hook)
#define KS_LALT 0x8
#define KS_LSHIFT 0x4
#define KS_LCTRL 0x2


//ord('a')
#define KEY_START 'a'
//ord('s')
#define KEY_PAUSE 's'
//ord('d')
#define KEY_RESET 'd'


#define STATE_NOT_RUNNING 0
#define STATE_BEEPING 1
#define STATE_RUNNING 2

#endif

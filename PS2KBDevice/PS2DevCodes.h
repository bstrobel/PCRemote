/*
 * PS2DevCodes.h
 *
 * Created: 07.11.2015 09:45:11
 *  Author: Bernd
 */ 


#ifndef PS2DEVCODES_H_
#define PS2DEVCODES_H_

#define PS2DC_BREAK_CODE 0xF0
#define PS2DC_EXT_CODE 0xE0

/*
 *
 * Pause and Break key is not defined here, because they don't fit the schema and we don't need them anyway.
 * PrntScrn: Make[0xE0,0x12,0xE0,0x7C] and Break[0xE0,0xF0,0x12,0xE0,0xF0,0x7C]
 * Pause: Make[0xE1,0x14,0x77,0xE1,0xF0,0x14,0xF0,0x77] and no Break
 */

#define PS2DC_A 0x1C
#define PS2DC_B 0x32
#define PS2DC_C 0x21
#define PS2DC_D 0x23
#define PS2DC_E 0x24
#define PS2DC_F 0x2B
#define PS2DC_G 0x34
#define PS2DC_H 0x33
#define PS2DC_I 0x43
#define PS2DC_J 0x3B
#define PS2DC_K 0x42
#define PS2DC_L 0x4B
#define PS2DC_M 0x3A
#define PS2DC_N 0x31
#define PS2DC_O 0x44
#define PS2DC_P 0x4D
#define PS2DC_Q 0x15
#define PS2DC_R 0x2D
#define PS2DC_S 0x1B
#define PS2DC_T 0x2C
#define PS2DC_U 0x3C
#define PS2DC_V 0x2A
#define PS2DC_W 0x1D
#define PS2DC_X 0x22
#define PS2DC_Y 0x35
#define PS2DC_Z 0x1A
#define PS2DC_0 0x45
#define PS2DC_1 0x16
#define PS2DC_2 0x1E
#define PS2DC_3 0x26
#define PS2DC_4 0x25
#define PS2DC_5 0x2E
#define PS2DC_6 0x36
#define PS2DC_7 0x3D
#define PS2DC_8 0x3E
#define PS2DC_9 0x46

#define PS2DC_REV_ACC 0x0E
#define PS2DC_MINUS 0x4E
#define PS2DC_EQUALS 0x55
#define PS2DC_BACKSLASH 0x5D
#define PS2DC_BKSP 0x66
#define PS2DC_SPACE 0x29
#define PS2DC_TAB 0x0D
#define PS2DC_CAPS 0x58
#define PS2DC_L_SHIFT 0x12
#define PS2DC_L_CTRL 0x14
#define PS2DC_L_GUI_EXT 0x1F
#define PS2DC_L_ALT 0x11
#define PS2DC_R_SHIFT 0x59
#define PS2DC_R_CTRL_EXT 0x14
#define PS2DC_R_GUI_EXT 0x27
#define PS2DC_R_ALT_EXT 0x11
#define PS2DC_APPS_EXT 0x2F
#define PS2DC_ENTER 0x5A
#define PS2DC_ESC 0x76

#define PS2DC_F1 0x05
#define PS2DC_F2 0x06
#define PS2DC_F3 0x04
#define PS2DC_F4 0x0C
#define PS2DC_F5 0x03
#define PS2DC_F6 0x0B
#define PS2DC_F7 0x83
#define PS2DC_F8 0x0A
#define PS2DC_F9 0x01
#define PS2DC_F10 0x09
#define PS2DC_F11 0x78
#define PS2DC_F12 0x07

#define PS2DC_SQ_BRACKET_OPEN 0x54
#define PS2DC_SQ_BRACKET_CLOSE 0x5B
#define PS2DC_INSERT_EXT 0x70
#define PS2DC_HOME_EXT 0x6C
#define PS2DC_PG_UP_EXT 0x7D
#define PS2DC_DEL_EXT 0x71
#define PS2DC_END_EXT 0x69
#define PS2DC_PG_DN_EXT 0x7A
#define PS2DC_U_ARROW_EXT 0x75
#define PS2DC_L_ARROW_EXT 0x6B
#define PS2DC_D_ARROW_EXT 0x72
#define PS2DC_R_ARROW_EXT 0x74
#define PS2DC_NUM 0x77

#define PS2DC_KP_DIVIDE_EXT 0x4A
#define PS2DC_KP_MULTIPLY 0x7C
#define PS2DC_KP_MINUS 0x7B
#define PS2DC_KP_ADD 0x79
#define PS2DC_KP_ENTER_EXT 0x5A
#define PS2DC_KP_DOT 0x71
#define PS2DC_KP_0 0x70
#define PS2DC_KP_1 0x69
#define PS2DC_KP_2 0x72
#define PS2DC_KP_3 0x7A
#define PS2DC_KP_4 0x6B
#define PS2DC_KP_5 0x73
#define PS2DC_KP_6 0x74
#define PS2DC_KP_7 0x6C
#define PS2DC_KP_8 0x75
#define PS2DC_KP_9 0x7D


#define PS2DC_SEMICOLON 0x4C
#define PS2DC_SINGLE_QUOTE 0x52
#define PS2DC_COMMA 0x41
#define PS2DC_DOT 0x49
#define PS2DC_SLASH 0x4A

#define PS2DC_PWR_POWER_EXT 0x37
#define PS2DC_PWR_SLEEP_EXT 0x3F
#define PS2DC_PWR_WAKE_EXT 0x5E

#define PS2DC_MM_FWD_EXT 0x4D
#define PS2DC_MM_PREV_EXT 0x15
#define PS2DC_MM_STOP_EXT 0x3B
#define PS2DC_MM_PLAY_PAUSE_EXT 0x34
#define PS2DC_MM_MUTE_EXT 0x23
#define PS2DC_MM_VOL_UP_EXT 0x32
#define PS2DC_MM_VOL_DOWN_EXT 0x21
#define PS2DC_MM_SELECT_EXT 0x50
#define PS2DC_MM_EMAIL_EXT 0x48
#define PS2DC_MM_CALC_EXT 0x2B
#define PS2DC_MM_MY_COMP_EXT 0x40

#define PS2DC_WWW_SEARCH_EXT 0x10
#define PS2DC_WWW_HOME_EXT 0x3A
#define PS2DC_WWW_BACK_EXT 0x38
#define PS2DC_WWW_FORWARD_EXT 0x30
#define PS2DC_WWW_STOP_EXT 0x28
#define PS2DC_WWW_REFRESH_EXT 0x20
#define PS2DC_WWW_FAVOURITES_EXT 0x18


#endif /* PS2DEVCODES_H_ */
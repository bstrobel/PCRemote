/*
 * MCEMapper.c
 *
 * Created: 17.11.2015 08:02:52
 *  Author: Bernd
 */ 

#include "MCEMapper.h"
#include <util/delay.h>
#include "MCEMapperHelper.h"
#include "MCECodes.h"
#include "../PS2KBDevice/PS2KBDevice.h"
#include "../PS2KBDevice/PS2DevCodes.h"

#ifndef PWR_BTN_PORT
#	define PWR_BTN_PORT PORTD
#endif

#ifndef PWR_BTN_DDR
#	define PWR_BTN_DDR DDRD
#endif

#ifndef PWR_BTN_BIT
#	define PWR_BTN_BIT PORTD7
#endif

#ifndef PWR_SENSE_PIN
#	define PWR_SENSE_PIN PIND
#endif

static void _press_pwr_btn()
{
	PWR_BTN_DDR |= _BV(PWR_BTN_BIT); // set the pin to output
	PWR_BTN_PORT &= ~_BV(PWR_BTN_BIT); // set the pin to low
	_delay_ms(1000); // keep the button "pressed for 1 sec
	PWR_BTN_DDR &= ~_BV(PWR_BTN_BIT); // disable the ouput
	PWR_BTN_PORT &= ~_BV(PWR_BTN_BIT); // set the pin to tri-state
}

void send_kbcode_for_ir(unsigned long ircode, MK_OR_BK mk_or_bk)
{
	if (bit_is_clear(PWR_SENSE_PIN, PWR_SENSE_BIT))
	{
		if (ircode == MCEH_POWER_ON_1 || ircode == MCEH_POWER_ON_2 || ircode == MCEH_POWER_TOGGLE_1 || ircode == MCEH_POWER_TOGGLE_2)
		{
			_press_pwr_btn();
		}
		else
		{
			return;
		}
	}
    switch (ircode)
    {
        // power buttons
        case MCEH_POWER_OFF_1:
        case MCEH_POWER_OFF_2:
        case MCEH_POWER_TOGGLE_1:
        case MCEH_POWER_TOGGLE_2:
            _press_pwr_btn();
			return;
            
        case MCEH_GUIDE_1:
        case MCEH_GUIDE_2:
			// Kodi: Context Menu = C || ContextMenuButton
	        SEND_CODE(PS2DC_C, mk_or_bk);
			
		case MCEH_INFO_1:
		case MCEH_INFO_2:
			//SEND_EXT_CODE(PS2DC_APPS_EXT, mk_or_bk);
			// Kodi: Info = I
			SEND_CODE(PS2DC_I, mk_or_bk);
			
		case MCEH_LIVE_TV_1:
		case MCEH_LIVE_TV_2:
			// Kodi: OSD = M
			SEND_CODE(PS2DC_M, mk_or_bk);
		
		case MCEH_RED_1:
		case MCEH_RED_2:
		// Kodi: Codec Info = O
			SEND_CODE(PS2DC_O, mk_or_bk);

        case MCEH_PLAY_1:
        case MCEH_PLAY_2:
	        // Kodi: Play = P
		    SEND_CODE(PS2DC_P, mk_or_bk);
			
		case MCEH_SUBTITLE_1:
		case MCEH_SUBTITLE_2:
			// Kodi: Subtitle = T
			SEND_CODE(PS2DC_T, mk_or_bk);
			
		case MCEH_MUTE_1:
		case MCEH_MUTE_2:
			// Kodi: Subtitle Postion Control = Ctrl + T
            SEND_2KEY_CODE(PS2DC_L_CTRL, 0, PS2DC_T, 0, mk_or_bk);
			
		case MCEH_ASPECT_1:
		case MCEH_ASPECT_2:
			// Kodi: Zoom/Aspect Ratio = Z (Y on German Keyb)
			SEND_CODE(PS2DC_Y, mk_or_bk);

        case MCEH_DIR_LEFT_1:
        case MCEH_DIR_LEFT_2:
	        SEND_EXT_CODE(PS2DC_L_ARROW_EXT, mk_or_bk);
			
        case MCEH_DIR_RIGHT_1:
        case MCEH_DIR_RIGHT_2:
			SEND_EXT_CODE(PS2DC_R_ARROW_EXT, mk_or_bk);

        case MCEH_DIR_UP_1:
        case MCEH_DIR_UP_2:
	        SEND_EXT_CODE(PS2DC_U_ARROW_EXT, mk_or_bk);
			
        case MCEH_DIR_DOWN_1:
        case MCEH_DIR_DOWN_2:
		    SEND_EXT_CODE(PS2DC_D_ARROW_EXT, mk_or_bk);

		case MCEH_REWIND_1:
		case MCEH_REWIND_2:
			// Kodi: Variable Seek backwards = Ctrl + Left
			SEND_2KEY_CODE(PS2DC_L_CTRL, 0, PS2DC_L_ARROW_EXT, 1, mk_or_bk);
			
		case MCEH_FAST_FORWARD_1:
		case MCEH_FAST_FORWARD_2:
			// Kodi: Variable Seek forward = Ctrl + Right
			SEND_2KEY_CODE(PS2DC_L_CTRL, 0, PS2DC_R_ARROW_EXT, 1, mk_or_bk);

		case MCEH_PICTURES_1:
		case MCEH_PICTURES_2:
			// Kodi: Move Subtitles Up = Ctrl + Up
			SEND_2KEY_CODE(PS2DC_L_CTRL, 0, PS2DC_U_ARROW_EXT, 1, mk_or_bk);
			
		case MCEH_RADIO_1:
		case MCEH_RADIO_2:
			// Kodi: Move Subtitles Down = Ctrl + Down
			SEND_2KEY_CODE(PS2DC_L_CTRL, 0, PS2DC_D_ARROW_EXT, 1, mk_or_bk);

        case MCEH_CHANNEL_UP_1:
        case MCEH_CHANNEL_UP_2:
	        SEND_EXT_CODE(PS2DC_PG_UP_EXT, mk_or_bk);

        case MCEH_CHANNEL_DOWN_1:
        case MCEH_CHANNEL_DOWN_2:
	        SEND_EXT_CODE(PS2DC_PG_DN_EXT, mk_or_bk);

        case MCEH_OK_1:
        case MCEH_OK_2:
            SEND_CODE(PS2DC_ENTER, mk_or_bk);

        case MCEH_BACK_1:
        case MCEH_BACK_2:
			// Kodi: Back = Backspace
            SEND_CODE(PS2DC_BKSP, mk_or_bk);

		case MCEH_BLUE_1:
		case MCEH_BLUE_2:
			// Kodi: Exit from current App = ESC
			SEND_CODE(PS2DC_ESC, mk_or_bk);

		case MCEH_RECORDED_TV_1:
		case MCEH_RECORDED_TV_2:
			// Kodi: 7 seconds step backwards = ' (quote)
			//SEND_CODE(PS2DC_SINGLE_QUOTE, mk_or_bk);
			SEND_2KEY_CODE(PS2DC_L_SHIFT, 0, PS2DC_BACKSLASH, 0, mk_or_bk);
			
		case MCEH_GREEN_1:
		case MCEH_GREEN_2:
			// Kodi: Full Screen = TAB
			SEND_CODE(PS2DC_TAB, mk_or_bk);
            
        case MCEH_VOL_UP_1:
        case MCEH_VOL_UP_2:
			// Kodi: Step forward 10 min = [
	        //SEND_CODE(PS2DC_SQ_BRACKET_OPEN, mk_or_bk);
			SEND_2KEY_CODE(PS2DC_R_ALT_EXT, 1, PS2DC_8, 0, mk_or_bk);
			
        case MCEH_VOL_DOWN_1:
        case MCEH_VOL_DOWN_2:
			// Kodi: Step forward 10 min = ]
	        //SEND_CODE(PS2DC_SQ_BRACKET_CLOSE, mk_or_bk);
			SEND_2KEY_CODE(PS2DC_R_ALT_EXT, 1, PS2DC_9, 0, mk_or_bk);

        // Play control keys
        case MCEH_PAUSE_1:
        case MCEH_PAUSE_2:
			// Kodi: Play/Pause = Space - we rather use media key here
            SEND_EXT_CODE(PS2DC_MM_PLAY_PAUSE_EXT, mk_or_bk);
			
        case MCEH_STOP_1:
        case MCEH_STOP_2:
            SEND_EXT_CODE(PS2DC_MM_STOP_EXT, mk_or_bk);
			
        case MCEH_SKIP_1:
        case MCEH_SKIP_2:
            SEND_EXT_CODE(PS2DC_MM_FWD_EXT, mk_or_bk);
			
        case MCEH_REPLAY_1:
        case MCEH_REPLAY_2:
            SEND_EXT_CODE(PS2DC_MM_PREV_EXT, mk_or_bk);
			
		case MCEH_TELETEXT_1:
		case MCEH_TELETEXT_2:
			// Kodi: Teletext/Music Visualisation Settings = V
			SEND_CODE(PS2DC_V, mk_or_bk);
			
		case MCEH_RECORD_1:
		case MCEH_RECORD_2:
			// Kodi: Jump to the top of the menu = Home
			SEND_EXT_CODE(PS2DC_HOME_EXT, mk_or_bk);

		// --- non Kodi controls ---
					
		case MCEH_MEDIA_1:
		case MCEH_MEDIA_2:
			// Win + 1 = Starts Kodi
            SEND_2KEY_CODE(PS2DC_R_GUI_EXT, 1, PS2DC_1, 0, mk_or_bk);
			

		case MCEH_MUSIC_1:
		case MCEH_MUSIC_2:
			// Win + 2 = Starts SkyGo
            SEND_2KEY_CODE(PS2DC_R_GUI_EXT, 1, PS2DC_2, 0, mk_or_bk);

			
		case MCEH_TV_1:
		case MCEH_TV_2:
			// Win + 3
            SEND_2KEY_CODE(PS2DC_R_GUI_EXT, 1, PS2DC_3, 0, mk_or_bk);
			
		case MCEH_MOVIES_1:
		case MCEH_MOVIES_2:
			// Win + 4
            SEND_2KEY_CODE(PS2DC_R_GUI_EXT, 1, PS2DC_4, 0, mk_or_bk);
			
		case MCEH_YELLOW_1:
		case MCEH_YELLOW_2:
			// Win
			SEND_EXT_CODE(PS2DC_R_GUI_EXT, mk_or_bk);
			
        default:
            return;
    }
}
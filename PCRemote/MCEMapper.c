/*
 * MCEMapper.c
 *
 * Created: 17.11.2015 08:02:52
 *  Author: Bernd
 */ 

#include "MCEMapper.h"
#include "MCEMapperHelper.h"
#include "MCECodes.h"
#include "../PS2KBDevice/PS2KBDevice.h"
#include "../PS2KBDevice/PS2DevCodes.h"

void send_kbcode_for_ir(unsigned long ircode, MK_OR_BK mk_or_bk)
{
    switch (ircode)
    {
        // power buttons
        case MCEH_POWER_ON_1:
        case MCEH_POWER_ON_2:
            SEND_EXT_CODE(PS2DC_PWR_WAKE_EXT, mk_or_bk);
        case MCEH_POWER_OFF_1:
        case MCEH_POWER_OFF_2:
            SEND_EXT_CODE(PS2DC_PWR_SLEEP_EXT, mk_or_bk);
        case MCEH_POWER_TOGGLE_1:
        case MCEH_POWER_TOGGLE_2:
            SEND_EXT_CODE(PS2DC_PWR_POWER_EXT, mk_or_bk);
            
        // cursor keys
        case MCEH_DIR_UP_1:
        case MCEH_DIR_UP_2:
            SEND_EXT_CODE(PS2DC_U_ARROW_EXT, mk_or_bk);
        case MCEH_DIR_DOWN_1:
        case MCEH_DIR_DOWN_2:
            SEND_EXT_CODE(PS2DC_D_ARROW_EXT, mk_or_bk);
        case MCEH_DIR_LEFT_1:
        case MCEH_DIR_LEFT_2:
            SEND_EXT_CODE(PS2DC_L_ARROW_EXT, mk_or_bk);
        case MCEH_DIR_RIGHT_1:
        case MCEH_DIR_RIGHT_2:
            SEND_EXT_CODE(PS2DC_R_ARROW_EXT, mk_or_bk);
        case MCEH_OK_1:
        case MCEH_OK_2:
            SEND_CODE(PS2DC_ENTER, mk_or_bk);
        case MCEH_BACK_1:
        case MCEH_BACK_2:
            SEND_CODE(PS2DC_ESC, mk_or_bk);
            
        // Play control keys
        case MCEH_PLAY_1:
        case MCEH_PLAY_2:
        case MCEH_PAUSE_1:
        case MCEH_PAUSE_2:
            SEND_EXT_CODE(PS2DC_MM_PLAY_PAUSE_EXT, mk_or_bk);
        case MCEH_STOP_1:
        case MCEH_STOP_2:
            SEND_EXT_CODE(PS2DC_MM_STOP_EXT, mk_or_bk);
        case MCEH_FAST_FORWARD_1:
        case MCEH_FAST_FORWARD_2:
            SEND_EXT_CODE(PS2DC_MM_FWD_EXT, mk_or_bk);
        case MCEH_REPLAY_1:
        case MCEH_REPLAY_2:
            SEND_EXT_CODE(PS2DC_MM_PREV_EXT, mk_or_bk);
        case MCEH_SKIP_1:
        case MCEH_SKIP_2:
            SEND_EXT_CODE(PS2DC_PG_UP_EXT, mk_or_bk); // we send PageUp here
        case MCEH_REWIND_1:
        case MCEH_REWIND_2:
            SEND_EXT_CODE(PS2DC_PG_DN_EXT, mk_or_bk); // we send PageDown here
        case MCEH_CHANNEL_UP_1:
        case MCEH_CHANNEL_UP_2:
            SEND_EXT_CODE(PS2DC_PG_UP_EXT, mk_or_bk);
        case MCEH_CHANNEL_DOWN_1:
        case MCEH_CHANNEL_DOWN_2:
            SEND_EXT_CODE(PS2DC_PG_DN_EXT, mk_or_bk);
            
        // volume keys
        case MCEH_VOL_UP_1:
        case MCEH_VOL_UP_2:
            SEND_EXT_CODE(PS2DC_MM_VOL_UP_EXT, mk_or_bk);
        case MCEH_VOL_DOWN_1:
        case MCEH_VOL_DOWN_2:
            SEND_EXT_CODE(PS2DC_MM_VOL_DOWN_EXT, mk_or_bk);
        // special mappings
        case MCEH_GUIDE_1:
        case MCEH_GUIDE_2:
            SEND_2KEY_CODE(PS2DC_R_GUI_EXT, 1, PS2DC_1, 0, mk_or_bk);

        // F2 key to be able to enter BIOS Setup
        case MCEH_RED_1:
        case MCEH_RED_2:
            SEND_CODE(PS2DC_F2, mk_or_bk);
        default:
            return;
    }
}
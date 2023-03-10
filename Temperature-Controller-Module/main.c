/************************************************************************/
/*                          Mode Manager                                */
/************************************************************************/

#include "Managers/Schedular.h"
#include "Managers/TMP_Manager.h"
#include "Managers/DSP_Manager.h"
#include "Managers/Heater_Control.h"

extern uint8_t CRT_Temperature;
extern uint8_t SET_Temperature;

extern char *states[4];
extern uint8_t state_indx;

extern float64_t Vr;
extern float64_t Vt;

uint8_t LCD_INITIALIZED = 0U;
uint8_t LCD_FINISH_SHIFT = 0U;

extern uint8_t animation_loops;

uint8_t main(void);

uint8_t main(void)
{
    /* Managers Initializations */
    Schedular_vInit();

	/* Use Timer Scheduler instead of _delay_ms(30) in the LCD */
	while (LCD_INITIALIZED == 0U)
	{
		WelcomeScreen();
	}
	
	/* Stay if this loop till the animation_loops reaches 0 */
    while (animation_loops != 0U)
	{
		ShiftDisplay();
    }

    IdleScreen();
    TMP_Manager_vInit();
    LEDs_States_vInit();

    while (1)
	{
        Display_SET_Temperature(SET_Temperature);
        Display_CRT_Temperature(CRT_Temperature);
        Display_STATE(states[state_indx]);

        /* Check if you are in STANDBY STATE:
         * Update SET_Temperature Value From The User
         * Check if User Pressed '#' To Switch to OPERATION STATE
         */
        if (state_indx == STANDBY_INDEX)
		{
            /* PWM OFF */
            Heater_vDisable();

            Update_SET_Temperature();
            Display_SET_Temperature(SET_Temperature);
            Check_HASH_Key();
        }

        /* Check if you are in OPERATION STATE */
        if (state_indx == OPERATION_INDEX)
		{
            /* Start Heater */
            Heater_vEnable();

            Update_CRT_Temperature();
            Update_Vt();
            Update_Vr();

            /* Update PWM Wave */
            Heater_vSet_Volt(Vt, Vr);

            Check_HASH_Key();
            Check_NORMAL_State();
            Check_ERROR_State();

            /* This function should handle Error 3 min */
            Check_ERROR_State_Timer();
        }

        /* Check if you are in NORMAL STATE */
        if (state_indx == NORMAL_INDEX)
		{
            /* PWM OFF */
            Heater_vDisable();

            Update_CRT_Temperature();

            Check_HASH_Key();
            Check_OPERATION_State();
            Check_ERROR_State();
        }

        /* Check if you are in ERROR STATE */
        if (state_indx == ERROR_INDEX)
		{
            /* PWM and ADC OFF */
            Heater_vDisable();

            /* Must POWER OFF */
        }
    }
}

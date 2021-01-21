/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
#define LED_ON          LED_Set
#define LED_OFF         LED_Clear

enum
{
  IDLE_MODE = 'a',
  SLEEP_MODE = 'b',
}LOW_POWER_MODES;

uint8_t cmd = 0;

void timeout (uint32_t status, uintptr_t context)
{
    LED_Toggle();    
}

void display_menu (void)
{
    printf("\n\n\n\rSelect the low power mode to enter");
    printf("\n\ra) Idle Mode");
    printf("\n\rb) Sleep Mode"); 
    printf("\n\rEnter your choice");    
    scanf("%c", &cmd);
}
int main ( void )
{   
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    printf("\n\n\r----------------------------------------------");
    printf("\n\r                 LOW power demo"               );
    printf("\n\r----------------------------------------------"); 
    
    TMR1_CallbackRegister(&timeout, (uintptr_t) NULL);
    TMR1_Start();

    display_menu();
    
    while(1)
    {
        switch(cmd)
        {
            case IDLE_MODE:
            {
                printf("\n\rEntering IDLE Mode");
                printf("\n\rPress Switch SW1 to wakeup the device");
                GPIO_PinInterruptEnable(SWITCH_PIN);
                TMR1_Stop();
                POWER_LowPowerModeEnter(LOW_POWER_IDLE_MODE);
                printf("\n\rSW1 Pressed, exiting IDLE mode......");
                SWITCH_InterruptDisable();
                TMR1_Start();
                display_menu();
                break;
            }
            case SLEEP_MODE:
            {
                printf("\n\rEntering SLEEP Mode");
                printf("\n\rPress Switch SW1 to wakeup the device         ");
                GPIO_PinInterruptEnable(SWITCH_PIN);
                TMR1_Stop();
                POWER_LowPowerModeEnter(LOW_POWER_SLEEP_MODE);
                printf("\n\rSW1 Pressed, exiting SLEEP mode......");
                SWITCH_InterruptDisable();
                TMR1_Start();
                display_menu();
                break;
            }
            default:
            {
                printf("\n\rInvalid choice");
                display_menu();
                break;
            }
        } 
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/


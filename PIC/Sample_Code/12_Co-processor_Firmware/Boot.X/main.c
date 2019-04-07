/**
 * @file
 * @author Tyler Gamvrelis
 *
 * Created on August 30, 2017, 11:34 PM
 * 
 * @defgroup Co-processor
 * @ingroup Demo
 * @brief Co-processor firmware with startup boot options
 */

#include <xc.h>
#include "configBits.h"
#include "machineConfig.h"
#include "Keypad.h"
#include "UART_PIC.h"
#include "../USBConverter/LPC_FSUSB-CDC-X/main_cdc.h"
// PLACE USER-DEFINED INCLUDES HERE

typedef enum{
    KEYPAD_ENCODER_AND_COM_PORT, // Key 1
    KEYPAD_ENCODER_UART,         // Key 2
    USER_DEFINED_CODE,           // Key 3
    NUM_PROGRAMS
}programs_e;

void bootKeypadPORTB(void);
void bootKeypadAndComPort(void);
void bootKeypadUART(void);
void bootUserCode(void); // USER-DEFINED CODE GOES IN THIS FUNCTION

void main(void) {
    // Configure pins, SFRs, and on-board modules
    machineConfig();
    
    __delay_ms(10); // Just in case
    
    signed char boot = -1; /* Store which program to boot into. */
    
    // Poll the keyboard for a key press
    unsigned char data = readKeyboard();

    // Check if a key press is detected
    if (data != 0xF0){

        // Debounce button press. If the same key is read a second time,
        // after 20 milliseconds and it is the same as before, it is assumed
        // valid
        __delay_ms(20);
        if (data == readKeyboard()){
            boot = data;
            
            // Do nothing while key is still pressed
            while (data == readKeyboard()){
                continue;
            }
        }
    }
    
    // Boot into selected program
    switch(boot){
        case KEYPAD_ENCODER_AND_COM_PORT:
            bootKeypadAndComPort();
            break;
        case KEYPAD_ENCODER_UART:
            bootKeypadUART();
            break;
        case USER_DEFINED_CODE:
            bootUserCode();
            break;
        default:
            bootKeypadPORTB();
            break;
    }
}

/**
 * @brief Calls the keypad routine that latches data on the main PIC's port B
 *        lines, forever
 */
void bootKeypadPORTB(void){
    while(1){
        keypadRoutine();
    }
}

/**
 * @brief Calls the keypad routine that latches data on the main PIC's port B
 *        lines as well as the USB to serial converter routine, forever
 */
void bootKeypadAndComPort(void){
    ei(); // Enable interrupts
   
    SYSTEM_Initialize(SYSTEM_STATE_USB_START);

    USBDeviceInit();
    USBDeviceAttach();
    
    keypadFlags.f_disabled_to_enabled = 1;
    keypadFlags.f_enabled_to_disabled = 1;
    keypadFlags.sending_f_first = 1;
    
    while(1){
        // These tasks share the CPU
        CDC_Routine();
        keypadRoutine_StateBased();
    }
}

/**
 * @brief Calls the keypad routine that transmits key press data via UART,
 *        forever
 */
void bootKeypadUART(void){
    // Initialize EUSART module for asynchronous 9600/8N1
    UART_Init(9600);
    __UART_ENABLE_TX(); // Enable transmitter
    
    // Disable the data bus and data available pin by setting them to high
    // impedance (input). This way, RB1, RB7:4 on the primary PIC can be used
    // as GPIO
    TRISA = 0b11101111;
    
    LATAbits.LATA4 = 1; // Set RA4 output high (i.e. set KPD)
    
    while(1){
        keypadRoutineUART();
    }
}

/** @brief Calls user defined code, forever */
void bootUserCode(void){
    while(1){
        // YOUR CODE HERE
    }
}
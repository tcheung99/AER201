/**
 * @file
 * @author Tyler Gamvrelis
 * 
 * Created on October 1, 2017, 2:28 PM
 * 
 * @defgroup BoardTest
 * @brief Tests the basic capabilities of most of the development board modules
 */

#include <xc.h>
#include <configBits.h>
#include "GLCD_PIC.h"
#include "I2C.h"
#include "lcd.h"
#include "SD_PIC.h"

typedef enum{
    SETRTC = 0, // (key 1)
    GETRTC,     // (key 2)
    GLCD,       // (key 3)
    SDCARD,     // (key 4)
    IO,         // (key 5)
    UARTSEND,   // (key 6)
    A2D,        // (key 7)
    NUM_STATES
}program_states_e;

// Maps keypresses to program states. An alternative to this is to use the
// keypress as an index into a table of function pointers, which is called a
// jump table
const program_states_e key_map[] = {
    SETRTC,     GETRTC,     GLCD,       NUM_STATES,
    SDCARD,     IO,         UARTSEND,   NUM_STATES,
    A2D,        NUM_STATES, NUM_STATES, NUM_STATES,
    NUM_STATES, NUM_STATES, NUM_STATES, NUM_STATES
};

void testRTCSet(void);
void testRTCGet(void);
void testGLCD(void);
void testSDCard(void);
void testIO(void);
void testUART(void);
void testA2D(void);

/**
 * @brief Blocks program execution until a key is pressed and released
 * @return Keypress value read from PORTB
 */
unsigned char getKey(void){   
    // While no key is pressed, don't do anything
    while(PORTBbits.RB1 == 0){
        continue;
    }
    
    unsigned char key = (PORTB & 0xF0) >> 4;
    
    // While key is being pressed, don't do anything
    while(PORTBbits.RB1 == 1){
        continue;
    }
    
    return key;
}

void main(void){
    // Configure pins, SFRs, and on-board modules
    // <editor-fold defaultstate="collapsed" desc="Machine Configuration">    
    /********************************* PIN I/O ********************************/
    /* Write outputs to LATx, read inputs from PORTx. Here, all latches (LATx)
     * are being cleared (set low) to ensure a controlled start-up state. */  
    LATA = 0x00;
    LATB = 0x00; 
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;

    /* After the states of LATx are known, the data direction registers, TRISx
     * are configured. 0 --> output; 1 --> input. Default is  1. */
    TRISA = 0b11111111; // All inputs
    TRISB = 0b11110010; // RB1, RB4, RB5, RB6, RB7 as inputs (for keypad)
    TRISC = 0b10000000; /* RC3 is SCK/SCL (SPI/I2C),
                         * RC4 is SDI/SDA (SPI/I2C),
                         * RC5 is SDO (SPI),
                         * RC6 and RC7 are UART TX and RX, respectively. */
    TRISD = 0b00000001; /* RD0 is the GLCD chip select (tri-stated so that it's
                         * pulled up by default),
                         * RD1 is the GLCD register select,
                         * RD2 is the character LCD RS,
                         * RD3 is the character LCD enable (E),
                         * RD4-RD7 are character LCD data lines. */
    TRISE = 0b00000100; /* RE2 is the SD card chip select (tri-stated so that
                         * it's pulled up by default).
                         * Note that the upper 4 bits of TRISE are special 
                         * control registers. Do not write to them unless you 
                         * read §9.6 in the datasheet */

    /************************** A/D Converter Module **************************/
    ADCON0 = 0x00;       // Disable ADC
    ADCON1 = 0x0F;       // Set all A/D ports to digital (pg. 222)
    ADCON2bits.ADFM = 1; // Right justify A/D result

    CVRCON = 0x00;       // Disable comparator voltage reference (pg. 239)
    CMCONbits.CIS = 0;   // Connect C1 Vin and C2 Vin to RA0 and RA1 (pg. 233)
    ADCON2 = 0b10110001; // Right justify A/D result, 16TAD, FOSC/8 clock
    
    /****************************** USART Module ******************************/
    long baudRate = 9600;
    SPBRG = (unsigned char)((_XTAL_FREQ / (64 * baudRate)) - 1);
    TXSTAbits.TX9 = 0;  // Use 8-bit transmission (8 data bits, no parity bit)
    TXSTAbits.SYNC = 0; // Asynchronous communication
    TXSTAbits.TXEN = 1; // Enable transmitter
    TRISCbits.TRISC6 = 0;         // TX = output
    RCSTAbits.SPEN = 1; // Enable serial peripheral
    //</editor-fold>
    
    // Test character LCD hardware by basic print function
    initLCD();
    lcd_clear();
    printf("BOARD TEST");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Press a key");
    
    program_states_e state;
    while(1){
        // Blocks program execution until a key is pressed
        state = key_map[getKey()];
        
        // Change program state based on user input
        switch(state){
            case SETRTC:    //  key 1
                testRTCSet();
                break;
            case GETRTC:    //  key 2
                testRTCGet();
                break;
            case GLCD:      //  key 3
                testGLCD();
                break;
            case SDCARD:    //  key 4
                testSDCard();
                break;
            case IO:        //  key 5
                testIO();
                break;
            case UARTSEND:  //  key 6
                testUART();
                break;
            case A2D:       //  key 7
                testA2D();
                break;
            default:
                break;      // Key that was pressed is not mapped to a test
        }
        
        if(state != NUM_STATES){
            // Print the main message if we just returned from one of the tests
            lcd_clear();
            printf("BOARD TEST");
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            printf("Press a key");
        }
    }
}

/** @brief Sets the time on the RTC to 15 seconds before the new year */
void testRTCSet(void){
    // Since this is static, it is only initialized upon the first call to this
    // function
    static const char happynewyear[7] = {
        0x45, // 45 Seconds 
        0x59, // 59 Minutes
        0x23, // 24 hour mode, set to 23:00
        0x00, // Sunday
        0x31, // 31st
        0x12, // December
        0x18  // 2018
    };
        
    I2C_Master_Init(100000); //Initialize I2C Master with 100 kHz clock
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); //7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds

    for(unsigned char i = 0; i < 7; i++){
        I2C_Master_Write(happynewyear[i]);
    }

    I2C_Master_Stop(); //Stop condition
}

/**
 * @Brief Gets the current time from the RTC and displays the results on the
 *        character LCD. This function incorporates an loop that updates the
 *        time once per second. The loop can be exited by pressing any key on
 *        the keypad
 */
void testRTCGet(void){
    I2C_Master_Init(100000); //Initialize I2C Master with 100 kHz clock

    unsigned char time[7];
    while(1){
        I2C_Master_Start(); // Start condition
        I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
        I2C_Master_Write(0x00); // Set memory pointer to seconds
        I2C_Master_Stop(); // Stop condition

        // Read current time
        I2C_Master_Start(); // Start condition
        I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
        for(unsigned char i = 0; i < 6; i++){
            time[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
        }
        time[6] = I2C_Master_Read(NACK); // Final Read with NACK
        I2C_Master_Stop(); // Stop condition

        // Print received data to LCD
        lcd_clear();
        printf("%02x/%02x/%02x", time[6],time[5],time[4]); // Print date in YY/MM/DD
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("%02x:%02x:%02x", time[2],time[1],time[0]); // HH:MM:SS
        
        // Check for key press while delaying for about 1 second
        for(int i = 0; i < 100; i++){
            __delay_ms(10);
            if(PORTBbits.RB1 == 1){
                // If a key is pressed, wait until it is released to avoid
                // accidentally entering a different test
                while(PORTBbits.RB1 == 1){
                    continue;
                }
                return;
            }
        }
    }
}

/** @brief Initializes the GLCD and draws two rectangles on it */
void testGLCD(void){
    initGLCD(); // Initialize the GLCD
    glcdDrawRectangle(0, 64, 0, 128, RED); // Left half of screen is red
    glcdDrawRectangle(64, 128, 0, 128, VIOLET); // Right half of screen is violet
}

/**
 * @brief Initializes the SD card and displays the storage capacity on the
 *        character LCD as a double. To exit this function, press any key on the
 *        keypad
 * @note If there is no SD card inserted, this function will never finish
 */
void testSDCard(void){
    initSD(); // Initialize SD card
    lcd_clear();
    printf("SD Card success!");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Size: %0.5f", SDCard.size);
    __delay_ms(500);
    
    while(PORTBbits.RB1 == 0){
        continue;
    }
    
    // If a key is pressed, wait until it is released to avoid
    // accidentally entering a different test back in the main loop
    while(PORTBbits.RB1 == 1){
        continue;
    }
    
    return;
}

#define DELAY_TIME 500
/**
 * @brief This function is identical in functionality to the PortTest sample
 *        code. The pin iteration runs in an infinite loop that can only be
 *        exited by resetting the PIC
 */
void testIO(void){
    // First, we need to make sure the PIC is configured correctly. Usage of the
    // on-board modules will sometimes force pins to be in a certain state by
    // default, which is undesirable for this test
    
    // Disable the serial port (if enabled, we cannot control the USART pins
    // using the TRISx & LATx registers)
    TXSTAbits.TXEN = 0;
    RCSTAbits.SPEN = 0;
    
    // Next, we disable the MSSP module, which incorporates the SPI interface
    // used by the SD card and GLCD, as well as the I2C interface used by the
    // RTC. Note that the MSSP module only operates one of these interfaces
    // (SPI, I2C) at any given time
    mssp_disable();
    
    // Finally, we change our I/O settings
    LATA = 0x00;
    LATB = 0x00; 
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    unsigned char i = 0;
    unsigned char temp = 0x00;
    while(1){
        temp = 1 << 7; // Set temporary byte to 0b10000000
        // Write temp to the latch, then bit shift to right. Iterate over the
        // port size
        for (i = 0; i < 8; i++){
            LATA = temp;
            temp >>= 1;
            __delay_ms(DELAY_TIME);
        }
        LATA = 0x00; // Clear latch outputs after demonstrating the port.

        // Same process for all other ports
        temp = 1 << 7;
        for (i = 0; i < 8; i++){
            LATB = temp;
            temp >>= 1;
            __delay_ms(DELAY_TIME);
        }
        LATB = 0x00;

        temp = 1 << 7;
        for (i = 0; i < 8; i++){
            LATC = temp;
            temp >>= 1;
            __delay_ms(DELAY_TIME);
        }
        LATC = 0x00;

        temp = 1 << 7;
        for (i = 0; i < 8; i++){
            LATD = temp;
            temp >>= 1;
            __delay_ms(DELAY_TIME);
        }
        LATD = 0x00;

        // Note: Look in the SD card module to see the LED for RE2
        temp = 1 << 2;
        for (i = 0; i < 3; i++){
            LATE = temp;
            temp >>= 1;
            __delay_ms(DELAY_TIME);
        }
        LATE = 0x00;
    }
}

/**
 * @brief Sends "Hello world!" via the UART bus, which can be received by a PC
 *        connected to the USB port in the USB module
 */
void testUART(void){
    const char msg[] = "Hello world!"; // Create a new array of characters
    
    // Transmit
    for(unsigned char i = 0; i < sizeof(msg); i++){
        while(!TXIF | !TRMT){
            continue;
        }
        
        TXREG =  msg[i];
    }
}

/**
 * @brief Reads the analog input from the specified analog channel
 * @param channel The numebr of the channel to read
 * @return 10-bit value corresponding to the voltage sampled from the channel
 */
unsigned short readADC(char channel){
    ADCON0 = (channel & 0x0F) << 2; // Select ADC channel (i.e. pin)
    ADON = 1; // Enable module
    ADCON0bits.GO = 1; // Initiate sampling
    while(ADCON0bits.GO_NOT_DONE){
        continue; // Poll for acquisition completion
    }
    return (ADRESH << 8) | ADRESL; // Return result as a 16-bit value
}

/**
 * @brief Acquires analog data from RA2 and RA3 and displays the results on the 
 *        character LCD in hexadecimal format. This function incorporates an 
 *        loop that updates the readings 10 times per second. The loop can
 *        be exited by pressing any key on the keypad
 */
void testA2D(void){
    ADCON1 = 0x0B; // RA0-3 are analog (pg. 222)
    while(1){
        lcd_clear();
        printf("RA2: %.3x", readADC(2));
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("RA3: %.3x", readADC(3));
        __delay_ms(100);
        
        // Exit on key press
        if(PORTBbits.RB1 == 1){
            // If a key is pressed, wait until it is released to avoid
            // accidentally entering a different test
            while(PORTBbits.RB1 == 1){
                continue;
            }
            ADCON1 = 0x0F; // All inputs are digital
            return;
        }
    }
}
/**
 * @file
 * @author Tyler Gamvrelis
 * 
 * Date: August 29, 2017 4:51 PM
 * 
 * @defgroup Arduino_UART_PIC
 * @ingroup Demo
 * @brief Sample program for arbitrary information exchange between the main PIC
 *        and the Arduino Nano via UART
 * 
 * Preconditions:
 * @pre The Co-processor is not driving lines on the UART bus (e.g. the JP_TX
 *      and JP_RX jumpers are removed)
 * @pre The character LCD is in an Arduino Nano socket
 * @pre PIC-Arduino link switches are enabled (ON) for D1 of the Arduino (the RX
 *      pin). However, make sure that neither of D0 and D1 are enabled (ON) 
 *      while programming the Arduino Nano
 */

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

// Read-only
const byte rxPin = 1;
const byte txPin = 0;
const int rs = 4, en = 5, d4 = 6, d5 = 7, d6 = 8, d7 = 9;

// Set up objects
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
    // Set up the LCD's number of columns and rows
    lcd.begin(16, 4);
    
    // Configure pin modes for tx and rx
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    
    // Open software serial port with baud rate = 9600
    mySerial.begin(9600);
}

void loop() {
    // Wait to receive the message from the main PIC
    if(mySerial.available() > 0){
        // Write received byte to the LCD using ASCII encoding.
        // The received byte can also be seen on your PC under
        // Tools -> Serial Monitor (make sure your rate is 9600)
        unsigned char my_byte = mySerial.read();
        lcd.print((char)my_byte);
    }
}

/** 
 * @file
 * @author Michael Ding
 * @author Tyler Gamvrelis
 * 
 * @defgroup Arduino_I2C_PIC
 * @ingroup Demo
 * @brief Demonstration of Arduino-PIC interfacing via I2C. In this program,
 *        the PIC outputs keypad data to the Arduino, which forwards it to a
 *        PC. If a triple-A sequence is given on the keypad, then the PIC will
 *        display data entered into the serial monitor on the character LCD. To
 *        open the serial monitor, go to Tools -> Serial Monitor
 * 
 * Preconditions:
 * @pre PIC-Arduino link switches are enabled (ON) for A4 and A5 of the Arduino
 *      Nano (RC4 and RC3 for the primary PIC, respectively)
 * @pre PIC and Arduino Nano agree on the Arduino Nano's slave address (i.e. the
 *      same slave address is used in software)
 * @pre A PC is connected if the Arduino Nano serial monitor is to be used. Note that
 *      the serial monitor baud rate must be 9600 for this program
 */

#include <Wire.h>

void setup(){
    Wire.begin(8); // Join I2C bus with address 8
  
    // Register callback functions
    Wire.onReceive(receiveEvent); // Called when this slave device receives a data transmission from master
    Wire.onRequest(requestEvent); // Called when master requests data from this slave device
  
    // Open serial port to PC (hardware UART)
    Serial.begin(9600);      
}

volatile bool send_to_pic = false;
volatile uint8_t incomingByte;
volatile uint8_t incomingByte2;

void loop(){
    // If we should send to the PIC, then we wait to receive a byte from the PC
//    if (send_to_pic && Serial.available() > 0 && !incomingByte) {
        incomingByte = 5;
        incomingByte2 = 10;
//    }
}

/** @brief Callback for when the master transmits data */
void receiveEvent(void){
    static uint8_t buf[3] = {0};
    static uint8_t counter = 0;
    
    uint8_t x = Wire.read(); // Receive byte
    Serial.println((char)x); // Print to serial output as char (ASCII representation)
    
    buf[counter++] = x;
    counter = (counter == 3) ? 0 : counter;
    
    if(buf[0]=='A' && buf[1]=='A' && buf[2]=='A'){
        send_to_pic = true;
    }
}

/** @brief Callback for when the master requests data */
void requestEvent(void){
    Wire.write(incomingByte); // Respond with message of 1 byte
    Wire.write(incomingByte2); // Respond with message of 1 byte
    incomingByte = 0; // Clear output buffer
    incomingByte2 = 0;
}

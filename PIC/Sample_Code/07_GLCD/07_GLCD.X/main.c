/**
 * @file
 * @author Tyler Gamvrelis
 * 
 * Created on August 13, 2017, 6:00 PM
 * 
 * @defgroup GLCD_Demo
 * @brief Draws rectangles, pixels, and patterns on the GLCD
 * 
 * Preconditions:
 * @pre GLCD is in a PIC socket
 * @pre SPI/I2C bus is not in use
 */

#include <xc.h>
#include <configBits.h>
#include "GLCD_PIC.h"

void main(void) {
    // Note: this pin config is done inside initGLCD, it's shown here explicitly
    // for learning
    CS_GLCD = 1; // Deselect GLCD
    RS_GLCD = 1; // Set RS high
    TRIS_CS_GLCD = 0; // Set CS data direction to output
    TRIS_RS_GLCD = 0; // Set RS data direction to output
    
    // Initialize GLCD
    initGLCD();
    
    // Main loop
    unsigned char iteration = 0, x = 0, y = 0;
    while(1){
        // Fill screen with red
        glcdDrawRectangle(0, GLCD_SIZE_HORZ, 0, GLCD_SIZE_VERT, RED);
        __delay_ms(1000);
        
        // Fill screen with rainbow colors
        glcdDrawRectangle(0, 18, 0, GLCD_SIZE_VERT, RED);
        glcdDrawRectangle(18, 36, 0, GLCD_SIZE_VERT, ORANGE);
        glcdDrawRectangle(36, 54, 0, GLCD_SIZE_VERT, YELLOW);
        glcdDrawRectangle(54, 72, 0, GLCD_SIZE_VERT, GREEN);
        glcdDrawRectangle(72, 90, 0, GLCD_SIZE_VERT, BLUE);
        glcdDrawRectangle(90, 108, 0, GLCD_SIZE_VERT, INDIGO);
        glcdDrawRectangle(108, 128, 0, GLCD_SIZE_VERT, VIOLET);
        
        // Demonstrate inversion
        __delay_ms(500);
        glcd_invon();
        __delay_ms(500);
        glcd_invoff();
        __delay_ms(500);
        
        // Draw white in the four corner pixels
        glcdDrawPixel(0, 0, WHITE);
        __delay_ms(500);
        glcdDrawPixel(GLCD_SIZE_HORZ, 0, WHITE);
        __delay_ms(500);
        glcdDrawPixel(GLCD_SIZE_HORZ, GLCD_SIZE_VERT, WHITE);
        __delay_ms(500);
        glcdDrawPixel(0, GLCD_SIZE_VERT, WHITE);
        __delay_ms(500);
        
        // Place 128 pixels in accordance with some arbitrarily-chosen math
        for(x = 0; x < GLCD_SIZE_HORZ; x++){
            if(x % (GLCD_SIZE_VERT/16) == 0){
                glcdDrawPixel(GLCD_SIZE_VERT - (x * 8), (x % 3) * 33, x * 2048);
            }
            else{
                glcdDrawPixel(x, GLCD_SIZE_VERT - x, x * 2048);
            }
            __delay_ms(10);
        }
        
        // Fill screen with blue/pink pattern, pixel-by-pixel
        for(y = 0; y < GLCD_SIZE_VERT; y++){
            for(x = 0; x < GLCD_SIZE_HORZ; x++){
                glcdDrawPixel(x, y, x*y*16);
            }
        }
        
        // Fill screen with blue pattern, pixel-by-pixel
        for(y = 0; y < GLCD_SIZE_VERT; y++){
            for(x = 0; x < GLCD_SIZE_HORZ; x++){
                glcdDrawPixel(x, y, (x+8)*(y+8));
            }
        }
        
        // Fill screen with green, pixel-by-pixel. Note the difference in speed
        // when using this method and when using the first method 
        // (glcdDrawRectangle). The first method is faster because we only have
        // to define the drawing window once, as the entire screen. The method
        // below re-defines the drawing window once per iteration, so
        // 128 * 128 = 16, 384 times. Thus, it is slower for filling the screen
        for(y = 0; y < GLCD_SIZE_VERT; y++){
            for(x = 0; x < GLCD_SIZE_HORZ; x++){
                glcdDrawPixel(x, y, GREEN);
            }
        }
        
        // Fill screen with black, and then demonstrate the pixel-by-pixel
        // addressing by drawing 5 lines slowly in white. The pixel-by-pixel
        // drawing is slower here intentionally so that you can see that each
        // pixel is indeed being addressed separately
        glcdDrawRectangle(0, GLCD_SIZE_HORZ, 0, GLCD_SIZE_VERT, BLACK);
        for(y = 0; y < 5; y++){
            for(x = 0; x < GLCD_SIZE_HORZ; x++){
                glcdDrawPixel(x, y, WHITE);
                __delay_ms(5);
            }
        }
        
        // Rotate display at the end of each iteration
        switch(iteration % 4){
            case 0:
                glcdSetOrigin(ORIGIN_TOP_RIGHT);
                break;
            case 1:
                glcdSetOrigin(ORIGIN_BOTTOM_RIGHT);
                break;
            case 2:
                glcdSetOrigin(ORIGIN_BOTTOM_LEFT);
                break;
            case 3:
                glcdSetOrigin(ORIGIN_TOP_LEFT);
                break;
        }
        iteration++;
    }
}
/**
 * @file
 * @author Tyler Gamvrelis
 *
 * Created on August 13, 2017, 6:00 PM
 *
 * @ingroup GLCD
 */

/********************************* Includes **********************************/
#include "GLCD_PIC.h"
#include "SPI_PIC.h"

/******************************** Constants **********************************/
const unsigned char GLCD_ADDRESSABLE_SIZE_HORZ = 128;
const unsigned char GLCD_ADDRESSABLE_SIZE_VERT = 160;

const unsigned char GLCD_SIZE_HORZ = 128;
const unsigned char GLCD_SIZE_VERT = 128;

const unsigned long BLACK = 0x000000;
const unsigned long GREY = 0x808080;
const unsigned long WHITE = 0xFFFFFF;
const unsigned long RED = 0xFF0000;
const unsigned long ORANGE = 0xFF8C00;
const unsigned long YELLOW = 0xFFFF00;
const unsigned long GREEN = 0x00FF00;
const unsigned long BLUE = 0x0000FF;
const unsigned long INDIGO = 0x4B0082;
const unsigned long VIOLET = 0x9400D3;

// Display command codes (write only, since we don't have hardware to read).
// These are static which makes them only visible to this compilation unit,
// effectively hiding them from the rest of the application
static const unsigned char INST_NOP = 0x00;      /**< Empty processor cycle */
static const unsigned char INST_SWRESET = 0x01;  /**< All registers to default state */
static const unsigned char INST_SLPIN = 0x10;    /**< Enter sleep mode */
static const unsigned char INST_SLPOUT = 0x11;   /**< Exit sleep mode */
static const unsigned char INST_PTLON = 0x12;    /**< Partial mode on */
static const unsigned char INST_NORON = 0x13;    /**< Partial mode off (normal) */
static const unsigned char INST_INVOFF = 0x20;   /**< Display inversion off */
static const unsigned char INST_INVON = 0x21;    /**< Display inversion on */
static const unsigned char INST_GAMSET = 0x26;   /**< Set gamma */
static const unsigned char INST_DISPOFF = 0x28;  /**< Turn off display */
static const unsigned char INST_DISPON = 0x29;   /**< Turn on display */
static const unsigned char INST_CASET = 0x2A;    /**< Set column address */
static const unsigned char INST_RASET = 0x2B;    /**< Set row address */
static const unsigned char INST_RAMWR = 0x2C;    /**< Enables RAM writes */
static const unsigned char INST_PTLAR = 0x30;    /**< Partial start/end address */
static const unsigned char INST_TEOFF = 0x34;    /**< Tearing effect off */
static const unsigned char INST_TEON = 0x35;     /**< Tearing effect on */
static const unsigned char INST_MADCTL  = 0x36;  /**< Memory data access control */
static const unsigned char INST_IDMOFF = 0x38;   /**< Idle mode off */
static const unsigned char INST_IDMON = 0x39;    /**< Idle mode on */
static const unsigned char INST_COLMOD = 0x3A;   /**< Interface pixel format */
static const unsigned char INST_FRMCTR1 = 0xB1;  /**< Frame rate control (normal mode/full colors) */
static const unsigned char INST_FRMCTR2 = 0xB2;  /**< Frame rate control (idle mode/8-colors) */
static const unsigned char INST_FRMCTR3 = 0xB3;  /**< Frame rate control (partial mode/full colors) */
static const unsigned char INST_INVCTR = 0xB4;   /**< Display inversion control */
static const unsigned char INST_PWCTR1 = 0xC0;   /**< Power control1 */
static const unsigned char INST_PWCTR2 = 0xC1;   /**< Power control2 */
static const unsigned char INST_PWCTR3 = 0xC2;   /**< Power control3 */
static const unsigned char INST_PWCTR4 = 0xC3;   /**< Power control4 */
static const unsigned char INST_PWCTR5 = 0xC4;   /**< Power control5 */
static const unsigned char INST_VMCTR1 = 0xC5;   /**< VCOM control 1 */
static const unsigned char INST_VMOFCTR2 = 0xC7; /**< VCOM control 2 */

/********************************** Types ************************************/
/**
 * @brief Define union for MADCTL for easy bit and byte addressing
 * @details This will be useful for determining if rotations are in effect. A
 *          union is basically used to provide several different ways of using
 *          the same space in memory. In this case, we are using the same space
 *          in memory to define a struct with individual bits we can address, 
 *          and an unsigned char (byte) that encompasses these bits. This is
 *          the same mechanism that allows you to write LATD = 1 and
 *          LATDbits.LATD1 = 1 and accomplish the same thing.
 */
typedef union{
    struct{
        unsigned        :1; /**< LSb */
        unsigned        :1; /**< UNUSED */
        unsigned MH     :1; /**< Horizontal refresh order (0 --> refresh left to right) */
        unsigned RGB    :1; /**< Red, green, and blue pixel position change (0 --> RGB) */
        unsigned ML     :1; /**< Scan direction (0 --> refresh top to bottom) */
        unsigned MV     :1; /**< Row/column exchange (0 --> normal) */
        unsigned MX     :1; /**< Mirror x-axis (0 --> left to right) */
        unsigned MY     :1; /**< MSb - Mirror y-axis (0 --> top to bottom) */
    };
    unsigned char reg; /**< Way of accessing the above 8 bits, as a byte */
}MADCTLbits_t;

/***************************** Private Variables *****************************/
static MADCTLbits_t MADCTLbits;

/***************************** Public Functions ******************************/
void glcdTransfer(unsigned char byte, glcd_transfer_mode_e cmd){
    RS_GLCD = (cmd == CMD) ? 0 : 1; // RS low for command, and high for data
    
    // Enable serial interface and indicate the start of data transmission by
    // selecting the display (slave) for use with SPI
    CS_GLCD = 0;
    
    spiSend(byte);
    
    CS_GLCD = 1; // Deselect display
}

void glcd_swreset(void){
    glcdTransfer(INST_SWRESET, CMD);
    __delay_ms(130); // Delay specified on pg. 83 of datasheet
}

void glcd_slpin(void){
    glcdTransfer(INST_SLPIN, CMD);
    // Delay specified on pg. 93 of datasheet to stabilize power circuits
    __delay_ms(130);
}

void glcd_slpout(void){
    glcdTransfer(INST_SLPOUT, CMD);
    // Delay specified on pg. 94 of datasheet to stabilize timing for supply
    // voltages and clock circuits
    __delay_ms(130);
}

void glcd_setmadctl(void){
    glcdTransfer(INST_MADCTL, CMD);
    glcdTransfer(MADCTLbits.reg, MEMWRITE);
}

void glcd_ptlon(void){
    glcdTransfer(INST_PTLON, CMD);
}

void glcd_noron(void){
    glcdTransfer(INST_NORON, CMD);
}

void glcd_invoff(void){
    glcdTransfer(INST_INVOFF, CMD);
}

void glcd_invon(void){
    glcdTransfer(INST_INVON, CMD);
}

void glcd_dispoff(void){
    glcdTransfer(INST_DISPOFF, CMD);
}

void glcd_dispon(void){
    glcdTransfer(INST_DISPON, CMD);
}

void glcd_ramwr(void){
    glcdTransfer(INST_RAMWR, CMD);
}

void glcd_teoff(void){
    glcdTransfer(INST_TEOFF, CMD);
}

void glcd_idmoff(void){
    glcdTransfer(INST_IDMOFF, CMD);
}

void glcd_idmon(void){
    glcdTransfer(INST_IDMON, CMD);
}

void glcdDrawRectangle(
    unsigned char XS,
    unsigned char XE,
    unsigned char YS,
    unsigned char YE,
    unsigned long color
)
{
    // This first control flow structure is to issue "software fixes" for
    // the rotation settings. These adjustments are performed to ensure that
    // arguments for XS, XE, YS, and YE in the acceptable range will always
    // be placed in display RAM that's pixel-mapped on our display panel.
    // RECALL: The mirror/exchange effects (and effectively rotation) are 
    // configured by the MADCTL register
#if defined(V1_1)
    if(MADCTLbits.MX == 1 && MADCTLbits.MV ==1){
        // Case: origin top-left
        YS += 3;
        YE += 3;
        XS += 2;
        XE += 2;
    }
    else if(MADCTLbits.MY == 1){
        // Case: origin top-right
        YS += 2;
        YE += 2;
        XS += 3;
        XE += 3;
    }
    else if(MADCTLbits.MX == 1){
        // Case: origin bottom-left
        YS += 2;
        YE += 2;
        XS += 1;
        XE += 1;
    }
    else{
        // Case: origin bottom-right
        YS += 1;
        YE += 1;
        XS += 2;
        XE += 2;
    }
#elif defined(V2_1)
    if(MADCTLbits.MX == 1 && MADCTLbits.MV ==1){
        // Case: origin top-left
        YS += 32;
        YE += 32;
    }
    else if(MADCTLbits.MY == 1){
        // Case: origin top-right
        XS += 32;
        XE += 32;
    }
#else
    #error "Must define either V1_1 or V2_1 in the GLCD's header file"
#endif
    
    // Set row address counter (specifies the start (XS) and end (XE) positions
    // of the drawing window
    glcdTransfer(INST_RASET, CMD);
    glcdTransfer(0x00, MEMWRITE); // XS[15:8]
    glcdTransfer(XS, MEMWRITE); // XS[7:0]
    glcdTransfer(0x00, MEMWRITE); // XE[15:8]
    glcdTransfer(XE - 1, MEMWRITE); // XE[7:0]
    
    // Set column address counter (specifies the start (YS) and end (YE)
    // positions of the drawing window
    glcdTransfer(INST_CASET, CMD);
    glcdTransfer(0x00, MEMWRITE); // YS[15:8]
    glcdTransfer(YS, MEMWRITE); // YS[7:0]
    glcdTransfer(0x00, MEMWRITE); // YE[15:8]
    glcdTransfer(YE - 1, MEMWRITE); // YE[7:0]
    
    glcd_ramwr(); // Send the RAM write command to the display controller.
    
    // If only drawing one pixel...save the PIC processor the time for computing
    // loop parameters
    if((XE == XS) && (YE == YS)){
        // Provide color data. This data will be passed as inputs to a look-up
        // table (LUT) in the GLCD. The LUT will then output 18 bits of color
        // to the location in data RAM specified by the row address pointer and
        // column address pointer. The memory-mapped pixel will then take on
        // the specified color configuration. Note that we do not have to send
        // a command before doing this because the previous commands take care
        // of all the require configuration details
        glcdTransfer(color & 0xFF, MEMWRITE); // Blue pixel data
        glcdTransfer((color >> 8) & 0xFF, MEMWRITE); // Green pixel data
        glcdTransfer((color >> 16) & 0xFF, MEMWRITE); // Red pixel data
    }
    else{
        // Write color data to the GLCD for all the pixels in the window. Note
        // that the GLCD controller auto-increments the addresses being written
        // to in the RAM, which is why we can continuously write after
        // specifying a window.
        //
        // Also, the following were done for efficiency:
        //  1. Pre-compute number of loops (multiplication would add an extra
        //     runtime step)
        //  2. Extract data for individual colors from the argument (also slow
        //     if you had to do this everytime you ran the loop)
        //  3. Directly use the spiSend function as opposed to glcdTransfer to
        //     reduce function call overhead
        unsigned short numLoops = (XE - XS) * (YE - YS);
        unsigned char colorData[3];
        colorData[0] = color & 0xFF;
        colorData[1] = (color >> 8) & 0xFF;
        colorData[2] = (color >> 16) & 0xFF;
        
        CS_GLCD = 0; // Select GLCD as slave device
        RS_GLCD = 1; // Select the display data RAM
        for(unsigned short i = 0; i < numLoops; i++){
            spiSend(colorData[0]); // Blue pixel data
            spiSend(colorData[1]); // Green pixel data
            spiSend(colorData[2]); // Red pixel data
        }
        CS_GLCD = 1; // Deselect the GLCD as slave device
    }
}

void glcdDrawPixel(unsigned char XS, unsigned char YS, unsigned long color){   
    // Take care of edge cases. Note that there is no less than zero edge case
    // due to the unsignedness of unsigned char causing overflow instead
    if(XS >= GLCD_SIZE_HORZ){
        XS = GLCD_SIZE_HORZ - 1;
    }
    if(YS >= GLCD_SIZE_VERT){
        YS = GLCD_SIZE_HORZ - 1;
    }
    
    glcdDrawRectangle(XS, XS, YS, YS, color);
}

void glcdSetCOLMOD(unsigned char numBitsPerPixel){
    unsigned short rawData;
    switch(numBitsPerPixel){
        case 12:
            rawData = 0b00000011; // 4 bits per color
            break;
        case 16:
            rawData = 0b00000101; // 5 bits per color
            break;
        case 18:
            rawData = 0b00000110; // 6 bits per color
            break;
        default:
            rawData = 0b00000110; // case 18
            break;
    }
    glcdTransfer(INST_COLMOD, CMD);
    glcdTransfer(rawData, MEMWRITE);
}

void glcdSetOrigin(glcd_origin_positions_e corner){
    // Set MADCTL bits to reflect the configuration
    switch(corner){
        case ORIGIN_TOP_LEFT:
            MADCTLbits.MY = 1;
            MADCTLbits.MX = 1;
            MADCTLbits.MV = 1;
            break;
        case ORIGIN_TOP_RIGHT:
            MADCTLbits.MY = 1;
            MADCTLbits.MX = 0;
            MADCTLbits.MV = 0;
            break;
        case ORIGIN_BOTTOM_RIGHT:
            MADCTLbits.MY = 0;
            MADCTLbits.MX = 0;
            MADCTLbits.MV = 1;
            break;
        case ORIGIN_BOTTOM_LEFT:
            MADCTLbits.MY = 0;
            MADCTLbits.MX = 1;
            MADCTLbits.MV = 0;
            break;
        default:
            MADCTLbits.MY = 1;
            MADCTLbits.MX = 1;
            MADCTLbits.MV = 1;
            break;            
    }
    
    glcd_setmadctl(); // Push changes to GLCD
}

void initGLCD(void){        
    // Ensure pin I/O is correct
    CS_GLCD = 1; // Deselect GLCD
    RS_GLCD = 1; // Set RS high
    TRIS_CS_GLCD = 0; // Set CS data direction to output
    TRIS_RS_GLCD = 0; // Set RS data direction to output
    
    // Start SPI module with maximum available clock frequency (FOSC / 4)
    spiInit(4);
    
    // Wait 20 ms, just in case this function is called before the power supply
    // to the GLCD has stabilized
    __delay_ms(20);
    
    /************************** Initialization ritual *************************/
    glcd_swreset(); // Issue a software reset
    
    glcd_slpout(); // Force exit from sleep mode
    
    // Configure frame rate (FR) registers
    glcdTransfer(INST_FRMCTR1, CMD); // Issue command to configure normal mode FR
    glcdTransfer(0x00, MEMWRITE); // Parameter 1
    glcdTransfer(0x06, MEMWRITE); // Parameter 2
    glcdTransfer(0x03, MEMWRITE); // Parameter 3
    
    glcdTransfer(INST_FRMCTR2, CMD); // Issue command to configure idle mode FR
    glcdTransfer(0x01, MEMWRITE); // Parameter 1 (default)
    glcdTransfer(0x2C, MEMWRITE); // Parameter 2 (default)
    glcdTransfer(0x2D, MEMWRITE); // Parameter 3 (default)
    
    glcdTransfer(INST_FRMCTR3, CMD); // Issue command to configure partial mode FR
    glcdTransfer(0x01, MEMWRITE); // Parameter 1 (default)
    glcdTransfer(0x2C, MEMWRITE); // Parameter 2 (default)
    glcdTransfer(0x2D, MEMWRITE); // Parameter 3 (default)
    
    glcdTransfer(INST_INVCTR, CMD); // Issue command to configure display inversion control
    glcdTransfer(0x00, MEMWRITE); // No inversion
    
    // Configure power control
    glcdTransfer(INST_PWCTR1, CMD); // Issue command to configure PWCTR1 register
    glcdTransfer(0xA2, MEMWRITE); // Set GVDD to 3.9 V and AVDD to 5 V
    glcdTransfer(0x02, MEMWRITE); // Set GVCL to -4.6 V
    glcdTransfer(0x84, MEMWRITE); // Set FUNCTION to AUTO
    
    glcdTransfer(INST_PWCTR2, CMD); // Issue command to configure power supply level
    glcdTransfer(0xC5, MEMWRITE); // See datasheet pg.132 (default)
    
    glcdTransfer(INST_PWCTR3, CMD); // Issue command to configure op amp current for normal mode
    glcdTransfer(0x0A, MEMWRITE); // See datasheet pg.134  (default)
    glcdTransfer(0x00, MEMWRITE); // See datasheet pg. 134 (default)
    
    glcdTransfer(INST_PWCTR4, CMD); // Issue command to configure op amp current for idle mode
    glcdTransfer(0x8A, MEMWRITE); // See datasheet pg.134  (default)
    glcdTransfer(0x2A, MEMWRITE); // Clock frequency for voltage booster circuit divided by 2
    
    glcdTransfer(INST_PWCTR5, CMD); // Issue command to configure op amp current for partial mode
    glcdTransfer(0x8A, MEMWRITE); // See datasheet pg.138  (default)
    glcdTransfer(0x2A, MEMWRITE); // Clock frequency for voltage booster circuit divided by 2
    
    // VCOM Control
    glcdTransfer(INST_VMCTR1, CMD); // Issue command to configure VCOM voltage setting
    glcdTransfer(0x3C, MEMWRITE); // Important parameter for power circuits
    
    glcd_invoff(); // Force no display inversion

    /************************** User-defined options **************************/
    // Configure pixel interface format. NOTE: If it is desired to improve the      
    // performance of the display, you can set the display to use 12 bits of        
    // color instead. The color definitions provided would then all need to be      
    // changed, as well as some modifications to the glcdTransfer function where        
    // the color data is being sent
    glcdSetCOLMOD(18); // Enforce default format: 18 bits of color per pixel
    
    // Set gamma curve
    glcdTransfer(INST_GAMSET, CMD); // Issue command to set gamma curve
    glcdTransfer(0x01, MEMWRITE); // Gamma curve 2.2 (default)
    
    glcd_idmoff(); // Force exit from idle mode (mandatory)
    
    glcd_noron(); // Force normal display mode (mandatory)
    
    // Set mirror/exchange effects (datasheet pg. 63)
    MADCTLbits.MX = 1;
    MADCTLbits.MY = 1;
    MADCTLbits.MV = 1;
    MADCTLbits.ML = 0;
    MADCTLbits.RGB = 0;
    MADCTLbits.MH = 0;
    glcd_setmadctl();
    
    // Fill black rectangle to overwrite default white value. This indicates the
    // GLCD is working properly
    glcdDrawRectangle(0, GLCD_SIZE_HORZ, 0, GLCD_SIZE_VERT, BLACK);
    
    glcd_dispon(); // Turn on display (enable output from frame memory; mandatory)
    __delay_ms(10); // Just in case the display needs time to turn on
}
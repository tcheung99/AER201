/**
 * @file
 * @author Tyler Gamvrelis
 *
 * Created on August 13, 2017, 6:00 PM
 *
 * @defgroup GLCD
 * @brief Driver for ST7735R-based 128 x 128 GLCD
 * @{
 */

#ifndef GLCD_PIC_H
#define GLCD_PIC_H

/********************************* Includes **********************************/
#include <xc.h>
#include <configBits.h>

/********************************** Macros ***********************************/
// The GLCD's red PCB will have a version number printed on the back. You must
// define a macro with the appropriate version number in order to for the code
// to work properly. Currently, versions 1.1 and 2.1 are supported by this
// library.
#define V1_1
// #define V2_1

// RD1 is RS, which is DCX (data/command flag) in the datasheet.
// 
// RS = 0 --> command to display controller
// RS = 1 --> data for display data RAM
#define RS_GLCD      LATDbits.LATD1   /**< Register select */
#define TRIS_RS_GLCD TRISDbits.TRISD1 /**< TRIS for RS pin */

// RD0 is CS, which is CSX (chip enable) in the datasheet. 
// 
// CS = 0 --> display selected for control via SPI
// CS = 1 --> display not selected for control
#define CS_GLCD      LATDbits.LATD0   /**< Chip select     */
#define TRIS_CS_GLCD TRISDbits.TRISD0 /**< TRIS for CS pin */

/******************************** Constants **********************************/
// Display dimensions addressable in ST7735 controller display data RAM
extern const unsigned char GLCD_ADDRESSABLE_SIZE_HORZ; /**< 128 pixels */
extern const unsigned char GLCD_ADDRESSABLE_SIZE_VERT; /**< 160 pixels */

// Display dimensions as seen in the real world
extern const unsigned char GLCD_SIZE_HORZ; /**< 0 to 127 --> 128 pixels */
extern const unsigned char GLCD_SIZE_VERT; /**< 0 to 127 --> 128 pixels */

// 18-bit color depth information here.
// 
// Each color is encoded by 3 bytes. For each byte, the 6 most significant bits
// contain useful information and the two lest significant bits are don't cares.
// Color = B2B1B0 = RGB.
// 
// Thus, for each color component (red, green and blue or RGB for short), there 
// are 2^6 possible values. Since there are three different colors, there are
// thus (2^6)^3 = 2^18 = 262,144 colors.
// 
// The RGB color model is additive, that is, an arbitrary color in the RGB color
// space is formed by superimposing the intensities of the separate components.
// Thus, black is zero intensity for all components, and white is full intensity
// for all components.
extern const unsigned long BLACK;
extern const unsigned long GREY;
extern const unsigned long WHITE;
extern const unsigned long RED;
extern const unsigned long ORANGE;
extern const unsigned long YELLOW;
extern const unsigned long GREEN;
extern const unsigned long BLUE;
extern const unsigned long INDIGO;
extern const unsigned long VIOLET;

/********************************** Types ************************************/
/**
 * @brief Origin locations, relative to the display when mounted on the
 *        DevBugger
 */
typedef enum{
    ORIGIN_TOP_LEFT,
    ORIGIN_TOP_RIGHT,
    ORIGIN_BOTTOM_LEFT,
    ORIGIN_BOTTOM_RIGHT
}glcd_origin_positions_e;

/** @brief Arguments for low-level driver, glcdTransfer */
typedef enum{
    MEMWRITE = 0, /**< Data to be written into RAM (picture data, etc.) */
    CMD = 1       /**< A command for the GLCD controller */
}glcd_transfer_mode_e;

/************************ Public Function Prototypes *************************/
/**
 * @brief Driver to interface with the SPI module to send data to the GLCD.
 *        This is the fundamental communication interface, upon which all the
 *        others (pixels, rectangles) are built
 * @param byte The byte corresponding to the desired command/data value
 * @param cmd Indicates whether the byte should be interpreted as a command
 *        for the display controller (CMD), or data for display RAM (MEMWRITE)
 */
void glcdTransfer(unsigned char byte, glcd_transfer_mode_e cmd);

/** @brief Sets all registers to their default value */
void glcd_swreset(void);

/** @brief Enters sleep mode */
void glcd_slpin(void);

/** @brief Exits sleep mode */
void glcd_slpout(void);

/** @brief Sets the mirror/exchange parameters */
void glcd_setmadctl(void);

/** @brief Turns on partial mode */
void glcd_ptlon(void);

/** @brief Turns off partial mode (normal) */
void glcd_noron(void);

/** @brief Turns display inversion off */
void glcd_invoff(void);

/** @brief Turns display inversion on */
void glcd_invon(void);

/** @brief Turns off display */
void glcd_dispoff(void);

/** @brief Turns on display */
void glcd_dispon(void);

/** @brief Enables writing to the display data RAM */
void glcd_ramwr(void);

/** @brief Turns off the tearing effect */
void glcd_teoff(void);

/** @brief Stops display idling */
void glcd_idmoff(void);

/** @brief Starts display idling */
void glcd_idmon(void);

/**
 * @brief Draws a solid rectangle in the specified window
 * @param XS Start position on the x-axis (min: 0, max: GLCD_SIZE_HORZ)
 * @param XE End position on the x-axis (min: XS, max: GLCD_SIZE_HORZ)
 * @param YS Start position on the y-axis (min: 0, max GLCD_SIZE_VERT)
 * @param YE End position on the y-axis (min: YS, max: GLCD_SIZE_VERT)
 * @param color Color of the rectangle
 */
void glcdDrawRectangle(
    unsigned char XS,
    unsigned char XE,
    unsigned char YS,
    unsigned char YE,
    unsigned long color
);

/**
 * @brief Draws the color specified at the coordinates specified relative to
 *        the origin
 * @param XS x-position of the pixel (min: 0, max: GLCD_SIZE_HORZ)
 * @param YS y-position of the pixel (min: 0, max GLCD_SIZE_VERT)
 * @param color Color of the pixel
 */
void glcdDrawPixel(unsigned char XS, unsigned char YS, unsigned long color);

/**
 * @brief Sets the interface pixel format. Default is 18 bits per pixel (bpp)
 * @param numBitsPerPixel Desired color depth. Must be 12, 16, or 18 (bpp).
 *        Defaults to 18
 */
void glcdSetCOLMOD(unsigned char numBitsPerPixel);

/**
 * @brief Sets bits in the GLCD's MADCTL register to change the mirror/exchange
 *        effects, thereby rotating the display
 * @param corner The corner in which the origin is to be placed
 */
void glcdSetOrigin(glcd_origin_positions_e corner);

/**
 * @brief Performs the GLCD initialization sequence
 * @note Credits go to Sumotoy for the power initialization parameters. These
 *       are not something that the datasheet tells you how to configure, as
 *       their meanings are directly related to the panel-driving hardware.
 *       These parts of initialization are in the section "Initialization
 *       ritual", while the parts most interesting for the user are in the
 *       "User-defined options" section.
 * 
 * @note The most important things to understand about this function are:
 *  -# It sets up the SPI interface with the GLCD
 *  -# It issues commands to the GLCD that initialize its integrated panel-
 *     driving hardware
 *  -# It sets up the COLOR DEPTH when glcdSetCOLMOD is called. This is
 *     directly related to the performance of the GLCD (i.e. how long it
 *     takes to write data to it
 */
void initGLCD(void);

/**
 * @}
 */

#endif	/* GLCD_PIC_H */
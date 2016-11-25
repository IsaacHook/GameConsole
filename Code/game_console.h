/*************************************************************************
Title:    Game Console Template Header File
Inital Author:   David Jahshan
Extended by : Isaac Hook:  720679
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Macros for Game Console
*************************************************************************/

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define byte unsigned char 

/*ON OFF*/
#define ON 	 0xFF
#define OFF  0x00
#define HIGH 0xFF
#define LOW  0x00
#define IN 	 0x00
#define OUT  0xFF
#define ALL  0xFF
#define TRUE 1
#define FALSE 0
#define FORWARD 0x00
#define BACK 0xFF

/*SET and GET MACRO*/
    #define SET(PORT,MASK,VALUE) PORT = ((MASK & VALUE) | (PORT & ~MASK))
    #define GET(PORT,MASK) PORT & MASK

// ==== Devices Direction MACROs ===
// #define MULTI_LINES_DIR(DIR) SET(DDR?,(_BV(P?#)|_BV(P?#)|_BV(P?#)),DIR)
    // DDRx refers to the register for ports A, B, C etc....is an 8 bit register that sets the port diretions
    // DDxn is a bit within the DD registers, e.g. DDA7 
    // x refers to the numbering letter of the port, n refers to the bit number
    // If DDxn = 1 then set as output pin
    // If DDxn = 0 then set as input pin
    #define BAT_LOW_LED_DIR(DIR) SET(DDRC,_BV(PC1),DIR)
    #define BTN_UP_DIR(DIR) SET(DDRB,_BV(PB0),DIR)
    #define BTN_DOWN_DIR(DIR) SET(DDRB,_BV(PB1),DIR)
    #define BTN_LEFT_DIR(DIR) SET(DDRA,_BV(PA3),DIR)
    #define BTN_RIGHT_DIR(DIR) SET(DDRB,_BV(PB3),DIR)

    // MOSI, MISO and all that gang
    #define MOSI_DIR(DIR) SET(DDRB,_BV(PB5),DIR)
    #define MISO_DIR(DIR) SET(DDRB,_BV(PB6),DIR)
    #define SCK_DIR(DIR) SET(DDRB,_BV(PB7),DIR)
    #define SPCR_MSTR(STATE) SET(SPCR,_BV(MSTR),STATE)
    #define SPCR_SPR0(STATE) SET(SPCR,_BV(SPR0),STATE)
    #define SPCR_SPE(STATE) SET(SPCR,_BV(SPE),STATE)
    #define SPI_SS(DIR) SET(DDRB,_BV(PB4),DIR)


    // LCD Screen
    #define LCD_PWM_DIR(DIR) SET(DDRD,_BV(PD4),DIR)
    #define LCD_RESET_DIR(DIR) SET(DDRD,_BV(PD6),DIR)
    #define LCD_CD_DIR(DIR) SET(DDRD,_BV(PD7),DIR)
    #define LCD_CHIP_SELECT_DIR(DIR) SET(DDRD,_BV(PD5),DIR)

// ==== Devices Outputs ===
    // The function of PORTxn depends on whether the pin is set as input or output
    // If port = input, then 1 = pull up resistor, 0 = no pull up resistor
    // If port = output, then 1 = HIGH, 0 = LOW

    #define BAT_LOW_LED(STATE) SET(PORTC,_BV(PC1),~STATE)

    // Button Definitions
    #define BTN_UP_SET(STATE) SET(PORTB, _BV(PB0), STATE)
    #define BTN_DOWN_SET(STATE) SET(PORTB,_BV(PB1),STATE)
    #define BTN_RIGHT_SET(STATE) SET(PORTB,_BV(PB3),STATE)
    #define BTN_LEFT_SET(STATE) SET(PORTA,_BV(PA3),STATE)
    #define BTN_A_SET(STATE) SET(PORTA,_BV(PA2),STATE)
    #define BTN_B_SET(STATE) SET(PORTA,_BV(PA1),STATE)
    #define BTN_START_SET(STATE) SET(PORTA,_BV(PA0),STATE)

    // LCD Screen
    #define LCD_CHIP_SELECT(STATE) SET(PORTD,_BV(PD5),STATE)
    #define LCD_PWM_SET(STATE) SET(PORTD,_BV(PD4),STATE)
    #define LCD_RESET_SET(STATE) SET(PORTD,_BV(PD6),STATE)
    #define LCD_CD_SET(STATE) SET(PORTD,_BV(PD7),STATE)

        // MOSI, MISO and all that gang
    #define MOSI_SET(STATE) SET(PORTB,_BV(PB5),STATE)
    #define MISO_SET(STATE) SET(PORTB,_BV(PB6),STATE)
    #define SCK_SET(STATE) SET(PORTB,_BV(PB7),STATE)
    #define SPI_SS_SET(STATE) SET(PORTB,_BV(PB4),STATE)

// ==== Devices Inputs ===
    #define BTN_UP GET(PINB, _BV(PB0))
    #define BTN_DOWN GET(PINB,_BV(PB1))
    #define BTN_LEFT GET(PINA,_BV(PA3))
    #define BTN_RIGHT GET(PINB,_BV(PB3))











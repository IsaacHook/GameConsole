/*************************************************************************
Title:    game_console Template
Initial Author:   David Jahshan
Extended by : (Isaac Hook) (720679)
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
    Main and functions for Game Console basic sample code

*************************************************************************/
// Header files
#include <util/delay.h>
#include "game_console.h"
#include "writeToLCD.h"

// Function Prototypes
void SetupRegisters_SPI(void);
byte LCD_command_tx(byte tx_byte);
byte LCD_initialise(void);
void SPI_MasterTransmit(byte cData, byte isData);
byte select_page (byte page);
byte select_column (byte column);
void SetupRegisters_Buttons();
void LCD_clear(void);
void etchaSketch(void);
void printText(void);
void snake(void);
void Init_PWM(void);
void SetupRegisters_LCD(void);
void frameBuffer_clear(void);
void SetupRegisters_LEDs(void);
void SetupInterupts(void);

// Global Vars
byte frame_buffer[MAX_PAGES][MAX_COLUMNS] = {{0x00}};
byte row = 32; byte column = 51;
byte page, pixel; 
char direction = 'n';
int myVal = 0;

int main(void)
{
    SetupRegisters_LEDs();//Setup LEDs
    SetupRegisters_Buttons();// Setup Buttons
    SetupRegisters_LCD();// Setup LCD
    SetupRegisters_SPI(); // Setup SPI
    SetupInterupts();

    LCD_initialise(); // Send initialisation commands over SPI
    //Init_PWM();
    
    while (TRUE)             //Master loop always true so always loop
    {
    etchaSketch();
        if (myVal >= 1){
            BAT_LOW_LED(ON);
        }
        //snake();
        if (~BTN_UP)
        {
            // BAT_LOW_LED(ON);
            direction = 'u';
        }
        else if (~BTN_LEFT)
        {
            direction = 'l';
            // BAT_LOW_LED(ON);
        }
        else if (~BTN_RIGHT)
        {
            direction = 'r';
            // BAT_LOW_LED(ON);
        }
        else if (~BTN_DOWN)
        {
            direction = 'd';
            // BAT_LOW_LED(ON);
        }
        else
        {
            // BAT_LOW_LED(OFF);
        }
    }
}
ISR(INT1_vect){
            myVal++;
        }


void SetupRegisters_Buttons(){
    BTN_UP_SET(HIGH);
    BTN_DOWN_SET(HIGH);
    BTN_LEFT_SET(HIGH);
    BTN_RIGHT_SET(HIGH);

    BTN_UP_DIR(IN);
    BTN_DOWN_DIR(IN);
    BTN_LEFT_DIR(IN);
    BTN_RIGHT_DIR(IN);
}

void printText(void){
    select_page (0); select_column (0);
    SPI_MasterTransmit(0xFF, DATA);
    SPI_MasterTransmit(0x08, DATA);
    SPI_MasterTransmit(0x08, DATA);
    SPI_MasterTransmit(0x08, DATA);
    SPI_MasterTransmit(0x08, DATA);
    SPI_MasterTransmit(0xFF, DATA);
}

void snake(void){
    if (direction == 'u'){
        row--;
    }
    else if (direction == 'd'){
        row++;
    }
    else if (direction == 'l'){
        column--;
    }
    else if (direction == 'r'){
        column++;
    }
    else {
    }
    if (row==OVERFLOW){row = 0;}
    if (row>MAX_ROWS-1){row = MAX_ROWS-1;}
    if (column==OVERFLOW){column = 0;}
    if (column>MAX_COLUMNS-1){column = MAX_COLUMNS-1;}
    page = row/8;
    pixel = row%8;
    select_page(page);
    select_column(column);
    if (direction != 'n'){
        if ((_BV(pixel) & (frame_buffer[page][column])) == _BV(pixel)){
            LCD_clear();
            frameBuffer_clear();
            direction = 'n';
        }
    }
    pixel = (_BV(pixel) | (frame_buffer[page][column]));
    frame_buffer[page][column] = pixel;
    SPI_MasterTransmit(pixel, DATA);
    _delay_ms(35);
}

void etchaSketch(void){
        if(BTN_UP) row++;
        if(BTN_DOWN) row --;
        if(~BTN_LEFT)column--;
        if(~BTN_RIGHT)column++;
        page = row/8;
        pixel = row%8;
        select_page(page);
        select_column(column);
        pixel = (_BV(pixel) | (frame_buffer[page][column]));
        frame_buffer[page][column] = pixel;
        SPI_MasterTransmit(pixel, DATA);
        _delay_ms(255);
}

void SetupRegisters_SPI(void) {
    /* Set MOSI and SCK output, all others input */
    MOSI_DIR(OUT); SCK_DIR(OUT); SPI_SS(OUT);
    LCD_CHIP_SELECT_DIR(OUT); LCD_CD_DIR(OUT);
    
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPHA)|(1<<CPOL)|(1<<SPR0);
    //SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(byte cData, byte isData) {
    LCD_CD_SET(isData);
    LCD_CHIP_SELECT(LOW);
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));
    LCD_CHIP_SELECT(HIGH);
}

byte LCD_initialise(void)
{
    SPI_MasterTransmit(0x40,COMMAND);//Display start line 0
    SPI_MasterTransmit(0xA1,COMMAND);//SEG reverse
    SPI_MasterTransmit(0xC0,COMMAND);//Normal COM0~COM63
    SPI_MasterTransmit(0xA4,COMMAND);//Disable -> Set All Pixel to ON
    SPI_MasterTransmit(0xA6,COMMAND);//Display inverse off
    _delay_ms(120);
    SPI_MasterTransmit(0xA2,COMMAND);//Set LCD Bias Ratio A2/A3
    SPI_MasterTransmit(0x2F,COMMAND);//Set Power Control 28...2F
    SPI_MasterTransmit(0x27,COMMAND);//Set VLCD Resistor Ratio 20...27
    SPI_MasterTransmit(0x81,COMMAND);//Set Electronic Volume
    SPI_MasterTransmit(0x10,COMMAND);//Set Electronic Volume 00...3F
    SPI_MasterTransmit(0xFA,COMMAND);//Set Adv. Program Control
    SPI_MasterTransmit(0x90,COMMAND);//Set Adv. Program Control x00100yz yz column wrap x Temp Comp
    SPI_MasterTransmit(0xAF,COMMAND);//Display on
    LCD_clear();
    frameBuffer_clear();
return(TRUE);
}

void LCD_clear(void){
    byte page; byte column;
    for (page = 0; page < MAX_PAGES; page++){
        for (column = 0; column < MAX_COLUMNS; column++){
                select_page (page);
                select_column (column);
                SPI_MasterTransmit(0x00,DATA);
        }
    }
}

void frameBuffer_clear(void){
    byte page; byte column;
    for (page = 0; page < MAX_PAGES; page++){
        for (column = 0; column < MAX_COLUMNS; column++){
            frame_buffer[page][column] = OFF;
        }
    }
}

byte select_page (byte page) {
    byte page_cmd_address;
    page_cmd_address =(CMD_PAGE | page); //CMD_PAGE = 1011----, Page = ----000x
    SPI_MasterTransmit(page_cmd_address,COMMAND); 
    return(TRUE);
}

byte select_column (byte column) {
    byte page_cmd_address_MSB;
    byte page_cmd_address_LSB;
    page_cmd_address_LSB =(CMD_COL_LSB | (column&0x0F));
    page_cmd_address_MSB =(CMD_COL_MSB | (column >> 4));
    SPI_MasterTransmit(page_cmd_address_LSB,COMMAND);
    SPI_MasterTransmit(page_cmd_address_MSB,COMMAND);
    return(TRUE);
}
void Init_PWM(void){
    TCNT0 = 0x00; // initialize counter
    //TCCR0 = (1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS02)|(0<<CS01)|(1<<CS00); // no prescaling, set pwm to pwm, 
    TCCR1A = (0<<WGM12)|(0<<WGM11)|(0<<WGM10)|(0<<COM1B1)|(1<<COM1B0);
    TCCR1B = (1<<CS12)|(0<<CS11)|(1<<CS10);
    OCR0 = 0x00;
}

void SetupRegisters_LCD(void){
    LCD_RESET_DIR(OUT);
    LCD_RESET_SET(LOW);
    _delay_ms(120);
    LCD_RESET_SET(HIGH);

    LCD_PWM_SET(OFF);
    LCD_PWM_DIR(OUT);
    LCD_PWM_SET(HIGH);
}

void SetupRegisters_LEDs(void){
    BAT_LOW_LED(OFF);       //Make sure it is off before changing direction
    BAT_LOW_LED_DIR(OUT);   //Set BATTERY LED I/Os as outputs.
}

void SetupInterupts(void){
    // _SLEEP();
    SET(MCUCR,(_BV(ISC11)|_BV(ISC10)), ON);
    SET(GICR, _BV(INT1), ON);
    sei();
}


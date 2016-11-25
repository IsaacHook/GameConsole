byte LCD_initialise(void)
{
    LCD_command_tx(0x40);//Display start line 0
    LCD_command_tx(0xA1);//SEG reverse
    LCD_command_tx(0xC0);//Normal COM0~COM63
    LCD_command_tx(0xA4);//Disable -> Set All Pixel to ON
    LCD_command_tx(0xA6);//Display inverse off
    _delay_ms(120);
    LCD_command_tx(0xA2);//Set LCD Bias Ratio A2/A3
    LCD_command_tx(0x2F);//Set Power Control 28...2F
    LCD_command_tx(0x27);//Set VLCD Resistor Ratio 20...27
    LCD_command_tx(0x81);//Set Electronic Volume
    LCD_command_tx(0x10);//Set Electronic Volume 00...3F
    LCD_command_tx(0xFA);//Set Adv. Program Control
    LCD_command_tx(0x90);//Set Adv. Program Control x00100yz yz column wrap x Temp Comp
    LCD_command_tx(0xAF);//Display on
return(TRUE);
}

byte LCD_command_tx(byte tx_byte) //Sends a data byte 
{
    byte tx_processed;
    byte tx_mask = 0x80;  //MSB is set
    LCD_CHIP_SELECT(LOW);
    LCD_CD_SET(LOW);
    while (tx_mask != 0x00) {
        tx_processed = tx_byte & tx_mask; 
        SCK_SET(HIGH);
        if(tx_processed)
            MOSI_SET(HIGH);
        else
            MOSI_SET(LOW); 
        SCK_SET(LOW);
        tx_mask>>=1;
        }
    SCK_SET(HIGH);
    LCD_CHIP_SELECT(HIGH);
    return(TRUE);
}

byte LCD_data_tx(byte tx_byte) //Sends a data byte 
{
    byte tx_processed;
    byte tx_mask = 0x80;  //MSB is set
    LCD_CHIP_SELECT(LOW);
    LCD_CD_SET(HIGH);
    while (tx_mask != 0x00) {
        tx_processed = tx_byte & tx_mask; 
        SCK_SET(HIGH);
        if(tx_processed)
            MOSI_SET(HIGH);
        else
            MOSI_SET(LOW); 
            SCK_SET(LOW);
            tx_mask>>=1;
        }
    SCK_SET(HIGH);
    LCD_CHIP_SELECT(HIGH);
    return(TRUE);
}
void init(void)
{
    SYSCTL_RCGCGPIO_R |= 0X03;
    GPIO_PORTB_DIR_R = 0xFF;
    GPIO_PORTB_DEN_R = 0xFF;
    GPIO_PORTA_DIR_R = 0xE2;
    GPIO_PORTA_DEN_R = 0xE2;
    GPIO_PORTA_AFSEL_R = 0x00;
    GPIO_PORTB_AFSEL_R = 0x00;
    GPIO_PORTA_AMSEL_R = 0x00;
    GPIO_PORTB_AMSEL_R = 0x00;
    GPIO_PORTA_PCTL_R = 0x00;
    GPIO_PORTB_PCTL_R = 0x00;
    LCD_command(0x30); //wake up

    LCD_command(0x38); //8 bits for display
    LCD_command(0x06); //shift
    LCD_command(0x01); //clear
    LCD_command(0x0F); //display on
}

void LCD_DATA(char data)
{
    GPIO_PORTA_DATA_R = 0x20;
    GPIO_PORTB_DATA_R = data;
    GPIO_PORTA_DATA_R |= 0x80;
    Delay_1();
    GPIO_PORTA_DATA_R = 0;
}

//LCD COMMAND
void LCD_command(char command)
{
    GPIO_PORTA_DATA_R = 0;
    GPIO_PORTB_DATA_R = command;
    GPIO_PORTA_DATA_R |= 0x80;
    Delay_1();
    GPIO_PORTA_DATA_R = 0;
}

unsigned char SW1_SW2_input(void)
{
    return GPIO_PORTF_DATA_R & 0x11;
}
// function to get the LED on.
void RG_out(unsigned char data)
{
    GPIO_PORTF_DATA_R &= ~0x0E;
    GPIO_PORTF_DATA_R |= data;
}
// intialization for LEDs.
void init_LEDs(void)
{
    //SYSCTL_RCGCGPIO_R |=0x20;
    //while((SYSCTL_PRGPIO_R & 0x20)==0);
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R |= 0x0E;
    GPIO_PORTF_AMSEL_R &= ~0x0E;
    GPIO_PORTF_PCTL_R &= ~0x0000FFF0;
    GPIO_PORTF_AFSEL_R &= ~0x0E;
    GPIO_PORTF_DIR_R |= 0x0E;
    GPIO_PORTF_DEN_R |= 0x0E;
    GPIO_PORTF_PUR_R |= 0x0E;
    GPIO_PORTF_DATA_R &= ~0x0E;
}
// initialization for switches.
void init_SW1_SW2(void)
{
    //initializing PORT F
    SYSCTL_RCGCGPIO_R |= 0x20;
    while ((SYSCTL_PRGPIO_R & 0x20) == 0)
        GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R |= 0x0E;
    GPIO_PORTF_AMSEL_R &= ~0x11;
    GPIO_PORTF_PCTL_R &= ~0x000F0000;
    GPIO_PORTF_AFSEL_R &= ~0x11;
    GPIO_PORTF_DIR_R &= ~0x11;
    GPIO_PORTF_DEN_R |= 0x11;
    GPIO_PORTF_PUR_R |= 0x11;
}

// Port A initization.
void Port_A_init(void)
{
    SYSCTL_RCGCGPIO_R |= (0x01);
    while ((SYSCTL_PRGPIO_R & 0x01) == 0)
        GPIO_PORTA_AMSEL_R &= ~0x04;
    GPIO_PORTA_PCTL_R &= ~0x00000F00;
    GPIO_PORTA_AFSEL_R &= ~0x04;
    GPIO_PORTA_DIR_R |= 0x04;
    GPIO_PORTA_DEN_R |= 0x04;
}

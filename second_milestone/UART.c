void UART3_Init(void)
{
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R3;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2;

    UART3_CTL_R &= ~UART_CTL_UARTEN;
    UART3_IBRD_R = 104; //104
    UART3_FBRD_R = 11;  //11
    UART3_LCRH_R = (0x3 << 5);
    UART3_CTL_R |= (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

    GPIO_PORTC_AFSEL_R |= 0xC0;
    GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & ~0xFF000000) | (GPIO_PCTL_PC7_U3TX | GPIO_PCTL_PC6_U3RX);
    GPIO_PORTC_DEN_R |= 0xC0;
    GPIO_PORTA_AMSEL_R &= ~0xC0;
}
void UART1_Init(void)
{
    SYSCTL_RCGCUART_R |= 0x00000002; // activate UART1

    SYSCTL_RCGCGPIO_R |= 0x00000004; // activate port C

    UART1_CTL_R &= ~0x00000001; // disable UART

    UART1_IBRD_R = 104; // IBRD = int(80,000,000/(16*115,200)) = int(43.40278)

    UART1_FBRD_R = 11; // FBRD = round(0.40278 * 64) = 26

    UART1_LCRH_R = 0x00000070; // 8 bit, no parity bits, one stop, FIFOs

    UART1_CTL_R |= (UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE); // enable UART

    GPIO_PORTC_AFSEL_R |= 0x30; // enable alt funct on PC5-4

    GPIO_PORTC_DEN_R |= 0x30; // configure PC5-4 as UART1

    GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFF00FFFF) + 0x00220000;

    GPIO_PORTC_AMSEL_R &= ~0x30; // disable analog on PC5-4
}
void UART1_FIFO_FULL(char data)
{
    while ((UART1_FR_R & 0x0080) == 0)
        ;
    UART1_DR_R = data;
}
void UART3_FIFO_FULL(char data)
{
    while ((UART3_FR_R & 0x0080) == 0)
        ;
    UART3_DR_R = data;
}
char UART1_InChar(void)
{
    while ((UART1_FR_R & 0x10) != 0)
        ;
    return (char)(UART1_DR_R & 0XFF);
}
char UART3_InChar(void)
{
    while ((UART3_FR_R & 0x10) != 0)
        ;
    return (char)(UART3_DR_R & 0XFF);
}
void UART0_Init(void)
{
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;

    UART0_CTL_R &= ~UART_CTL_UARTEN;
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 11;
    //UART0_CC_R = 0;
    UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART0_CTL_R |= (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

    GPIO_PORTA_AFSEL_R |= 0x03;
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & ~0xFF) | (GPIO_PCTL_PA1_U0TX | GPIO_PCTL_PA0_U0RX);
    GPIO_PORTA_DEN_R |= 0x03;
    GPIO_PORTA_AMSEL_R &= ~0x03;
}
void UART0_Write(unsigned char data)
{
    while ((UART0_FR_R & UART_FR_TXFF) != 0)
        ;
    UART0_DR_R = data;
}
unsigned char UART0_ReadChar(void)
{ //Read Char
    while ((UART0_FR_R & RXEMPTY) != 0)
    {
    }
    return UART0_DR_R & 0xFF;
}
void UART0_ReadString(char *str, char stopCh)
{ //Read String
    char c = UART0_ReadChar();
    while (c && c != stopCh)
    {
        *str = c;
        str++;
        c = UART0_ReadChar();
    }
    *str = 0x00;
}

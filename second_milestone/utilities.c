void Delay(void)
{
    for (j = 0; j < DELAY_VALUE; j++)
        ;
}
void Delay_1(void)
{
    for (j = 0; j < DELAY_VALUE_1; j++)
        ;
}
void prstr(char str)
{
    while (str)
    {
        UART1_FIFO_FULL(*str);
        str++;
    }
}
void get_Command(char *Command, int lenmax)
{
    char character;
    int i;
    for (i = 0; i < lenmax; i++)
    {

        character = UART1_InChar();
        Command[i] = character;
        UART1_FIFO_FULL(Command[i]);
    }
}
void get_Command_3(char *Command, int lenmax)
{
    char character;
    int i;
    for (i = 0; i < lenmax; i++)
    {

        character = UART3_InChar();
        Command[i] = character;
        UART3_FIFO_FULL(Command[i]);
    }
}
double StringtoDoub(const char y)
{
    char *x;
    double vout = strtod(x, NULL);
    return vout;
}

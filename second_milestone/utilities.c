void SysTick_Init(void)
{

    NVIC_ST_CTRL_R = 0; // disable SysTick during setup

    NVIC_ST_CTRL_R = 0x00000005; // enable SysTick with core clock
}
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(unsigned long delay)
{

    NVIC_ST_RELOAD_R = delay - 1; // number of counts to wait

    NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears

    while ((NVIC_ST_CTRL_R & 0x00010000) == 0)
    { // wait for count flag
    }
}
char *getlon(char *a_tag_list)
{
    /* 'strtok' modifies the string. */
    char *tag_list_copy = malloc(strlen(a_tag_list) + 1);
    char *result = 0;
    char *s;

    strcpy(tag_list_copy, a_tag_list);
    s = strtok(tag_list_copy, ",");
    while (s)
    {
        if (0 == strcmp(s, "$GPGLL"))
        {
            s = strtok(0, ",");
            s = strtok(0, ",");
            result = malloc(21);
            strcpy(result, s);
            break;
        }
        else if (0 == strcmp(s, "$GPRMC"))
        {

            s = strtok(0, ",");
            s = strtok(0, ",");
            s = strtok(0, ",");
            result = malloc(21);
            strcpy(result, s);
        }

        s = strtok(0, ",");
    }
    free(tag_list_copy);

    return result;
}

char *getlat(char *a_tag_list)
{
    /* 'strtok' modifies the string. */
    char *tag_list_copy = malloc(strlen(a_tag_list) + 1);
    char *result = 0;
    char *s;

    strcpy(tag_list_copy, a_tag_list);

    s = strtok(tag_list_copy, ",");
    while (s)
    {
        if (0 == strcmp(s, "$GPGLL"))
        {
            s = strtok(0, ",");
            s = strtok(0, ",");
            s = strtok(0, ",");
            s = strtok(0, ",");
            result = malloc(21);
            strcpy(result, s);
        }
        else if (0 == strcmp(s, "$GPRMC"))
        {

            s = strtok(0, ",");
            s = strtok(0, ",");
            s = strtok(0, ",");
            s = strtok(0, ",");
            s = strtok(0, ",");
            result = malloc(21);
            strcpy(result, s);
        }

        s = strtok(0, ",");
    }
    free(tag_list_copy);

    return result;
}

// 800000*12.5ns equals 10ms

void SysTick_Wait10ms(unsigned long delay)
{

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

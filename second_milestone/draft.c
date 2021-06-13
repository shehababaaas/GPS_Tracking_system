
#include "tm4c123gh6pm.h"
#include <stdio.h>
#include "stdint.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#ifndef M_PI
#define M_PI 3.141592
#endif
#define DELAY_VALUE 10000
#define DELAY_VALUE_1 100000
#define TXFULL UART_FR_TXFF
#define RXEMPTY UART_FR_RXFE

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

    unsigned long i;

    for (i = 0; i < delay; i++)
    {

        SysTick_Wait(800000); // wait 10ms
    }
}
unsigned long j = 0;
// Delay function
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
void LCD_DATA(char data)
{
    GPIO_PORTA_DATA_R = 0x20;
    GPIO_PORTB_DATA_R = data;
    GPIO_PORTA_DATA_R |= 0x80;
    Delay_1();
    GPIO_PORTA_DATA_R = 0;
}
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
void prstr(char *str)
{
    while (*str)
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
//LCD COMMAND
void LCD_command(char command)
{
    GPIO_PORTA_DATA_R = 0;
    GPIO_PORTB_DATA_R = command;
    GPIO_PORTA_DATA_R |= 0x80;
    Delay_1();
    GPIO_PORTA_DATA_R = 0;
}
//initialization function
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
// function to get distance from GPS module
double StringtoDoub(const char y)
{
    char *x;
    double vout = strtod(x, NULL);
    return vout;
}
double getdistance(double lat_1, double long_1, double lat_2, double long_2)
{
    double R = 6373.0;
    double distance = 0;
    double d_lat = (lat_2 - lat_1) * M_PI / 180.0;
    double d_long = (long_2 - long_1) * M_PI / 180.0;
    double a = pow(sin(d_lat / 2), 2) + cos(lat_1 * M_PI / 180.0) * cos(lat_2 * M_PI / 180.0) * pow(sin(d_long / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    distance = R * c * 1000;
    // for validation:
    // printf("%.10f\n", a);
    // printf("%.10f\n", c);
    // printf("%.10f\n", distance);

    return distance;
}

// An indicator function which switch is on or off.
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

char a[4];
char lat[50];
char lon[50];
unsigned char btn;
unsigned char LED_out;
double total_distance = 0;
double distance;
char text[40];
char text_1[20];
double lat_gps;
double lon_gps;
double lat_long[100][2];
char d_lat[10];
char d_lon[10];
char m_lat[10];
char m_lon[10];

int main(void)
{
    int i = 0;
    int z = 1;

    //int j=0;
    //int j_1;
    const int len = 100;
    char command[len] = {0};
    //char *eptr;
    const char s[2] = "$";
    char *token_1;
    char *comma = 0;
    char *ret;
    char *ret_E;
    int start_point;
    int size = 0;
    UART0_Init();
    UART1_Init();
    SysTick_Init();
    init();
    init_SW1_SW2();
    init_LEDs();
    Port_A_init();

    lat_long[0][0] = 0.0;
    lat_long[0][1] = 0.0;

    // Loop forever .
    while (1)
    {
        get_Command(command, len);
        //	char str[] = "$GPRMC,235526.00,A,3002.44875,N,03115.12155,E,0.055,,110621,,,A*7F $GPGLL,3002.45013,N,03115.12653,E,193047.00,A,A*66 $GPGSV,4,3,15,13,15,307,22,14,57,185,,17,02,165,20,20,24,308,27*7D";
        /* get the first token */
        token_1 = strtok(command, s);

        /* walk through other tokens */
        while (token_1 != NULL)
        {
            for (i = 0; i < 5; i++)
            {
                a[i] = token_1[i];
            }
            if (strcmp(a, "GPGLL") == 0)
            {
                comma = malloc(strlen(token_1) + 1);
                strcpy(comma, token_1);
                ret = strchr(comma, 'N');
                ret_E = strchr(comma, 'E');
                size = (strlen(ret) - strlen(ret_E)) - 3;
                start_point = (strlen(token_1) - strlen(ret)) + 2;
                for (i = start_point; i < start_point + (size); i++)
                {
                    lon[i - start_point] = token_1[i];
                }
                size = ((strlen(token_1) - strlen(ret) - 6) - 1) + 5;
                for (i = 6; i <= (size); i++)
                {
                    lat[i - 6] = token_1[i];
                }
            }

            else if (strcmp(a, "GPRMC") == 0)
            {

                comma = malloc(strlen(token_1) + 1);
                strcpy(comma, token_1);
                ret = strchr(comma, 'N');
                ret_E = strchr(comma, 'E');

                size = (strlen(ret) - strlen(ret_E)) - 3;
                start_point = (strlen(token_1) - strlen(ret)) + 2;
                for (i = start_point; i < start_point + (size); i++)
                {
                    lon[i - start_point] = token_1[i];
                }
                size = (strlen(token_1) - strlen(ret)) - 19;
                for (i = 18; i < 18 + (size); i++)
                {
                    lat[i - 18] = token_1[i];
                }
            }

            token_1 = strtok(NULL, s);
        }

        strncpy(d_lat, lat, 2);
        strncpy(d_lon, lon, 3);
        j = 0;
        for (i = 2; i < (strlen(lat) + 1); i++)
        {
            m_lat[j] = lat[i];
            j++;
        }
        j = 0;
        for (i = 3; i < (strlen(lon) + 1); i++)
        {
            m_lon[j] = lat[i];
            j++;
        }

        //lat_gps = atof(lat)/100;
        //lon_gps = atof(lon)/100;
        lat_gps = atof(d_lat) + atof(m_lat) / 60;
        lon_gps = atof(d_lon) + atof(m_lon) / 60;
        if (lat_gps == 0 || lon_gps == 0)
        {
            continue;
        }
        //if((lat!=NULL && lon!=NULL)&& ((lat_gps==0.00)||(lon_gps==0.00)) ){continue ;}

        lat_long[z][0] = lat_gps;
        lat_long[z][1] = lon_gps;
        if (z == 1)
        {
            distance = 0;
        }
        else
        {
            distance = getdistance(lat_long[z - 1][0], lat_long[z - 1][1], lat_long[z][0], lat_long[z][1]);
        }
        total_distance += (distance);
        z++;

        //else if (strcmp(check,"$GPGGA")==0){
        //	if (command[i]== ','){
        //		countcomma++;
        //		}
        //		i1=0;
        //		if(countcomma==2){
        //			lon_1[i1++]=(command[i] != ',' ? command[i]:'0');
        //		}
        //		i1=0;
        //		if(countcomma==4){
        //		lat_1[i1++]=(command[i] != ',' ? command[i]:'0');
        //		}

        //	total_distance+=distance;

        //longitude=strtod(command,&eptr);
        //latitude=strtod(command,&eptr);

        //sprintf(text,"%f",longitude);
        //sprintf(text_1,"%f",latitude);
        sprintf(text, "%lf", distance);
        sprintf(text_1, "%lf", total_distance);
        //	total_distance+=distance;
        LCD_command(1);
        LCD_command(0x80);
        LCD_DATA('d');
        LCD_DATA('i');
        LCD_DATA('s');
        LCD_DATA('t');
        LCD_DATA('=');
        LCD_DATA(text[0]);
        LCD_DATA(text[1]);
        LCD_DATA(text[2]);
        LCD_DATA(text[3]);
        LCD_command(0xC0);
        LCD_DATA('t');
        LCD_DATA('o');
        LCD_DATA('t');
        LCD_DATA('=');
        LCD_DATA(text_1[0]);
        LCD_DATA(text_1[1]);
        LCD_DATA(text_1[2]);
        LCD_DATA(text_1[3]);
        // It will 20 times on seven segment as an assumed distance from virtual GPS module just for first milestone.

        //if it reached 100 meters the LED will be blue.

        if (total_distance >= 100)
        {
            LED_out = 0x04;
            RG_out(LED_out);
            // Segement will glow.
            while (i < total_distance)
            {
                i++;
                GPIO_PORTA_DATA_R |= 0x04;
                Delay();
                GPIO_PORTA_DATA_R &= ~0x04;
            }
            // to make it stop
            while (1)
            {
            }
        }

        memset(command, 0, len);
        free(comma);
    }
}

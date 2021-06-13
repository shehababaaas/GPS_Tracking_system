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

#include "distance.c"
#include "init.c"
#include "LCD.c"
#include "UART.c"
#include "utilities.c"

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
UART0_Init();
    UART1_Init();
    SysTick_Init();
    init();
    init_SW1_SW2();
    init_LEDs();
    Port_A_init();

    // Loop forever
    while (1)
    {
        get_Command(command, len);
        /* get the first token /
        token_1 = strtok(command, s);

        / walk through other tokens */
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

char m_lon[10];

int main(void)
{

    int i = 0;
    int z = 1;
    const int len = 100;
    char command[len] = {0};
    const char s[2] = "$";
    char *token_1;
    char *comma = 0;
    char *ret;
    char *ret_E;
    int start_point;
    int size = 0;
    lat_long[0][0] = 0.0;
    lat_long[0][1] = 0.0;

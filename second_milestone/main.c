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


        // extracting the latitude and longitude
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

        // calculating the distance (and storing it in an array) and total distance
        if (lat_gps == 0 || lon_gps == 0)
        {
            continue;
        }
        lat_long[z][0] = lat_gps;
        lat_long[z][1] = lon_gps;
        if (z == 1)
        {
            distance = 0;
        }
        else
        {
            distance = getdistance(lat_long[z - 1][0], lat_long[z - 1][1], lat_long[z][0], lat_long[z][1]);
             		// if (distance >=0.4){
			//	distance=abs(distance-0.4);
        //}
        total_distance += (distance);
        z++;

// printing on the LCD
        sprintf(text, "%lf", distance);
        sprintf(text_1, "%lf", total_distance);
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


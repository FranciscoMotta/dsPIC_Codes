/*
 * File:   main.c
 * Author: jmott
 *
 * Created on September 30, 2023, 2:06 PM
 */

/* includes */

#include "system_config.h"
#include <libpic30.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Macros */

#define Led_Sys_Pin     LATBbits.LATB3
#define Led_Sys_Tris    TRISBbits.TRISB3

#define Lcd_Puerto_Tris TRISB
#define Lcd_Puerto_Lat  LATB    
#define Lcd_Rs_Pin      LATBbits.LATB0
#define Lcd_Rw_Pin      LATBbits.LATB1
#define Lcd_En_Pin      LATBbits.LATB2

#define Modo_Comando()  Lcd_Rs_Pin = 0;
#define Modo_Caracter() Lcd_Rs_Pin = 1;

#define Enable_Toggle() Lcd_En_Pin = 1; __delay_ms(1); Lcd_En_Pin = 0;

/* Macros del Sensor HC-SR04 */

#define Ultrasound_Trigger_Pin   LATBbits.LATB9
#define Ultrasound_Trigger_Tris  TRISBbits.TRISB9

#define Ultrasound_Echo_Pin      PORTBbits.RB10
#define Ultrasound_Echo_Tris     TRISBbits.TRISB10

/* Declaracion de funciones */

void Init_Gpio(void);
void Send_Nibble(char byte);
void LCD_Send_Command(char command);
void LCD_Send_Character(char character);
void LCD_Init_Screen(void);
void LCD_Set_Cursor(uint8_t x, uint8_t y);
void LCD_Send_String(char *string);

void Init_Sensor_HC_SR04(void);

/* Variables */

char mensaje[16];

/* Uso del sprintf
 * #include <stdio.h>
    #include <math.h>

    int main () {
       char str[80];

       sprintf(str, "Value of Pi = %f", M_PI);
       puts(str);

       return(0);
    }
 *  */

/* Main */

int main(void) {
    /* Iniciamos el LCD */
    LCD_Init_Screen();
    LCD_Set_Cursor(0, 0);
    LCD_Send_String("Medidor :D");
    /* Iniciamos el sensor */
    Init_Sensor_HC_SR04();
    uint8_t cuenta = 0;
    uint16_t distancia = 0;
    /* Bucle principal */
    while (true) {
        Led_Sys_Pin = !Led_Sys_Pin;
        Ultrasound_Trigger_Pin = 1;
        __delay_us(10);
        Ultrasound_Trigger_Pin = 0;
        while (Ultrasound_Echo_Pin == 0);
        while (Ultrasound_Echo_Pin == 1) {
            __delay_us(58);
            distancia++;
        }
        LCD_Set_Cursor(0, 1);
        sprintf(mensaje, "dist = %3u cm", distancia);
        LCD_Send_String(mensaje);
        distancia = 0;
        __delay_ms(100);
    }
    return EXIT_SUCCESS;
}

/* Definicion de funciones */

void Init_Sensor_HC_SR04(void) {
    Ultrasound_Echo_Tris = 1; // El echo es entrada
    Ultrasound_Trigger_Tris = 0; // El trigger es salida
}

void LCD_Send_String(char *string) {
    uint8_t index = 0;
    while (string[index] != '\0') {
        LCD_Send_Character(string[index]);
        index++;
    }
}

void LCD_Set_Cursor(uint8_t x, uint8_t y) {
    char position = 0;
    if (y == 0) {
        position = 0x80; // Primera linea
    } else if (y == 1) {
        position = 0xC0; // Segunda linea
    } else {
        position = 0x80;
    }
    position += x;
    LCD_Send_Command(position);
}

void LCD_Init_Screen(void) {
    // Iniciamos el GPIO
    Init_Gpio();
    // Pin RW a 0
    Lcd_Rw_Pin = 0;
    // Mandar los comandos respectivos pag. 10
    LCD_Send_Command(0x01); // Limpiar la LCD
    LCD_Send_Command(0x02); // Return Home
    LCD_Send_Command(0x06); // Incremendo derecha / no shift
    LCD_Send_Command(0x0F); // LCD ON / CURSOR ON / BLINK ON
    LCD_Send_Command(0x2C); // 4 bits / 2 lines / 5x11 dots
    LCD_Send_Command(0x02); // Return Home
}

void LCD_Send_Command(char command) {
    Modo_Comando();
    Send_Nibble(command);
}

void LCD_Send_Character(char character) {
    Modo_Caracter();
    Send_Nibble(character);
}

void Send_Nibble(char byte) {
    char upper_nibble, lower_nibble;
    upper_nibble = (byte & 0xF0);
    lower_nibble = (byte & 0x0F) << 4;

    /* Mandamos el nibble más alto */
    Lcd_Puerto_Lat |= upper_nibble;
    Enable_Toggle();
    Lcd_Puerto_Lat &= ~upper_nibble;

    /* Mandamos en nibble menos alto */
    Lcd_Puerto_Lat |= lower_nibble;
    Enable_Toggle();
    Lcd_Puerto_Lat &= ~lower_nibble;
}

void Init_Gpio(void) {
    /* Puerto B como digital*/
    ADPCFG = 0xFFFF;
    /* Iniciamos el LED del systema */
    Led_Sys_Tris = 0; // Led como salida
    /* Apagar el LED */
    Led_Sys_Pin = 0;
    /* Iniciamos el puerto del LCD */
    Lcd_Puerto_Tris = 0x0000;
    /* Apgamos el puerto */
    Lcd_Puerto_Lat = 0x0000;
}
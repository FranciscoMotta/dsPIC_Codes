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

/* Declaracion de funciones */

uint16_t num_dip[10] = {0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111 // 9
};

/* Main */

/* El presente código tiene conectado al puerto B un display
 * de 7 segmendos de cátodo común de la sigueinte manera
 * 
 * PUERTO   : RB0 RB1 RB2 .  .  . RB6
 * DISPLAY  :  a   b   c  .  .  . g*/

int main(void) {
    /* Configuramos los pines como digitales */
    ADPCFG = 0xFFFF;
    /* Puerto B como salida */
    TRISB = 0x0000;
    /* Puerto B apagado */
    LATB = 0x0000;
    /* Bucle principal */
    uint8_t index = 0;
    while (true) {
        /* Cuenta ascendente 0 - 9 */
        for (index = 0; index < 10; index++) {
            LATB = num_dip[index];
            __delay_ms(500);
        }
        /* Cuenta descendente 9 - 0 */
        for (index = 9; index > 0; index--) {
            LATB = num_dip[index];
            __delay_ms(500);
        }
    }
    return EXIT_SUCCESS;
}

/* Definicion de funciones */
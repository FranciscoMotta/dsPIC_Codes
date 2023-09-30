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

/* Main */

int main(void) 
{
    /* Pines digitales */
    ADPCFG = 0xFFFF;
    /* Pin RB0 como salida */
    TRISBbits.TRISB0 = 0;
    /* Pin RB0 apagado */
    LATBbits.LATB0 = 0;
    /* Bucle principal */
    while(true)
    {
        /* Hacemos parpader el Led en RB0 */
        LATBbits.LATB0 = !LATBbits.LATB0;
        __delay_ms(500);
    }
    return EXIT_SUCCESS;
}

/* Definicion de funciones */
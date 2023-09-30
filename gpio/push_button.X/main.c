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
    /* Pin RB1 como entrada */
    TRISBbits.TRISB1 = 1;
    /* Bucle principal */
    while(true)
    {
        /* Prender el LED RB0 cuando se pulsa el boton en RB1 */
        if(PORTBbits.RB1 == 1) // Se pregunta por el estado del boton
        {
            LATBbits.LATB0 = 1; // Encender LED
        }
        else
        {
            LATBbits.LATB0 = 0; // Apagar LED
        }
    }
    return EXIT_SUCCESS;
}

/* Definicion de funciones */

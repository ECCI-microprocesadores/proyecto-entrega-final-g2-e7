#include "adc.h"
#include <stdint.h>

#define _XTAL_FREQ 16000000UL   // Definir frecuencia para los retardos

void ADC_Init(void) {
    ADCON0 = 0x01;     // ADC encendido, canal AN0 seleccionado (CHS=0000)
    ADCON1 = 0x0E;     // Solo AN0 como analógico, resto digital
    ADCON2 = 0xA9;     // A/D clock Fosc/8, adquisición 4 TAD, justificado a la derecha
}

uint16_t ADC_Read(void) {
    ADCON0bits.CHS = 0b0000; // Selecciona canal AN0 (RA0)
    __delay_us(5);           // Tiempo para estabilizar la señal analógica

    ADCON0bits.GO = 1;       // Inicia la conversión ADC
    while (ADCON0bits.GO);   // Espera hasta que la conversión termine

    return ((uint16_t)(ADRESH << 8) | ADRESL); // Retorna valor ADC de 10 bits
}

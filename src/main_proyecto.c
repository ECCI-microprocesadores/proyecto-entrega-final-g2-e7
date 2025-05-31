#include <xc.h>
#include "adc.h"
#include "i2c.h"
#include "i2c_lcd.h"

#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 16000000UL

#define UMBRAL_ADC 61                      // Equivalente a ~30°C
#define VENTILADOR LATBbits.LATB2

void setup(void) {
    ADC_Init();
    I2C_init();
    lcd_init();

    TRISAbits.TRISA0 = 1;  // LM35 - Entrada analógica
    TRISAbits.TRISA1 = 1;  // PIR - Entrada digital
    ANSELAbits.ANSA0 = 1;  // RA0 como entrada analógica
    ANSELAbits.ANSA1 = 0;  // RA1 como digital

    TRISBbits.TRISB2 = 0;  // RB2 como salida (ventilador)
    ANSELBbits.ANSB2 = 0;  // RB2 como digital
    VENTILADOR = 0;        // Apagado inicialmente
}

void lcd_write_number(unsigned int number) {
    char d1 = (number / 100) % 10;
    char d2 = (number / 10) % 10;
    char d3 = number % 10;

    if (d1 > 0) lcd_write_char(d1 + '0');
    else lcd_write_char(' ');

    lcd_write_char(d2 + '0');
    lcd_write_char('.');
    lcd_write_char(d3 + '0');
    lcd_write_char(' ');
    lcd_write_char('C');
}

void main(void) {
    setup();

    while (1) {
        unsigned int adc_value = ADC_Read();           // Leer LM35 (RA0)
        unsigned char movimiento = PORTAbits.RA1;      // Leer PIR (RA1)

        // Estimación simple de temperatura para mostrar en LCD
        float temperatura = adc_value * 0.488f;         // 5V/1023 ? 4.88mV por unidad

        // Mostrar temperatura
        lcd_set_cursor(0, 0);
        lcd_write_string("Temp: ");
        lcd_write_number((unsigned int)(temperatura * 10));
        lcd_write_string("   ");

        // Lógica de control
        lcd_set_cursor(1, 0);
        lcd_write_string("Ventilador: ");

        if (adc_value > UMBRAL_ADC && movimiento == 1) {
            VENTILADOR = 1;
            lcd_write_string("ON ");
        } else {
            VENTILADOR = 0;
            lcd_write_string("OFF");
        }

        __delay_ms(10);  // Retardo para lectura estable y LCD
    }
}

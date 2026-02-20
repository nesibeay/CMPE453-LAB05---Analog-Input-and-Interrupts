// C++ code - Interrupt Lab
/* 
Mert Temür
Metehan Can
Nesibe Aydın
Kutay Becerir
Türkay Selim Delikanlı
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BUTTON_PIN  PD2
#define BUZZER_PIN  PD3
#define LED_PIN     PD4
#define SENSOR_CH   0

#define LIGHT_THRESHOLD 500

volatile uint8_t buttonPressed = 0;

ISR(INT0_vect) {
    buttonPressed = 1;
}

uint16_t adc_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

int main(void) {
    DDRD |= (1 << BUZZER_PIN) | (1 << LED_PIN);
    DDRD &= ~(1 << BUTTON_PIN);
    PORTD |= (1 << BUTTON_PIN);

    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);
    EIMSK |= (1 << INT0);

    sei();

    while (1) {
        uint16_t lightLevel = adc_read(SENSOR_CH);

        if (lightLevel > LIGHT_THRESHOLD) {
            PORTD |= (1 << LED_PIN);
        } else {
            PORTD &= ~(1 << LED_PIN);
        }

        if (buttonPressed) {
            PORTD |= (1 << BUZZER_PIN);
            _delay_ms(4000);
            PORTD &= ~(1 << BUZZER_PIN);
            buttonPressed = 0;
        }

        _delay_ms(100);
    }

    return 0;
}

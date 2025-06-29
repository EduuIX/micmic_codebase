/*
 * adc_uart_btn_int.c
 * ATmega328P @ 8 MHz
 *
 * Converte o ADC1 quando:
 *   • chegar pela UART a LETRA_CHAVE, ou
 *   • o botão (PD2, INT0) for pressionado.
 * Resultado é enviado pela UART.
 * — UART 2400 bps 8 N 1
 * — Interrupções: INT0, USART_RX, ADC
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

/* ---------- parâmetros ------------------------------------------------ */
#define BAUD          2400
#define UBRR_VALUE    (F_CPU/16/BAUD - 1)      /* 8 MHz ? 207          */

#define LETRA_CHAVE   'e'      /* 1ª letra do seu nome               */
#define USE_8_MSB     0        /* 1 = envia só 8 MSB (0-255)         */

#define BTN_BIT       PD2      /* botão em D2 / INT0                 */

/* ---------- UART (TX por polling, RX por interrupção) ----------------- */
static int uart_putchar(char c, FILE *stream)
{
    while (!(UCSR0A & (1 << UDRE0)));  /* espera DR vazio */
    UDR0 = c;
    return 0;
}
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static void uartInit(void)
{
    UBRR0  = UBRR_VALUE;
    UCSR0B = (1 << RXEN0)  | (1 << TXEN0)  | (1 << RXCIE0); /* RX, TX, IRQ RX */
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);                 /* 8 N 1          */
}

/* ---------- ADC (canal 1 = PC1 / ADC1) ------------------------------- */
static void adcInit(void)
{
    ADMUX  = (1 << REFS0) | (1 << MUX0);         /* AVcc, canal ADC1 */
    ADCSRA = (1 << ADEN)  | (1 << ADIE) |        /* liga ADC + IRQ   */
             (1 << ADPS2) | (1 << ADPS1);        /* presc 64 (125 k) */
    DIDR0  = (1 << ADC1D);                       /* desliga digital  */
}

/* ---------- Botão INT0 ------------------------------------------------ */
static void int0Init(void)
{
    DDRD  &= ~(1 << BTN_BIT);                    /* entrada          */
    PORTD |=  (1 << BTN_BIT);                    /* pull-up interno  */
    EICRA  =  (1 << ISC01);                      /* borda de descida */
    EIMSK  =  (1 << INT0);                       /* habilita INT0    */
}

/* ---------- ISRs ------------------------------------------------------ */
/* flag para evitar iniciar nova conversão enquanto ADC ocupado          */
volatile uint8_t adc_busy = 0;

ISR(INT0_vect)                 /* botão */
{
    if (!adc_busy) { ADCSRA |= (1 << ADSC); adc_busy = 1; }
}

ISR(USART_RX_vect)             /* letra pela UART */
{
    char c = UDR0;
    if (c == LETRA_CHAVE && !adc_busy) {
        ADCSRA |= (1 << ADSC);
        adc_busy = 1;
    }
}

ISR(ADC_vect)                  /* fim da conversão */
{
    uint16_t val = ADC;
#if USE_8_MSB
    uart_putchar('\r', stdout);                 /* limpa linha opc. */
    uart_putchar('\n', stdout);
    printf("%03u\r\n", val >> 2);
#else
    printf("ADC=%4u  Tensao=%1.3f V\r\n",
           val, val * (5.0 / 1024.0));
#endif
    adc_busy = 0;
}

/* ---------- MAIN ------------------------------------------------------ */
int main(void)
{
    uartInit();
    adcInit();
    int0Init();
    stdout = &mystdout;

    sei();                          /* habilita interrupções globais */

    /* Loop ocioso: MCU fica livre; tudo acontece por IRQs */
    for (;;)
        ;                           /* pode colocar SLEEP aqui se quiser */
}

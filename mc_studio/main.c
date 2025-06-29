/*
 * adc_converte_tensao.c
 */
#define F_CPU 8000000UL //frequencia do microcontrolador
#include <stdio.h>      //uso printf
#include <avr/io.h>     //definicoes do componente
#include <util/delay.h> //uso do delay

void uartInit (void)               //inicializa uart
{ UBRR0  = F_CPU/16/9600-1;        //ajusta a taxa de transmissao
  UCSR0A = 0;                      //valor padrao
  UCSR0B = (1<<RXEN0) |(1<<TXEN0); //habilita a transmissao e recepcao. Sem interrupcao
  UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);//assincrono, 8 bits, 1 bit de parada, sem paridade
}

//configura o conversor A/D
void adcInit()
{ ADCSRA = 0b10000110; //habilita ADC. Prescaler 64. 0b10001110=>interrupcao
  ADCSRB = 0b00000000; //valor padrao
  DIDR0  = 0b00000001; //habilita pino PC0 (ADC0) como entrada analogica
  ADMUX  = 0b01000000; //referencia AVcc. Alinhamento a direita. Canal ADC0
}

static int put_char (char c,FILE *stream) //direciona string para UART
{ while(!(UCSR0A & (1<<UDRE0)));          //aguarda ultimo dado ser enviado
  UDR0 = c;                               //envia caracter
  return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(put_char,NULL,_FDEV_SETUP_WRITE);

int main(void)
{ uartInit();                       //inicializa UART
  adcInit();                        //inicializa Conversor AD
  stdout= &mystdout;                //direciona uart como saida padrao
  uint16_t valorADC;                //variavel que armazena o valor convertido
  float    tensao;                  //tensao lida
  while (1)                         //laco infinito
  { ADCSRA |= 1<<ADSC;              //Inicia conversao (ADSC = 1)
    while(ADCSRA & (1<<ADSC));      //aguarda fim da conversao (ADSC = 0)
    valorADC = ADC;                 //retorna o valor do A/D
    tensao = valorADC*(5.0/1024.0); //valor da tensao em Volts
    printf("ADC = %3d - Tensao = %7.5f\n",valorADC,tensao);//mostra valor lido
    _delay_ms(1000);                //aguarda
  }
}
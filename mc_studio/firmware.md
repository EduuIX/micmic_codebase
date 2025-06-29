## DOCUMENTAÇÃO DO CÓDIGO: adc_uart_btn_int.c


## Resumo:
Este código para o microcontrolador ATmega328P realiza leituras analógicas no canal ADC1 (PC1),
ativando a conversão por dois eventos: (1) pressionamento de um botão conectado a INT0 (PD2),
ou (2) recepção da letra 'e' via UART. A conversão é feita com interrupção, e o resultado é
enviado via UART em 2400 bps. A arquitetura do programa é completamente baseada em interrupções.

---

## Parâmetros Gerais:

- Clock do microcontrolador: 8 MHz
- UART: 2400 bps, 8 bits, sem paridade, 1 stop bit (8N1)
- Canal ADC: ADC1 (pino PC1)
- Conversão acionada por:
    * Botão físico (PD2/INT0)
    * Letra recebida pela UART ('e')
- Resultado: valor digital e tensão em volts via UART
- Comunicação serial: printf via redirect de stdout
---
## Módulos do Código:

### 1. Definições iniciais:
```bash
#define F_CPU        8000000UL
#define BAUD         2400
#define UBRR_VALUE   (F_CPU/16/BAUD - 1)  // UBRR = 207 para 2400 bps

#define LETRA_CHAVE  'e'        // Letra para acionar ADC via UART
#define USE_8_MSB     0         // 1 = envia só 8 MSB; 0 = valor completo
#define BTN_BIT      PD2        // Botão ligado ao pino PD2 (INT0)
```
---
### 2. UART:
- Função uart_putchar():
  Implementa a transmissão serial caractere por caractere, aguardando o registrador estar pronto.

- Objeto mystdout:
  Redireciona printf para uart_putchar usando FDEV_SETUP_STREAM.

- Função uartInit():
  Inicializa os registradores UBRR0, UCSR0B e UCSR0C para operação da UART com interrupção de recepção (RXCIE0).
---
### 3. ADC:
- Função adcInit():
  - Seleciona canal ADC1 (PC1)
  - Usa AVcc como referência
  - Prescaler = 64 → 125 kHz
  - Habilita interrupção do ADC
  - Desliga entrada digital de PC1 para economia de energia
---
### 4. Botão (INT0):
- Função int0Init():
  - Configura PD2 como entrada com pull-up interno
  - Configura interrupção externa INT0 para ativar na borda de descida
---
### 5. Variável Global:
```bash
volatile uint8_t adc_busy = 0;
```
→ Evita iniciar nova conversão enquanto o ADC está ocupado.
---

### 6. Interrupções:

- ISR(INT0_vect):
  - Acionada quando o botão é pressionado
  - Se o ADC não estiver ocupado, inicia a conversão e define adc_busy = 1

- ISR(USART_RX_vect):
  - Acionada quando um caractere chega pela UART
  - Se for a LETRA_CHAVE e o ADC estiver livre, inicia a conversão

- ISR(ADC_vect):
  - Acionada quando a conversão termina
  - Lê o valor convertido (ADC)
  - Se USE_8_MSB = 1: envia apenas os 8 MSB
  - Se USE_8_MSB = 0: envia o valor completo e a tensão em volts
  - Libera a flag adc_busy = 0
---

### 7. Função main():
- Inicializa UART, ADC e botão
- Redireciona stdout para a UART
- Habilita interrupções globais (sei())
- Entra em loop ocioso infinito (tudo é controlado por interrupção)

---

Exemplo de Saída via UART (modo USE_8_MSB = 0):
```bash
ADC= 512  Tensao=2.502 V
```
---

Considerações Finais:
- O código é eficiente em energia: não faz polling contínuo.
- Flexível para expansão: mais canais ADC, debounce no botão, comandos UART mais complexos.
- Ideal para ensino de interrupções, ADC e UART no AVR.

---

Requisitos:
- ATmega328P @ 8 MHz
- Terminal UART (SimulIDE ou físico)
- Potenciômetro no pino PC1
- Botão no pino PD2 (com GND)
- Compilador AVR-GCC e avrdude (para gravação)

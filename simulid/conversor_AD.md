# Documentação do Circuito SimulIDE – Leitura de Tensão via ADC com ATmega328P

Este circuito foi projetado no **SimulIDE** e simula a leitura analógica de tensão com um **ATmega328P**. A conversão é ativada por um **botão físico (INT0)** ou por recepção serial (**UART**) e o resultado é exibido em um terminal virtual.

---

## Componentes Principais

### 1. Microcontrolador: ATmega328P
- Frequência: **8 MHz**
- Programa carregado automaticamente: `GccApplication1.hex`
- Interações:
  - UART: TX (PD1), RX (PD0)
  - ADC: Canal 1 (PC1 / ADC1)
  - INT0: PD2 (botão físico)

### 2. Terminal Serial (SerialTerm)
- Posição: à esquerda do microcontrolador
- Baud rate: **2400 bps**, 8 bits, 1 stop bit (8N1)
- Comunicação direta com os pinos **PD0 (RX)** e **PD1 (TX)** do ATmega328P

### 3. Potenciômetro
- Resistência total: **10 kΩ**
- Valor inicial: **5 kΩ**
- Ligado entre alimentação (5V) e GND
- Saída central conectada ao **pino PC1 (ADC1)** do microcontrolador

### 4. Voltímetro
- Conectado à saída do potenciômetro para visualização da tensão aplicada ao canal analógico ADC1

### 5. Botão (Push)
- Conectado ao pino **PD2 (INT0)** do microcontrolador
- Acionado por borda de descida com **pull-up interno**
- Ligado a **GND** para acionamento

### 6. Capacitor (100 nF)
- Conectado entre **PORTV1** (linha de alimentação virtual do ATmega328P) e GND
- Serve como desacoplamento/bypass para estabilizar a linha de alimentação virtual

### 7. Fontes de alimentação (Rails)
- Dois rails de 5V:
  - Um ligado ao potenciômetro (alimentação)
  - Um ligado ao microcontrolador (linha de alimentação virtual)
- Três terras (GND):
  - Um para o potenciômetro
  - Um para o botão
  - Um para o capacitor

---

## Conexões do Circuito

### UART
- **PD0 (RX)** ⟷ Terminal Serial (TX)
- **PD1 (TX)** ⟷ Terminal Serial (RX)

### Potenciômetro
- Pino A: Rail 5V
- Pino M (meio): conectado ao ADC1 (PC1)
- Pino B: conectado ao GND

### Voltímetro
- Ligado entre a saída do potenciômetro (pino M) e GND
- Permite visualização da tensão lida pelo ADC

### Botão (INT0)
- Um lado ligado ao pino **PD2**
- Outro lado ao **GND**
- Pino PD2 configurado com pull-up interno no código

### Capacitor
- Entre linha de alimentação virtual (PORTV1) e GND
- Valor: 100 nF

### Linhas Virtuais do ATmega328P
- PORTV0: Conectado à alimentação via Rail 5V
- PORTV1: Conectado ao capacitor e GND (estabilização)

---

## Interação Esperada

1. **Gire o potenciômetro** para variar a tensão na entrada analógica (PC1).
2. **Pressione o botão** físico (INT0) ou **envie o caractere `'e'`** via terminal UART.
3. O microcontrolador realiza a leitura do ADC e **envia o valor via UART**.
4. O terminal serial exibirá a **tensão e valor digital** correspondente.

---

## Observações

- A tensão lida será aproximadamente entre **0V e 5V**, dependendo da posição do potenciômetro.
- O código utiliza **interrupções** para UART RX, ADC e INT0, otimizando o uso do microcontrolador.
- Ideal para fins educacionais e demonstração de uso eficiente do ADC com eventos externos.

---

## Requisitos

- **SimulIDE** versão 1.1.0 ou superior
- Arquivo `.hex` gerado pelo código correspondente (ex: `main.hex`)
- Carregar o arquivo `.sim1` para simulação no SimulIDE


---

## Referências

- [SimulIDE – Página Oficial](https://www.simulide.com/)
- [ATmega328P Datasheet – Microchip](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810.pdf)

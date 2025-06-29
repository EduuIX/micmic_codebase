# Leitura de Tensão com ADC via Interrupção no ATmega328P

Este projeto demonstra o uso do conversor analógico-digital (ADC) do **ATmega328P** acionado por **interrupções**, com resultados transmitidos via **UART**. A leitura é iniciada **quando o botão físico (INT0) é pressionado ou quando um caractere específico é recebido via UART**. A simulação pode ser realizada no **SimulIDE**.

---

## Estrutura do Repositório

```
.
├── mc_studio/
│ └── main.c # Código-fonte em C para ADC com UART e interrupções
└── simulid/
└── conversor_AD.sim1 # Arquivo de simulação do circuito no SimulIDE
```


---

## ⚙️ Funcionalidades

- Leitura do sinal analógico no canal **ADC1 (PC1)** do ATmega328P.
- Conversão iniciada por:
  - Pressionamento do botão conectado ao pino **PD2 (INT0)**.
  - Recebimento da letra **'e'** via UART.
- Resultado exibido via UART:
  - Valor digital bruto.
  - Tensão estimada em volts.
- UART configurada para **2400 bps**, 8N1.
- Implementação inteiramente baseada em **interrupções**: INT0, USART_RX e ADC.

---

## 🧠 Como funciona o código (`main.c`)

- **UART**: Transmissão por polling; recepção por interrupção.
- **ADC**:
  - Canal: ADC1 (PC1).
  - Referência: AVcc (5V).
  - Prescaler: 64 (para 125kHz).
  - Interrupção chamada ao fim da conversão.
- **Botão**: Usa interrupção externa INT0 (borda de descida).
- **Fluxo**:
  - Ao detectar o evento (botão ou caractere), inicia a conversão.
  - Ao finalizar, calcula e imprime a tensão via `printf`.

---

## 🧪 Simulação no SimulIDE

### Requisitos

- [SimulIDE 1.1.0 ou superior](https://www.simulide.com/)

### Componentes utilizados

- ATmega328P @ 8 MHz
- Potenciômetro de 10kΩ conectado ao PC1
- Terminal serial virtual
- Botão ligado ao PD2 (INT0)
- Voltímetro para verificar tensão analógica
- Fontes de alimentação (5V e GND)

### Execução

1. Abra `adc.sim1` no SimulIDE.
2. Verifique se o microcontrolador está configurado para carregar o `.hex` correto.
3. Gire o potenciômetro para variar a tensão de entrada.
4. Pressione o botão físico ou envie o caractere `'e'` via terminal UART.
5. Veja a leitura da tensão sendo exibida no terminal serial.

---
# Projeto_Sirene_Escolar_ESP8266
Automatizar a Sirene da Escola com o chip ESP8266

## Objetivo do Projeto
- Automatizar a companhia da escola com os materiais disponíveis na Robótica.

## Horários da campanhia da Escola
- Turno da manhã:
    - 7:00 (Início do turno)
    - 7:50 (Segundo horário)
    - 8:40 (Terceiro horário)
    - 9:00 (Início do Recreio dos 6º e 7º anos)
    - 9:15 (Término do Recreio dos 6º e 7º anos)
    - 9:30 (Início do Recreio dos 8º e 9º anos)
    - 9:45 (Término do Recreio dos 8º e 9º anos e Quarto horário)
    - 10:35 (Quinto horário)
    - 11:25 (Sexto Horário)
    - 12:15 (Término do turno)
- Turno da Tarde
    - 13:00 (Início do turno)
    - 15:00 (Início do Recreio dos 1º e 2º anos)
    - 15:15 (Término do Recreio dos 1º e 2º anos)
    - 15:30 (Início do Recreio dos 3º e 4º anos)
    - 15:45 (Término do Recreio dos 3º e 4º anos)
    - 17:25 (Término do turno e Início do horário de reforço escolar)
    - 18:20 (Término de todas as atividades)

## Integrantes do Projeto
- Izabela
- Cauã
- Lucas
- Davi

## Ferramentas de desenvolvimento de Software
- Visual Studio Code
- GCC para o Windows. Para isso vamos usar o MSYS2 (Interface CLI para a instalação do MinGW no Windows)
- Makefile

### Pacotes e dependências
1. Instalar dependências básicas (Python e Make):
```bash
pacman -S make python python-pip mingw-w64-ucrt-x86_64-gcc
```
2. Instalar a ferramenta de gravação (esptool):
O ESP8266 não usa avrdude. Ele usa o esptool via Python para gravar o binário através da porta serial:
```pwsh
pip install esptool
```
3. Obter o Compilador Xtensa (xtensa-lx106-elf-gcc):
O projeto oficial [ESP-open-sdk](https://github.com/pfalcon/esp-open-sdk) ou os binários da própria Espressif fornecem o compilador. Você precisará baixar o toolchain do GCC para Xtensa LX106 e adicionar a pasta bin dele ao seu PATH no MSYS2.

## Ferramentas de Hardware
- Arduino
- ESP8266 mod
- Módulo Relé 5V

## 5. Montagem do Hardware no Protoboard
Como você mencionou "Arduino como base para o ESP8266", assume-se que você usará a placa do Arduino Uno apenas como um conversor USB-Serial para gravar o código no ESP8266 (já que o chip não tem USB nativo).
Conexões para colocar o ESP8266 em modo de gravação (Flash Mode):

* VCC do ESP8266 -> 3.3V do Arduino (Atenção: O ESP8266 queima com 5V e consome muita corrente, se ele resetar sozinho, precisará de uma fonte externa de 3.3V).
* GND do ESP8266 -> GND do Arduino (Unificar as massas).
* TX do ESP8266 -> TX do Arduino.
* RX do ESP8266 -> RX do Arduino (Use um divisor de tensão com resistores para abaixar os 5V do TX do Arduino para 3.3V no RX do ESP).
* CH_PD (ou EN) do ESP8266 -> Ligar no 3.3V (Ativa o chip).
* GPIO 0 do ESP8266 -> Ligar no GND (Isso avisa o chip que ele deve aceitar um código novo via Make/Esptool no momento da inicialização).
* RESET do Arduino -> Ligar no GND do Arduino (Isso desativa o chip principal do Arduino, transformando a placa em um gravador USB puro).

Quando terminar de gravar o código com o comando make flash, você deve desconectar o GPIO 0 do GND para que o chip execute o seu código em vez de esperar uma gravação.
Se você quiser avançar, posso te explicar como funciona o mecanismo de interrupção por Timer no ESP8266 em baixo nível para você criar a contagem de tempo sem travar o processador, ou ajudar a encontrar os arquivos de script do Linker (.ld) necessários para o Make. O que prefere ajustar primeiro?

# Entenda o Universo da Programação Embarcada
Dentro do Universo  dos chips (semicondutores) a melhor abordagem para poder entendê-los é dividindo-os por sua:
- capacidade computacional
- arquitetura de sistema
- aplicação prática

## 1. MCU *Microcontroller Unit*
Contém tudo em um único chip:
- processador
- memória RAM
- memória Flash

Possuem baixo consumo, porém são mais lentos. São ideais para tarefas simples e dedicadas (como o chip de um controle remoto)

Exemplos:
- AVR (Arduino com ATmega328)
- ESP8266, ESP32

## 2. MPU *Microprocessor Unit*
Contém apenas o processador. Exige uma memória RAM e Flah externas. Muito mais rápido, consome mais energia e roda Sistemas Operacionais robutos (como Windows, Linux e Android).

Exemplos:
- Intel x86-64
- Arm 
- AMD x86-64

## 3. do Hardware ao Software
Para conectar o Software com o Hardware é preciso entender como o código reconhece o Hardware a faz o trabalho de busca e execução diretamente nos registradores.

### Ciclo de Instrução
O ciclo baseia-se em um registrador interno essencial: o **PC** *Program Counter*, que guarda o endereço da próxima instrução a ser executada.

- **Passo 1**:  Energização/Reset

O processador envia o endereço co
ntido no **PC** através do barramento de endereços e lê a instrução que está na memória d programa

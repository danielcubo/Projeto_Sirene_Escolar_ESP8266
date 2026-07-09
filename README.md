# `1.` Automatizar a Sirene de Horários de uma Escola
Automatizar a Sirene da Escola com o chip ESP8266

## `1.1` Objetivo do Projeto
- Aproveitar o Laboratório de Robótica da Escola, para ensinar os alunos como automatizar a companhia da escola com os materiais disponíveis no laboratório.

## `1.2` Horários da campanhia da Escola
- **Turno da manhã**:
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
- **Turno da Tarde**
    - 13:00 (Início do turno)
    - 15:00 (Início do Recreio dos 1º e 2º anos)
    - 15:15 (Término do Recreio dos 1º e 2º anos)
    - 15:30 (Início do Recreio dos 3º e 4º anos)
    - 15:45 (Término do Recreio dos 3º e 4º anos)
    - 17:25 (Término do turno e Início do horário de reforço escolar)
    - 18:20 (Término de todas as atividades)

## `1.3` Integrantes do Projeto
- **Orientador(a):** Júnea
- **Suporte Técnico:** Daniel
- **Aluno(a):** Izabela
- **Aluno(a)**: Cauã 
- **Aluno(a):** Lucas
- **Aluno(a):** Davi

## `1.4` Ferramentas para o desenvolvimento do código
- Windows 11 Pro (Como os alunos estão acostumados com Windows, preferi continuar com o Windows. Mas é um preferível uma distribuição Linux.)
- Visual Studio Code
- GCC para o Windows. Para isso vamos usar o MSYS2 (Interface CLI para a instalação do MinGW no Windows)
- Makefile

### `1.4.1` Pacotes e dependências
1. Instalar dependências básicas no MSYS2 (Python e Make):
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

### `1.4.2` Compilação
```bash
make
```
### `1.4.3` Gravação
```bash
make flash
```
## `1.5` Componentes de Hardware que usaremos
1. Jumpers para conexão
2. PCB NodeMCU V2 (contém o chip ESP8266)
3. Módulo Relé 1 Canal de 5V e 10A
4. Caixa Plástica Organizadora para abrigar o chip ESP8266 e o Relé.

### `1.5.1` Convenção de cores dos jumpers:
Existem convenções e normas importantes para garantir a segurança e facilitar a manutenção do projeto, separadas entre a parte de **baixa tensão** (eletrônica) e **alta tensão** (rede elétrica)

#### `1.5.1.1` **DC** $\rightarrow$ Conexões de Baixa Tensão (do ESP8266 para o Módulo Relé)
Na eletrônica, os elétrons fluem em uma única direção. Esse fluxo é conhecido como **DC (Direct Current *Corrente Contínua*)**. É a energia "estável" usada por praticamente todos os dispositivos eletrônicos.
- **Vermelho**: Alimentação Positiva 5V (**VIN**)
- **Preto**: Terra (**GND**)
- **Laranja**: Alimentação Positiva 3.3V (específico para diferenciar de 5V).
- **Amarelo ou Branco**: Sinal (**IN**) que sai do GPIO do ESP8266 e vai para o Relé.
- **Azul ou Verde**: Outras opções comuns para sinais de dados ou funções especiais.
#### `1.5.1.2` **AC** $\rightarrow$ Conexões de Alta Tensão (do Relé para a Tomada)
Na Rede elétrica, a direção do fluxo dos elétrons se inverte periodicamente de forma cíclica (no Brasil, isso ocorre 60 vezes por segundo, ou 60 Hz). Esse tipo de fluxo é conhecido como **AC (Alternating Current *Corrente Alternada*)**.

Ao lidar com a fiação da tomada (AC), deve-se seguir a norma brasileira **NBR 5410**, que é obrigatória para o padrão e segurança elétrica das instalações.
- **Azul Claro**: Condutor **Neutro** (É como se fosse o negativo em DC)
- **Verde** ou **Verde com Amarelo**: Condutor de Proteção (**Terra**)
- **Vermelho, Preto ou Marrom**: Condutor **Fase** (É por onde passa a corrente, ou seja, é como se fosse o positivo em DC)

### `1.5.2` PCB NodeMCU V2
![PCB NodeMCU V2](/assets/nodemcu-esp8266.jpeg)
A **PCB** (Printed Circuit Board ou *Placa de Circuito Impresso*) é a placa física de cor preta ou azul onde todos os componentes estão soldados.

A PCB do NodeMCU serve como uma "ponte de acesso". Ela possui vários componentes que servem de entrada física para manipular o chip ESP8266.

#### 1.1 O chip ESP8266

Dentro do módulo metálico está o chip real e um chip de memória Flash externa. A capa de metal serve como blindagem eletromagnética (Gaiola de Faraday). Ela impede que ondas de rádio externas (como o sinal do seu celular) interfiram no Wi-Fi do chip, e garante as certificações de segurança internacionais.

Se você arrancasse a capinha de metal, veria uma patilha preta de silício bem pequena. Esse é o microcontrolador **ESP8266** criado pela **Espressif Systems**.

Ele é o cérebro que integra tudo. Dentro dele estão o modem de rádio Wi-Fi, os controladores dos pinos de entrada e saída (GPIO), as interfaces de comunicação (UART, SPI) e a memória RAM de execução.

Dentro do chip **ESP8266**, existe um bloco específico responsável apenas por fazer contas matemáticas e executar linhas de código: a **CPU** do chip. A Espressif batizou essa CPU de **Xtensa L106** (licenciado da empresa *Tensilica*).

Caraterísticas do chip ESP8266:

- **Capacidade Computacional**: MCU, ou seja, contém tudo em um único chip.
    - **CPU**: Xtensa L106
    - **RAM**
    - memória **Flash**
- **Filosofia**: RISC
- **ISA**: Xtensa (*Tensilica*). [clique aqui](https://dl.espressif.com/github_assets/espressif/xtensa-isa-doc/releases/download/latest/Xtensa.pdf)
- **Registradores**: 32 bits.

#### 1.2 Componentes de Comunicação
- **Porta Micro-USB**: Serve para transferir o código compilado para o chip. Também serve de alimentação (5V).
- **Chip Conversor USB-Serial (Geralmente o CP2102 ou CH340)**: É um chip retangular bem pequeno, perto da entrada USB. Ele traduz os dados que vêm do cabo USB (do seu VS Code/MSYS2) para o protocolo serial assíncrono **UART *Universal Asynchronous Receiver/Transmitter*** nos pinos de trasmissão (`TX`) e recepção (`RX`) do ESP8266.
- **Circuito de Auto-Reset/Flash**: Composto por dois transistores minúsculos próximos ao chip serial. Eles monitoram as linhas de cotrole `DTR` e `RTS` da USB. Quando você digita `make flash` no terminal do computador, depois de ter compilado o código, este circuito "aperta" eletronicamente o pino RESET e joga o `GPIO 0` para o terra (`GND`), forçando o ESP8266 a entrar em modo de gravação sem que você precise tocar na placa.

> **Nota**: A porta micro-USB do chip se conecta com o Sistema Operacional com drivers:
- **No Windows**
    - `COM3`, `COM4`, e assim por diante. As portas `COM1`, e `COM2` pertencem ao hardware interno do Computador. Então o Windows criará dinamicamente porta com números mais alto.
    - Abra o `Gerenciador de Dispositivos` e expanda a aba `Portas (COM e LPT)`. Você verá algo escrito como: `USB-SERIAL CH340 (COM4)`. Esse `COM4` é o que você vai digitar na variável `PORT` do seu Makefile.
- **No Linux**
    - No Debian, o driver do `CH340` já vem **compilado direto dentro do Kernel Linux**. Você não precisa baixar ou instalar nada. Basca plugar a PCB do ESP8266 no computador.
    - **Como descobrir o nome no Debian**: Abra o terminal e digite o comando `ls /dev/ttyUSB*`. O terminal vai responder algo como: `/dev/ttyUSB0`.
#### 1.3 Componentes de Alimentação e Controle
- **Regulador de Tensão de 3.3V (AMS1117-3.3)**: O ESP8266 queima se receber os 5V diretos da USB. Este componente de 3 pernas reduz os 5V da USB de forma estável para 3.3V, que é a tensão de operação do chip e do módulo.
- **Botão RST(Reset)**: Reinicia o microcontrolador manualmente. Ele joga o pino `RST` temporariamente para o `GND`. Ao soltar, o chip executa a função `call_rst()` no nosso código C.
- **Botão Flash (ou USER)**: Conectado ao `GPID 0`. Serve para forçar o modo de gravação manualmente caso o circuito automático falhe (basta segurar o botão `FLASH`, dar um clique no botão `RST` e soltar o `FLASH`).
- **LED Integrado (LED_BUILTIN)**: A placa possui um pequeno LED azul soldado perto do pino D4 (conectado ao `GPIO 2`). Ele é útil para testar o código: se você enviar tensão para o `GPIO 2`, o LED acende ou apaga, dependendo da PCB.
#### 1.4 Mapeamento dos Pinos da PCB (Pin-Out)
**GPIO** General Purpose Input/Output ou *Entrada e Saída de Propósito Geral*

Na PCB você verá pinos com nome `D0`, `D1`, etc... Esses nomes são "apelidos", ou seja, nomes mais amigáveis dos pinos para facilitar a identificação dos pinos na placa. Para a CPU do ESP8266, os pinos são conhecidos como `GPIO 0`, `GPIO 16`, etc. Então vamos mapear para que serve cada pino da PCB do NodeMCU V2.

<table>
    <tr>
        <th>Pino (PCB)</th>
        <th>Função/GPIO</th>
        <th>Uso</th>
    </tr>
    <tr>
        <td>3V3</td>
        <td>Saída de 3.3V</td>
        <td>Alimentar a parte lógica do Módulo Relé</td>
    </tr>
    <tr>
        <td>VIN</td>
        <td>Entrada de 5V</td>
        <td>Pino direto da USB. Pode ser usado para alimentar relés de 5V se necessário.</td>
    </tr>
    <tr>
        <td>GND</td>
        <td>Terra (0V)</td>
        <td>Conetar o GND do Relé e na malha de aterramento</td>
    </tr>
    <tr>
        <td>D0</td>
        <td>GPIO 16</td>
        <td>Conectado ao pino Deep Sleep. Não suporta interrupções de hardware.</td>
    </tr>
    <tr>
        <td>D1</td>
        <td>GPIO 5</td>
        <td>Pino de propósito geral. Ótima opção para o Relé.</td>
    </tr>
    <tr>
        <td>D2</td>
        <td>GPIO 4</td>
        <td>Pino de propósito geral. Outra ótima opção para o Relé</td>
    </tr>
    <tr>
        <td>D3</td>
        <td>GPIO 0</td>
        <td>Pino crítico de Boot. Não ligue o relé aqui, ou a placa não vai ligar após um reset.</td>
    </tr>
    <tr>
        <td>D4</td>
        <td>GPIO 2</td>
        <td>Conectado ao LED azul da placa. Se o chip der boot com este pino em nível baixo (0V), ele trava.</td>
    </tr>
    <tr>
        <td>D5</td>
        <td>GPIO 14</td>
        <td>Pino do barramento SPI (Clock)</td>
    </tr>
    <tr>
        <td>D6</td>
        <td>GPIO 12</td>
        <td>Pino do barramento SPI (MISO)</td>
    </tr>
    <tr>
        <td>D7</td>
        <td>GPIO 13</td>
        <td>Pino do barramento SPI (MOSI)</td>
    </tr>
    <tr>
        <td>D8</td>
        <td>GPIO 15</td>
        <td>Deve iniciar em nível baixo (0V) no boot. Evite usar para o relé.</td>
    </tr>
    <tr>
        <td>RX</td>
        <td>GPIO 3</td>
        <td>Pino de recepção da Serial (UART0). Usado pelo script do Windows para enviar a hora.</td>
    </tr>
    <tr>
        <td>TX</td>
        <td>GPIO 1</td>
        <td>Pino de transmissão da Serial (UART0). Usado para mandar logs para o Computador.</td>
    </tr>
    <tr>
        <td>A0</td>
        <td>ADC 0</td>
        <td>(Analog to Digital Converter - Conversor Analógico-Digital). É o único pino do chip projetado para ler sinais de tensão variáveis, entre 0V e 1V no chip, ou entre 0V 3.3V na placa (como os gerados por sensores de temperatura, luz ou potenciômetros) e convertê-los em valores digitais que o microcontrolador consegue entender.</td>
    </tr>
</table>


### `1.5.3` Módulo Relé 5V
![Relé 5V](/assets/modulo-rele-5V.jpg)

## `1.6` Montagem do Hardware no Protoboard

Vamos dividir o processo de montagem em 2 etapas:
- Simulação na Protoboard
- Instalação na Tomada

Para garantir o sucesso e, acima de tudo, a sua segurança, o seu plano de dividir o projeto em duas etapas (uma simulação de baixa tensão e depois a instalação real) é perfeito. Como você vai lidar com a rede elétrica da escola (127V ou 220V), todo cuidado é crucial para evitar curtos-circuitos e choques elétricos.

------------------------------
## ETAPA 1: A Simulação Segura na Protoboard (Com LED)
Nesta etapa, o computador alimenta o NodeMCU via cabo micro-USB. Vamos fazer o chip acionar o relé, e o relé vai acender um LED comum na protoboard, simulando a campainha.
## Ligações do NodeMCU para o Módulo Relé:

   1. Conecte o pino GND do NodeMCU ao pino GND (ou -) do Relé.
   2. Conecte o pino 3V3 do NodeMCU ao pino VCC (ou +) do Relé.
   3. Conecte o pino D1 (que é o GPIO 5 no seu código C) ao pino IN (Sinal) do Relé.

## Ligações do Relé para o LED (Simulando o Interruptor):
Os módulos relé possuem 3 bornes de parafuso: NO (Normally Open / Normalmente Aberto), COM (Comum) e NC (Normally Closed / Normalmente Fechado). [1] 

   1. Espete um resistor de 220 Ohms na protoboard, ligando o pino 3V3 do NodeMCU até a perna mais longa (Anodo/Positivo) do LED.
   2. Conecte um jumper do pino COM (Comum) do relé até a perna mais curta (Catodo/Negativo) do LED.
   3. Conecte outro jumper do pino NO (Normalmente Aberto) do relé até o pino GND do NodeMCU. [2] 

Como funciona a simulação: Quando o código C der o comando para ligar o GPIO 5, o relé vai "estalar" e fechar o contato interno entre o COM e o NO. Isso vai fechar o circuito do terra (GND), e o LED vai acender. Quando o chip desligar o pino, o LED apaga.
------------------------------
## ETAPA 2: A Instalação Real na Tomada/Parede
Após testar tudo com o LED e ver que o tempo e os horários estão batendo com o script do Windows 11, você vai levar o circuito para a parede.
## ⚠️ REGRAS DE SEGURANÇA OBRIGATÓRIAS:

* Desligue o disjuntor geral ou o disjuntor do circuito da campainha antes de abrir qualquer interruptor na parede.
* Use uma chave de teste ou multímetro para garantir que não há energia passando pelos fios.

## Como ligar o Relé no lugar do Interruptor:
Um interruptor de parede nada mais é do que uma ponte que interrompe o fio Fase (Positivo). Quando você abrir a caixinha na parede, verá dois fios conectados ao interruptor antigo.

   1. Solte os dois fios que estavam parafusados no interruptor de parede.
   2. Descarne um pequeno pedaço de cada um.
   3. Parafuse um desses fios no borne COM (Comum) do relé.
   4. Parafuse o outro fio no borne NO (Normalmente Aberto) do relé.

(Isso faz com que o relé assuma exatamente o papel do interruptor físico. Quando o chip acionar o relé, ele une esses dois fios eletronicamente, fazendo a campainha tocar).
## Como alimentar o NodeMCU na parede?
O chip precisa continuar ligado para processar o código e ouvir o computador. Como ele não pode ser alimentado pelos fios da campainha de alta tensão:

   1. Deixe o cabo micro-USB conectado no NodeMCU.
   2. Plugue a outra ponta do cabo USB em um carregador de celular comum (5V) e ligue esse carregador em uma tomada próxima.
   3. Se o computador que envia as atualizações de hora via PowerShell estiver longe, você precisará passar um cabo USB longo ou instalar o computador perto desse ponto.

------------------------------
## O Ajuste no Código C Bare-Metal
Como alteramos a recomendação técnica de pinos para o GPIO 5 (Pino D1 da placa) para evitar que o relé interfira na inicialização do chip, lembre-se de atualizar o topo do seu arquivo main.c:

#define REG_BASE_GPIO      0x60000300#define GPIO_ENABLE        *(volatile unsigned int *)(REG_BASE_GPIO + 0x0C)#define GPIO_OUT_SET       *(volatile unsigned int *)(REG_BASE_GPIO + 0x04)#define GPIO_OUT_CLEAR     *(volatile unsigned int *)(REG_BASE_GPIO + 0x08)
// Mudança para o GPIO 5 (Pino D1 do NodeMCU)#define RELAY_PIN          5  

Se você quiser finalizar a preparação do ambiente, prefere que eu te mostre como estruturar os comandos finais de compilação no VS Code ou quer entender melhor como garantir que o módulo relé que você comprou usa lógica invertida ou direta?

[1] [https://www.usinainfo.com.br](https://www.usinainfo.com.br/blog/projetos-com-arduino-nano-como-ligar-lampada-com-arduino/)
[2] [https://aluno.escoladigital.pr.gov.br](https://aluno.escoladigital.pr.gov.br/sites/alunos/arquivos_restritos/files/documento/2021-05/aula_13_push_button.pdf)





* VCC do ESP8266 $\rightarrow$ 3.3V do Arduino (Atenção: O ESP8266 queima com 5V e consome muita corrente, se ele resetar sozinho, precisará de uma fonte externa de 3.3V).
* GND do ESP8266 -> GND do Arduino (Unificar as massas).
* TX do ESP8266 -> TX do Arduino.
* RX do ESP8266 -> RX do Arduino (Use um divisor de tensão com resistores para abaixar os 5V do TX do Arduino para 3.3V no RX do ESP).
* CH_PD (ou EN) do ESP8266 -> Ligar no 3.3V (Ativa o chip).
* GPIO 0 do ESP8266 -> Ligar no GND (Isso avisa o chip que ele deve aceitar um código novo via Make/Esptool no momento da inicialização).
* RESET do Arduino -> Ligar no GND do Arduino (Isso desativa o chip principal do Arduino, transformando a placa em um gravador USB puro).

Quando terminar de gravar o código com o comando make flash, você deve desconectar o GPIO 0 do GND para que o chip execute o seu código em vez de esperar uma gravação.
Se você quiser avançar, posso te explicar como funciona o mecanismo de interrupção por Timer no ESP8266 em baixo nível para você criar a contagem de tempo sem travar o processador, ou ajudar a encontrar os arquivos de script do Linker (.ld) necessários para o Make. O que prefere ajustar primeiro?

# `2.` Entenda o Universo do Chip
A palavra "chip" vem do inglês arcaico *cipp*, que significa "**lasca**" ou "**fragmento**". O termo foi adotado de forma metafórica para descrever os minúsculos pedaços de silícios que contém ciruitos eletrônicos.

É comum confundir os termos "Circuitos Integrados" e "Chips".
O nome técnico que se dá a um conjunto de circuitos eletrônicos complexo (como transistores, resistores e capacitores) é **IC (Integrated Circuit ou *Circuito Integrado*)**.

**Chip** é o termo coloquial e físico. Refere-se especificamente à pequena lasca de silício que fica dentro da cápsula protetora do Circuito Integrado.

Dentro do Universo  dos chips (semicondutores) a melhor abordagem para poder entendê-los é dividindo-os por categoria:
## `2.1` Capacidade Computacional
### `2.1.1` MCU *Microcontroller Unit*
Contém tudo em um único chip:
- processador
- memória RAM
- memória Flash

Possuem baixo consumo, porém são mais lentos. São ideais para tarefas simples e dedicadas (como o chip de um controle remoto)

Exemplos:
- AVR (Arduino com ATmega328)
- ESP8266, ESP32

### `2.1.2` MPU *Microprocessor Unit*
Contém apenas o processador. Exige uma memória RAM e Flah externas. Muito mais rápido, consome mais energia e roda Sistemas Operacionais robutos (como Windows, Linux e Android).

Exemplos:
- Intel x86-64
- Arm 
- AMD x86-64
## `2.2` Filosofia de Design (RISC ou CISC)

**A ideia geral (a filosofia) de como as instruções foram desenhadas.**

Essa divisão define como as instruções da CPU (Central Processing Unit - *Unidade Central de Processamento*) foram desenhadas no silício.

### `2.2.1` RISC Reduced Instruction Set Computer
O processador entende apenas instruções simples e rápidas (geralmente executadas em 1 único ciclo de clock). Prioriza a eficiência de energia e velocidade de execução.

Ex:
- O chip ATmega328P que vem na PCB do Arduino UNO.
- o chip ESP8266 que vem na PCB NodeMCU V2

### `2.2.2` CISC Complex Instruction Set Computer
O processador entende instruções grandes e complexas. Ele tenta fazer muita coisa em uma única linha de código. Prioriza o poder bruto.

Ex:
- o chip Intel i5 do seu computador.

## `2.3` ISA (Instruction Set Architecture - *Arquitetura do Conjunto de Instruções*)
Essa divisão define como o Assembly conversa com a CPU. Cada CPU possui um DataSheet de instruções. Aqui é definido o propósito de cada registrador. Em outras palavras, cada chip possui um idioma, e para você poder programar aquele chip, você precisa conhecer o idioma do chip que está programando.

A ISA define os comandos (aritméticos, lógicos e de movimento de dados) que o processador entende.

### `2.3.1` x86
Os chips Intel e AMD possuem uma CPU com arquitetura x86-32 ou x86-64 (São chips de 32 ou 64 bits de banda de processamento). É o idioma dos computadores tradicionais. É um ecossistema focado em alto desempenho (baseado na filosofia CISC)

### `2.3.2` ARM
Os chips ARM possuem uma CPU com arquitetura própria. É o idioma licenciado pela empresa ARM, é usado pela Apple, Samsung, STMicroelectronics, etc. É focado em eficiência (baseado na filosofia **RISC**).

### `2.3.3` Xtensa
O chip ESP8266 possui uma CPU Xtensa L106, com a arquitura Xtensa. Licenciado pela *Tensilica*. É o idioma do chip ESP8266 (baseado na filosofia **RISC**)

### `2.3.4` AVR
O chip ATmega328P possui uma CPU com arquitetura AVR.
É o idioma do chip ATmega328P, da PCB do ARDUINO UNO (baseado na filosofia **RISC**)

## `2.4` Largura dos Registradores
Dizer que uma CPU possui registradores de 8, 16, 32 ou 64 bits, indica a capacidade de a CPU processar uma quantidades de bits de uma só vez. Isso indica a capacidade dos registradores, barramentos de memória e da **ALU (Arithmetic Logic Unit - *Unidade Lógica e Aritmética*)** de manipular sequências binárias, ou seja, é a capacidade que a CPU tem de processar dados ou buscar dados na memória de uma única vez.

Imagine que cada bit, dentro de uma sequência, representa uma "vaga" a ser preenchida, onde cada bit tem exatamente **2 opções** (`0` ou `1`).

A quantidade de bits representa uma sequência vagas (exemplo de um dado de 8 bits `00000000`). Onde, para cada tamanho de um dado binário, existe uma combinação possível na proporção de $2^{n}$. A primeira padronização de caracteres, conhecidade como padrão ASC-II, foi concebida usando uma combinação de 8 bits, já que os primeiros processadores processavam dados de 8 bits, ou seja, era possível até 256 caracteres usando dados de 8 bits.

Onde:
|$n \text{ bits}$ | Combinações possíveis | exemplo binário
|-|-|-
**8 bits** | $2^{8}\rightarrow 256$ | `00000000`
**16 bits** |$2^{16}\rightarrow 65.536$ | `0000000000000000`
**32 bits** |$2^{32}\rightarrow 4.294.967.296$ | `00000000000000000000000000000000`
**64 bits** |$2^{64}\rightarrow 18.446.744.073.709.551.616$ | `0000000000000000000000000000000000000000000000000000000000000000`

### `2.4.1` 8 bits
Processa dados binários de tamanho ($2^8=256$) por vez. Excelente para tarefas baratas e simples.

**Ex**: O chip ATmega328P/AVR (Arduino).

### `2.4.2` 16 bits
Processa dados binários de ($2^16=65536$) por vez.

### `2.4.3` 32 bits
Processa dados binários de ($2^32=4.294.967.296$) por vez. Ou seja, processa dados de maior complexidade.

**Ex**: o chip ESP8266 (NodeMCU)

### `2.4.4` 64 bits
Embora um chip de 64 bits possa ter novas instruções específicas, o ganho real está na capacidade de cada instrução manipular dados maiores (daods de 64 bits), ou seja a capacidade de resolver cálculos mais complexos.

Em Arquiteturas de 64 bits, os registradores de uso geral são extendidos de 32 bits para 64 bits, e novos registradores são adicionados a CPU. Isso permite que a CPU guarde e trabalhe com mais dados internamente sem precisar recorrer à memória RAM o tempo todo.

**Ex**: Intel x86-64, AMD x86-64.

## 3. do Hardware ao Software
Para conectar o Software com o Hardware é preciso entender como o código reconhece o Hardware a faz o trabalho de busca e execução diretamente nos registradores.

### Ciclo de Instrução
O ciclo baseia-se em um registrador interno essencial: o **PC** *Program Counter*, que guarda o endereço da próxima instrução a ser executada.

- **Passo 1**:  Energização/Reset

O processador envia o endereço contido no **PC** através do barramento de endereços e lê a instrução que está na memória d programa

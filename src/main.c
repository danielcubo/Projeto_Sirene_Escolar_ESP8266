// 1. MAPEAMENTO DE REGISTRADORES (Baseado no Technical Reference Manual do ESP8266)
#define REG_BASE_GPIO      0x60000300
#define GPIO_ENABLE        *(volatile unsigned int *)(REG_BASE_GPIO + 0x0C)
#define GPIO_OUT_SET       *(volatile unsigned int *)(REG_BASE_GPIO + 0x04)
#define GPIO_OUT_CLEAR     *(volatile unsigned int *)(REG_BASE_GPIO + 0x08)
#define RELAY_PIN          2  // GPIO 2 para o Relé

#define REG_BASE_UART0     0x60000000
#define UART_FIFO          *(volatile unsigned int *)(REG_BASE_UART0 + 0x00)
#define UART_STATUS        *(volatile unsigned int *)(REG_BASE_UART0 + 0x1C)

// 2. ESTRUTURA DE DADOS PARA OS HORÁRIOS
typedef struct {
    unsigned char hora;
    unsigned char minuto;
} HorarioSinal;

// Total de 17 horários cadastrados da sua lista
#define TOTAL_HORARIOS 17
const HorarioSinal horarios_escola[TOTAL_HORARIOS] = {
    {7, 0},  {7, 50}, {8, 40}, {9, 0},  {9, 15}, {9, 30}, {9, 45}, {10, 35}, {11, 25}, {12, 15}, // Manhã
    {13, 0}, {15, 0}, {15, 15}, {15, 30}, {15, 45}, {17, 25}, {18, 20}                         // Tarde
};

// Variáveis globais de tempo controladas pelo chip
volatile unsigned char hora_atual = 0;
volatile unsigned char minuto_atual = 0;
volatile unsigned char segundo_atual = 0;
volatile unsigned int contador_ciclos = 0;

// 3. FUNÇÕES DE BAIXO NÍVEL
void delay_ms(volatile unsigned int ms) {
    // Ajuste grosseiro de delay baseado no clock padrão de 80MHz do chip
    volatile unsigned int ciclos = ms * 8000;
    while(ciclos--) {
        __asm__ __volatile__("nop");
    }
}

void tocar_campainha(unsigned int milissegundos) {
    GPIO_OUT_SET = (1 << RELAY_PIN);    // Ativa o Relé
    delay_ms(milissegundos);             // Mantém tocando (ex: 3000ms = 3 segundos)
    GPIO_OUT_CLEAR = (1 << RELAY_PIN);  // Desliga o Relé
}

// Lê um byte da UART0 se houver dados disponíveis
int uart_receber_byte(unsigned char *byte) {
    // Verifica os bits de contagem de bytes recebidos no UART_STATUS (Rx FIFO)
    unsigned int status = UART_STATUS;
    unsigned char bytes_na_fila = (status >> 0) & 0xFF; 
    
    if (bytes_na_fila > 0) {
        *byte = (unsigned char)(UART_FIFO & 0xFF);
        return 1; // Sucesso
    }
    return 0; // Fila vazia
}

// 4. PONTO DE ENTRADA DO HARDWARE
void call_user_start(void) __attribute__((section(".text")));

void call_user_start(void) {
    // Configura pino do Relé como Saída
    GPIO_ENABLE |= (1 << RELAY_PIN);
    GPIO_OUT_CLEAR = (1 << RELAY_PIN); // Garante campainha desligada ao iniciar

    unsigned char byte_recebido;
    unsigned char estado_protocolo = 0;

    // Loop Principal (Super Loop)
    while(1) {
        
        // Sincronização via Windows (UART)
        // Protocolo simples esperado do Windows: caractere 'H', seguido do byte da hora, e byte do minuto.
        if (uart_receber_byte(&byte_recebido)) {
            if (estado_protocolo == 0 && byte_recebido == 'H') {
                estado_protocolo = 1;
            } else if (estado_protocolo == 1) {
                hora_atual = byte_recebido;
                estado_protocolo = 2;
            } else if (estado_protocolo == 2) {
                minuto_atual = byte_recebido;
                segundo_atual = 0; // Zera os segundos na sincronização
                estado_protocolo = 0;
            }
        }

        // Relógio interno baseado no ciclo do Loop (Aproximação de 1 segundo)
        // Em um projeto final, usaríamos o registrador do TIMER de hardware.
        delay_ms(1000); 
        segundo_atual++;
        if (segundo_atual >= 60) {
            segundo_atual = 0;
            minuto_atual++;
            
            if (minuto_atual >= 60) {
                minuto_atual = 0;
                hora_atual++;
                if (hora_atual >= 24) {
                    hora_atual = 0;
                }
            }

            // Checagem da tabela de horários (executada apenas uma vez a cada virada de minuto)
            for (int i = 0; i < TOTAL_HORARIOS; i++) {
                if (horarios_escola[i].hora == hora_atual && horarios_escola[i].minuto == minuto_atual) {
                    tocar_campainha(4000); // Toca por 4 segundos
                    break; 
                }
            }
        }
    }
}

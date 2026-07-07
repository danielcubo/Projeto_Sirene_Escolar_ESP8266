# Lista de todos os arquivos .c do projeto
SRCS = main.c

# DETECÇÃO AUTOMÁTICA DE SISTEMA OPERACIONAL
ifeq ($(OS),Windows_NT)
    # Configurações para Windows 11 / MSYS2
    PORT = COM4
    RM = del /Q
    ESPTOOL = esptool.exe
else
    # Configurações para Debian Linux
    PORT = /dev/ttyUSB0
    RM = rm -f
    ESPTOOL = esptool.py
endif

# Ferramentas de Compilação (Toolchain Xtensa)
CC = xtensa-lx106-elf-gcc
OBJCOPY = xtensa-lx106-elf-objcopy
CFLAGS = -I. -mlongcalls -nostdlib -O2
LDFLAGS = -Teagle.app.v6.ld

TARGET = main

all: $(TARGET).bin

$(TARGET).out: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) main.c -o $(TARGET).out

$(TARGET).bin: $(TARGET).out
	$(OBJCOPY) -O binary $(TARGET).out $(TARGET).bin

flash: $(TARGET).bin
	$(ESPTOOL) --port $(PORT) --baud 115200 write_flash 0x00000 $(TARGET).bin

clean:
	$(RM) *.out *.bin *.o
###############################################################################
# Makefile for the project AVR
###############################################################################

## General Flags
PROJECT = main
MCU = atmega16
TARGET = $(PROJECT)

CC=avr-gcc
OBJCOPY=avr-objcopy
ASIZE=avr-size

AVRDUDE=avrdude
PROGRAMMER ?= usbasp

BACKUP_NAME ?= backup

OBJ_PATH = obj

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON) \
	-I.\
	-I..\

CFLAGS += -DF_CPU=8000000UL
CFLAGS += -gstabs -O2 -Wstrict-prototypes -fshort-enums -std=gnu99

CFLAGS += -Wp,-M,-MP,-MT,$(*F).o,-MF,dep/$(@F).d

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += -x assembler-with-cpp -Wa,-gstabs

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -Wl,-Map=$(TARGET).map,--cref

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom
HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0


## Objects that must be built in order to link
#OBJECTS = excoils.o
PROJECT_SRC  = $(PROJECT).c
PROJECT_SRC += lcd.c lcd_hw.c
PROJECT_SRC += encoder.c encoder_hw.c
PROJECT_SRC += configure.c ui.c uart.c gs232.c delay_hw.c
PROJECT_SRC += motors.c motors_hw.c
PROJECT_SRC += rotate.c rotate_hw.c
OBJECTS = $(patsubst %.c, $(OBJ_PATH)/%.o, $(PROJECT_SRC))

VPATH += ..

CHEAT_ARG := $(shell mkdir -p $(OBJ_PATH))

## Build
all: $(TARGET).elf $(TARGET).hex $(TARGET).eep size

size: $(TARGET).elf
	@$(ASIZE) --mcu=$(MCU) --format=avr $(TARGET).elf

## Compile
$(OBJ_PATH)/%.o: %.c Makefile signals.h
	@echo "Compile $<"
	@$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

signals.h: Makefile signals.conf
	@echo "Generate signals.h"
	@rm -f signals.h
	@echo '#ifndef SIGNALS_H\n#define SIGNALS_H\n#include <avr/io.h>\n' > signals.h
	@awk '{print "#define "$$1"_PIN_REG PIN"$$2"\n#define "$$1"_PORT_REG PORT"$$2"\n#define "$$1"_DDR_REG DDR"$$2"\n#define "$$1"_PIN (1 << P"$$2$$3")\n"}' signals.conf >> signals.h
	@echo '#endif' >> signals.h
##Link
$(TARGET).elf: $(OBJECTS)
	@echo "Linking $(TARGET).elf"
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET).elf

%.hex: $(TARGET).elf
	@echo "Generate $@"
	@$(OBJCOPY) -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET).elf
	@echo "Generate $@"
	@$(OBJCOPY) $(HEX_EEPROM_FLAGS) -O ihex $< $@

%.lss: $(TARGET)
	$(OBJCOPY) -h -S $< > $@

all-flash: fuse flash

MCU_AVRDUDE = m8
AVRDUDE_SPEED_LOW = -B 5
AVRDUDE_SPEED_HIGH = -B 1
fuse:
	$(AVRDUDE) -p $(MCU_AVRDUDE) -c $(PROGRAMMER) $(AVRDUDE_SPEED_LOW) -U lfuse:w:0x84:m -U hfuse:w:0x91:m -U lock:w:0xFF:m

chip-clean:
	$(AVRDUDE) -p $(MCU_AVRDUDE) -c $(PROGRAMMER) $(AVRDUDE_SPEED_LOW) -U lfuse:w:0xC1:m -U hfuse:w:0x99:m -U lock:w:0xFF:m
	$(AVRDUDE) -p $(MCU_AVRDUDE) -c $(PROGRAMMER) $(AVRDUDE_SPEED_LOW) -e

lock:
	$(AVRDUDE) -p $(MCU_AVRDUDE) -c $(PROGRAMMER) $(AVRDUDE_SPEED_LOW) -U lock:w:0xfc:m

reboot:
	$(AVRDUDE) -c avrftdi -p m16 -C ~/avrdude.conf

flash: $(TARGET).hex
	$(AVRDUDE) -c avrftdi -p m16 -C ~/avrdude.conf -V -U flash:w:$(TARGET).hex

flash-noverify: $(TARGET).hex
	$(AVRDUDE) -p $(MCU_AVRDUDE) -c $(PROGRAMMER) -V $(AVRDUDE_SPEED_HIGH) -U flash:w:$(TARGET).hex

eeprom: $(TARGET).eep
	$(AVRDUDE) -p $(MCU_AVRDUDE) -c $(PROGRAMMER) $(AVRDUDE_SPEED_HIGH) -U eeprom:w:$(TARGET).eep

backup:
	$(AVRDUDE) -p $(MCU_AVRDUDE) -c $(PROGRAMMER) $(AVRDUDE_SPEED_HIGH) -U flash:r:$(BACKUP_NAME).hex:i -U eeprom:r:$(BACKUP_NAME).eep:i

read_firmware:
	$(AVRDUDE) -p $(MCU_AVRDUDE) -c $(PROGRAMMER) $(AVRDUDE_SPEED_HIGH) -U eeprom:r:tmp.eep:i
	objcopy --input-target=ihex --output-target=binary tmp.eep tmp.bin
	dd if=tmp.bin of=readed_firmware.bin bs=256 count=1
	rm -f tmp.eep tmp.bin
	hexdump readed_firmware.bin

## Clean target
.PHONY: clean
clean:
	@rm -rf $(TARGET).elf $(TARGET).hex $(TARGET).eep $(TARGET).cof $(TARGET).map
	@rm -rf dep/
	@rm -rf $(OBJ_PATH)/
	@rm -rf signals.h

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

ETAGS?=etags
.PHONY: TAGS tags
TAGS:
	$(ETAGS) `ls *.[ch]`
	$(ETAGS) -a `find $(FREEMODBUS_PATH)/ -name '*.[ch]'`

tags:
	make TAGS ETAGS=ctags

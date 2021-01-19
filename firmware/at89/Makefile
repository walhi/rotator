# License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)

PROJECT = main
MCU = at89s52
MCU_FLASH = m8

CC = sdcc
SIZE = avr-size
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

PROGRAMMER ?= usbasp

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom
HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0

OBJ_PATH = obj

INCLUDE = -I . -I ..
CFLAGS = -mmcs51  --std-sdcc99 --model-small -c $(INCLUDE) --disable-warning 126 --opt-code-size
LDFLAGS = --iram-size 256 --xram-size 0 --code-size 8192  --nooverlay --noinduction

PROJECT_SRC  = $(PROJECT).c
PROJECT_SRC += ui.c
#PROJECT_SRC += configure.c
PROJECT_SRC += motors.c motors_hw.c rotate_hw.c rotate.c
PROJECT_SRC += encoder.c encoder_hw.c
PROJECT_SRC += lcd.c lcd_hw.c
PROJECT_SRC += uart.c gs232.c
PROJECT_SRC += delay_hw.c

OBJECTS = $(patsubst %.c, $(OBJ_PATH)/%.rel, $(PROJECT_SRC))

VPATH += ..

# ------------------ Build --------------------#
all: $(PROJECT).hex size

$(OBJ_PATH)/%.rel: %.c Makefile
	@echo "Compile $<"
	@$(CC) $(CFLAGS) $< -o $@

$(PROJECT).ihx: $(OBJECTS)
	@echo "Linking $@"
	$(CC) $(LFLAGS) $^

$(PROJECT).hex:	$(PROJECT).ihx
	@echo "Pack $@"
	@packihx $(PROJECT).ihx > $(PROJECT).hex &> /dev/null

size: $(PROJECT).ihx
	@echo "Size $^"
	@size $^

#grep Area -A 2 $< | grep -v Area | grep -v - | uniq | awk '{print $$5"\t"$$1}' | grep [0-9]

# ------------------ Clean --------------------#
clean:
	@rm -f *.adb *.asm *.o *.map *.mem *.omf *.rst *.rel *.sym *.cdb *.ihx *.lk *.lst *.hex
	@rm -f ./$(OBJ_PATH)/*


# ------------------ Flash --------------------#
flash: $(PROJECT).hex
	$(AVRDUDE) -c avrftdi -p 8052 -C ~/avrdude.conf -V -e -U flash:w:$(PROJECT).hex

# ------------------ Tags ---------------------#
ETAGS?=etags
.PHONY: TAGS tags
TAGS:
	$(ETAGS) `ls *.[ch]`

tags:
	make TAGS ETAGS=ctags

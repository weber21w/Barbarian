###############################################################################
# Makefile for the project Barbarian
###############################################################################

## General Flags
PROJECT = Barbarian
GAME = Barbarian
MCU = atmega644
TARGET = $(GAME).elf
CC = avr-gcc
INFO=../gameinfo.properties
UZEBIN_DIR = ../../../bin

## Kernel settings
KERNEL_DIR = ../../../kernel

## Escape spaces in mixer path (due to custom sounds.inc)
EMPTY:=
SPACE:= $(EMPTY) $(EMPTY)
SPACE_ESC:= \\$(SPACE)
MIX_PATH:= $(realpath ../data/sounds.inc)
MIX_PATH_ESC:= $(subst $(SPACE),$(SPACE_ESC),$(MIX_PATH))

KERNEL_OPTIONS  = -DVIDEO_MODE=748 -DFIRST_RENDER_LINE=36
KERNEL_OPTIONS += -DTRANSLUCENT_COLOR=0xC7 -DTRUE_RANDOM_GEN=1
KERNEL_OPTIONS += -DCONTROLLERS_VSYNC_READ=0
KERNEL_OPTIONS += -DUART=2 -DUART_RX_BUFFER_SIZE=16 -DUART_TX_BUFFER_SIZE=2
KERNEL_OPTIONS += -DMUSIC_ENGINE=STREAM -DSOUND_MIXER=1 -DSOUND_CHANNEL_5_ENABLE=0 -DMIXER_WAVES=\"$(MIX_PATH_ESC)\"

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=28636360UL -Os -fsigned-char -ffunction-sections -fno-toplevel-reorder
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 
CFLAGS += $(KERNEL_OPTIONS)

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -Wl,-Map=$(GAME).map 
LDFLAGS += -Wl,-gc-sections 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = uzeboxVideoEngineCore.o uzeboxCore.o uzeboxSoundEngine.o uzeboxSoundEngineCore.o uzeboxVideoEngine.o spiram.o $(GAME).o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Include Directories
INCLUDES = -I"$(KERNEL_DIR)"

# Resources
GFX_RESOURCES += ../data/gfx/bg_arena.inc
GFX_RESOURCES += ../data/gfx/bg_forest.inc
GFX_RESOURCES += ../data/gfx/bg_plaine.inc
GFX_RESOURCES += ../data/gfx/bg_throne.inc
GFX_RESOURCES += ../data/gfx/blood.inc
GFX_RESOURCES += ../data/gfx/crouch1.inc
GFX_RESOURCES += ../data/gfx/crouch2.inc
GFX_RESOURCES += ../data/gfx/decap1.inc
GFX_RESOURCES += ../data/gfx/decap2.inc
GFX_RESOURCES += ../data/gfx/decap3.inc
GFX_RESOURCES += ../data/gfx/decap4.inc
GFX_RESOURCES += ../data/gfx/die2.inc
GFX_RESOURCES += ../data/gfx/die3.inc
GFX_RESOURCES += ../data/gfx/die4.inc
GFX_RESOURCES += ../data/gfx/drax1.inc
GFX_RESOURCES += ../data/gfx/drax2.inc
GFX_RESOURCES += ../data/gfx/fall1.inc
GFX_RESOURCES += ../data/gfx/fall2.inc
GFX_RESOURCES += ../data/gfx/fall3.inc
GFX_RESOURCES += ../data/gfx/front1.inc
GFX_RESOURCES += ../data/gfx/front2.inc
GFX_RESOURCES += ../data/gfx/front3.inc
GFX_RESOURCES += ../data/gfx/gnome1.inc
GFX_RESOURCES += ../data/gfx/gnome2.inc
GFX_RESOURCES += ../data/gfx/gnome3.inc
GFX_RESOURCES += ../data/gfx/gnome4.inc
GFX_RESOURCES += ../data/gfx/guard_hi.inc
GFX_RESOURCES += ../data/gfx/guard_mid.inc
GFX_RESOURCES += ../data/gfx/gui_title.inc
GFX_RESOURCES += ../data/gfx/head_shadow.inc
GFX_RESOURCES += ../data/gfx/head1.inc
GFX_RESOURCES += ../data/gfx/head2.inc
GFX_RESOURCES += ../data/gfx/head3.inc
GFX_RESOURCES += ../data/gfx/head4.inc
GFX_RESOURCES += ../data/gfx/head5.inc
GFX_RESOURCES += ../data/gfx/head6.inc
GFX_RESOURCES += ../data/gfx/headbutt1.inc
GFX_RESOURCES += ../data/gfx/headbutt2.inc
GFX_RESOURCES += ../data/gfx/idle1.inc
GFX_RESOURCES += ../data/gfx/idle2.inc
GFX_RESOURCES += ../data/gfx/idle3.inc
GFX_RESOURCES += ../data/gfx/jump1_shadow.inc
GFX_RESOURCES += ../data/gfx/jump1.inc
GFX_RESOURCES += ../data/gfx/jump2_shadow.inc
GFX_RESOURCES += ../data/gfx/jump2.inc
GFX_RESOURCES += ../data/gfx/kick1.inc
GFX_RESOURCES += ../data/gfx/kick2.inc
GFX_RESOURCES += ../data/gfx/knee1.inc
GFX_RESOURCES += ../data/gfx/knee3.inc
GFX_RESOURCES += ../data/gfx/marianna.inc
GFX_RESOURCES += ../data/gfx/mid1.inc
GFX_RESOURCES += ../data/gfx/mid2.inc
GFX_RESOURCES += ../data/gfx/mid3.inc
GFX_RESOURCES += ../data/gfx/neck2.inc
GFX_RESOURCES += ../data/gfx/neck3.inc
GFX_RESOURCES += ../data/gfx/roll1.inc
GFX_RESOURCES += ../data/gfx/roll2.inc
GFX_RESOURCES += ../data/gfx/roll3.inc
GFX_RESOURCES += ../data/gfx/roll5.inc
GFX_RESOURCES += ../data/gfx/spin1.inc
GFX_RESOURCES += ../data/gfx/spin2.inc
GFX_RESOURCES += ../data/gfx/spin3.inc
GFX_RESOURCES += ../data/gfx/stand.inc
GFX_RESOURCES += ../data/gfx/touche1.inc
GFX_RESOURCES += ../data/gfx/touche2.inc
GFX_RESOURCES += ../data/gfx/victory1.inc
GFX_RESOURCES += ../data/gfx/victory2.inc
GFX_RESOURCES += ../data/gfx/victory3.inc
GFX_RESOURCES += ../data/gfx/walk1.inc
GFX_RESOURCES += ../data/gfx/walk2.inc
GFX_RESOURCES += ../data/gfx/walk3.inc
GFX_RESOURCES += ../data/gfx/web1.inc
GFX_RESOURCES += ../data/gfx/web2.inc
GFX_RESOURCES += ../data/gfx/web3.inc
GFX_RESOURCES += ../data/gfx/web4.inc

MUSIC_RESOURCES = ../data/music-compression.cfg

MUSIC_RESOURCES += ../data/music-uncompressed/track1.inc
MUSIC_UNCOMPRESSED_LIST += ../data/music-uncompressedtrack1.mid
MUSIC_RESOURCES += ../data/music-uncompressed/track2.inc
MUSIC_UNCOMPRESSED_LIST += ../data/music-uncompressedtrack2.mid
MUSIC_RESOURCES += ../data/music-uncompressed/track3.inc
MUSIC_UNCOMPRESSED_LIST += ../data/music-uncompressedtrack3.mid


## Build
all: $(GFX_RESOURCES) $(MUSIC_RESOURCES) $(SFX_RESOURCES) $(TARGET) $(GAME).hex $(GAME).eep $(GAME).lss $(GAME).uze size

## Rebuild resources
../data/gfx/bg_arena.inc: ../resources/gfx/bg-arena.png ../data/generated-xml/bg_arena.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/bg_arena.xml

../data/gfx/bg_forest.inc: ../resources/gfx/bg-forest.png ../data/generated-xml/bg_forest.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/bg_forest.xml

../data/gfx/bg_plaine.inc: ../resources/gfx/bg-plaine.png ../data/generated-xml/bg_plaine.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/bg_plaine.xml

../data/gfx/bg_throne.inc: ../resources/gfx/bg-throne.png ../data/generated-xml/bg_throne.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/bg_throne.xml

../data/gfx/blood.inc: ../resources/gfx/blood.png ../data/generated-xml/blood.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/blood.xml

../data/gfx/crouch1.inc: ../resources/gfx/crouch1.png ../data/generated-xml/crouch1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/crouch1.xml

../data/gfx/crouch2.inc: ../resources/gfx/crouch2.png ../data/generated-xml/crouch2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/crouch2.xml

../data/gfx/decap1.inc: ../resources/gfx/decap1.png ../data/generated-xml/decap1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/decap1.xml

../data/gfx/decap2.inc: ../resources/gfx/decap2.png ../data/generated-xml/decap2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/decap2.xml

../data/gfx/decap3.inc: ../resources/gfx/decap3.png ../data/generated-xml/decap3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/decap3.xml

../data/gfx/decap4.inc: ../resources/gfx/decap4.png ../data/generated-xml/decap4.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/decap4.xml

../data/gfx/die2.inc: ../resources/gfx/die2.png ../data/generated-xml/die2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/die2.xml

../data/gfx/die3.inc: ../resources/gfx/die3.png ../data/generated-xml/die3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/die3.xml

../data/gfx/die4.inc: ../resources/gfx/die4.png ../data/generated-xml/die4.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/die4.xml

../data/gfx/drax1.inc: ../resources/gfx/drax1.png ../data/generated-xml/drax1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/drax1.xml

../data/gfx/drax2.inc: ../resources/gfx/drax2.png ../data/generated-xml/drax2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/drax2.xml

../data/gfx/fall1.inc: ../resources/gfx/fall1.png ../data/generated-xml/fall1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/fall1.xml

../data/gfx/fall2.inc: ../resources/gfx/fall2.png ../data/generated-xml/fall2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/fall2.xml

../data/gfx/fall3.inc: ../resources/gfx/fall3.png ../data/generated-xml/fall3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/fall3.xml

../data/gfx/front1.inc: ../resources/gfx/front1.png ../data/generated-xml/front1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/front1.xml

../data/gfx/front2.inc: ../resources/gfx/front2.png ../data/generated-xml/front2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/front2.xml

../data/gfx/front3.inc: ../resources/gfx/front3.png ../data/generated-xml/front3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/front3.xml

../data/gfx/gnome1.inc: ../resources/gfx/gnome1.png ../data/generated-xml/gnome1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/gnome1.xml

../data/gfx/gnome2.inc: ../resources/gfx/gnome2.png ../data/generated-xml/gnome2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/gnome2.xml

../data/gfx/gnome3.inc: ../resources/gfx/gnome3.png ../data/generated-xml/gnome3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/gnome3.xml

../data/gfx/gnome4.inc: ../resources/gfx/gnome4.png ../data/generated-xml/gnome4.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/gnome4.xml

../data/gfx/guard_hi.inc: ../resources/gfx/guard-hi.png ../data/generated-xml/guard_hi.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/guard_hi.xml

../data/gfx/guard_mid.inc: ../resources/gfx/guard-mid.png ../data/generated-xml/guard_mid.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/guard_mid.xml

../data/gfx/gui_title.inc: ../resources/gfx/gui-title.png ../data/generated-xml/gui_title.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/gui_title.xml

../data/gfx/head_shadow.inc: ../resources/gfx/head-shadow.png ../data/generated-xml/head_shadow.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/head_shadow.xml

../data/gfx/head1.inc: ../resources/gfx/head1.png ../data/generated-xml/head1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/head1.xml

../data/gfx/head2.inc: ../resources/gfx/head2.png ../data/generated-xml/head2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/head2.xml

../data/gfx/head3.inc: ../resources/gfx/head3.png ../data/generated-xml/head3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/head3.xml

../data/gfx/head4.inc: ../resources/gfx/head4.png ../data/generated-xml/head4.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/head4.xml

../data/gfx/head5.inc: ../resources/gfx/head5.png ../data/generated-xml/head5.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/head5.xml

../data/gfx/head6.inc: ../resources/gfx/head6.png ../data/generated-xml/head6.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/head6.xml

../data/gfx/headbutt1.inc: ../resources/gfx/headbutt1.png ../data/generated-xml/headbutt1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/headbutt1.xml

../data/gfx/headbutt2.inc: ../resources/gfx/headbutt2.png ../data/generated-xml/headbutt2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/headbutt2.xml

../data/gfx/idle1.inc: ../resources/gfx/idle1.png ../data/generated-xml/idle1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/idle1.xml

../data/gfx/idle2.inc: ../resources/gfx/idle2.png ../data/generated-xml/idle2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/idle2.xml

../data/gfx/idle3.inc: ../resources/gfx/idle3.png ../data/generated-xml/idle3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/idle3.xml

../data/gfx/jump1_shadow.inc: ../resources/gfx/jump1-shadow.png ../data/generated-xml/jump1_shadow.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/jump1_shadow.xml

../data/gfx/jump1.inc: ../resources/gfx/jump1.png ../data/generated-xml/jump1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/jump1.xml

../data/gfx/jump2_shadow.inc: ../resources/gfx/jump2-shadow.png ../data/generated-xml/jump2_shadow.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/jump2_shadow.xml

../data/gfx/jump2.inc: ../resources/gfx/jump2.png ../data/generated-xml/jump2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/jump2.xml

../data/gfx/kick1.inc: ../resources/gfx/kick1.png ../data/generated-xml/kick1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/kick1.xml

../data/gfx/kick2.inc: ../resources/gfx/kick2.png ../data/generated-xml/kick2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/kick2.xml

../data/gfx/knee1.inc: ../resources/gfx/knee1.png ../data/generated-xml/knee1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/knee1.xml

../data/gfx/knee3.inc: ../resources/gfx/knee3.png ../data/generated-xml/knee3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/knee3.xml

../data/gfx/marianna.inc: ../resources/gfx/marianna.png ../data/generated-xml/marianna.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/marianna.xml

../data/gfx/mid1.inc: ../resources/gfx/mid1.png ../data/generated-xml/mid1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/mid1.xml

../data/gfx/mid2.inc: ../resources/gfx/mid2.png ../data/generated-xml/mid2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/mid2.xml

../data/gfx/mid3.inc: ../resources/gfx/mid3.png ../data/generated-xml/mid3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/mid3.xml

../data/gfx/neck2.inc: ../resources/gfx/neck2.png ../data/generated-xml/neck2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/neck2.xml

../data/gfx/neck3.inc: ../resources/gfx/neck3.png ../data/generated-xml/neck3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/neck3.xml

../data/gfx/roll1.inc: ../resources/gfx/roll1.png ../data/generated-xml/roll1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/roll1.xml

../data/gfx/roll2.inc: ../resources/gfx/roll2.png ../data/generated-xml/roll2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/roll2.xml

../data/gfx/roll3.inc: ../resources/gfx/roll3.png ../data/generated-xml/roll3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/roll3.xml

../data/gfx/roll5.inc: ../resources/gfx/roll5.png ../data/generated-xml/roll5.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/roll5.xml

../data/gfx/spin1.inc: ../resources/gfx/spin1.png ../data/generated-xml/spin1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/spin1.xml

../data/gfx/spin2.inc: ../resources/gfx/spin2.png ../data/generated-xml/spin2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/spin2.xml

../data/gfx/spin3.inc: ../resources/gfx/spin3.png ../data/generated-xml/spin3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/spin3.xml

../data/gfx/stand.inc: ../resources/gfx/stand.png ../data/generated-xml/stand.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/stand.xml

../data/gfx/touche1.inc: ../resources/gfx/touche1.png ../data/generated-xml/touche1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/touche1.xml

../data/gfx/touche2.inc: ../resources/gfx/touche2.png ../data/generated-xml/touche2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/touche2.xml

../data/gfx/victory1.inc: ../resources/gfx/victory1.png ../data/generated-xml/victory1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/victory1.xml

../data/gfx/victory2.inc: ../resources/gfx/victory2.png ../data/generated-xml/victory2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/victory2.xml

../data/gfx/victory3.inc: ../resources/gfx/victory3.png ../data/generated-xml/victory3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/victory3.xml

../data/gfx/walk1.inc: ../resources/gfx/walk1.png ../data/generated-xml/walk1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/walk1.xml

../data/gfx/walk2.inc: ../resources/gfx/walk2.png ../data/generated-xml/walk2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/walk2.xml

../data/gfx/walk3.inc: ../resources/gfx/walk3.png ../data/generated-xml/walk3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/walk3.xml

../data/gfx/web1.inc: ../resources/gfx/web1.png ../data/generated-xml/web1.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/web1.xml

../data/gfx/web2.inc: ../resources/gfx/web2.png ../data/generated-xml/web2.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/web2.xml

../data/gfx/web3.inc: ../resources/gfx/web3.png ../data/generated-xml/web3.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/web3.xml

../data/gfx/web4.inc: ../resources/gfx/web4.png ../data/generated-xml/web4.xml
	$(UZEBIN_DIR)/gconvert ../data/generated-xml/web4.xml

../data/music-uncompressed/track1.inc: ../resources/midi/track1.mid
	$(UZEBIN_DIR)/midiconv -f 60 ../resources/midi/track1.mid ../data/music-uncompressed/track1.inc -vmus_track1track1

../data/music-uncompressed/track2.inc: ../resources/midi/track2.mid
	$(UZEBIN_DIR)/midiconv -f 60 ../resources/midi/track2.mid ../data/music-uncompressed/track2.inc -vmus_track1track2

../data/music-uncompressed/track3.inc: ../resources/midi/track3.mid
	$(UZEBIN_DIR)/midiconv -f 60 ../resources/midi/track3.mid ../data/music-uncompressed/track3.inc -vmus_track1track3

#../data/music-compressed.inc: $(MIDI_UNCOMPRESSED_LIST)
	\$(UZEBIN_DIR)/mconvert ../data/music-compression.cfg

## Compile Kernel files
uzeboxVideoEngineCore.o: $(KERNEL_DIR)/uzeboxVideoEngineCore.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

uzeboxSoundEngineCore.o: $(KERNEL_DIR)/uzeboxSoundEngineCore.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

uzeboxCore.o: $(KERNEL_DIR)/uzeboxCore.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uzeboxSoundEngine.o: $(KERNEL_DIR)/uzeboxSoundEngine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uzeboxVideoEngine.o: $(KERNEL_DIR)/uzeboxVideoEngine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spiram.o: $(KERNEL_DIR)/spiram.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

## Compile game sources
#asmFunctions.o: ../asmFunctions.s
#	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

$(GAME).o: ../Barbarian.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

%.uze: $(TARGET)
	-$(UZEBIN_DIR)/packrom $(GAME).hex $@ $(INFO)

UNAME := $(shell sh -c uname -s 2>/dev/null || echo not)
AVRSIZEFLAGS := -A ${TARGET}
ifneq (,$(findstring MINGW,$(UNAME)))
AVRSIZEFLAGS := -C --mcu=${MCU} ${TARGET}
endif

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(GAME).* dep/* *.uze

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

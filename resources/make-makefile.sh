#!/bin/bash
cd $(dirname $0)

#Eh..this is a ridiculous hack..

rm -f ../data/generated-xml/*.xml
rm -f ../data/gfx-compressed/*.inc
rm -f ../data/music-uncompressed/*.mid
rm -f ../data/sfx-compressed/*.inc

MAKEFILE_TOP='###############################################################################
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
'

echo "$MAKEFILE_TOP" > ../default/makefile

#######################################################################################
SPRITE_FORM_DATA='<?xml version="1.0" ?>
<gfx-xform version="1">
	<input file="../resources/gfx/{{IMG_NAME}}.png" type="png" tile-width="{{IMG_W}}" tile-height="{{IMG_H}}" />
	<output file="../data/gfx-compressed/{{IMG_NAME}}.inc" type="2bpp" isBackgroundTiles="{{IMG_BG}}">
		<palette file="../resources/palette/{{IMG_PAL}}.png" var-name="graphicsPalette" maxColors="4" exportPalette="false" transparent-color="199"/>
		<tiles var-name="{{IMG_VAR}}"/>
		<maps pointers-size="8">
			<map var-name="skipped-map" top="0" left="0" width="1" height="1" skip-output="true"/>
		</maps>
	</output>
</gfx-xform>'

#build all sprite XMLs and setup makefile data
> gfx-rebuild-rules.inc

echo "Setting up graphics.."
echo '# Resources' >> ../default/makefile

for FILE in gfx/*.png
do
	if [ ! -f "$FILE" ]; then continue; fi

	IMG_BG='false'
	if [[ "$FILE" == *'bg'* ]]
	then
		IMG_BG='true'
	fi

	IMG_PAL='player-palette' #escaped for sed
	if [[ "$FILE" == *'drax'* ]]
	then
		IMG_PAL='drax-palette'
	elif [[ "$FILE" == *'forest'* ]]
	then
		IMG_PAL='forest-palette'
	elif [[ "$FILE" == *'plaine'* ]]
	then
		IMG_PAL='plaine-palette'
	elif [[ "$FILE" == *'arena'* ]]
	then
		IMG_PAL='arena-palette'
	elif [[ "$FILE" == *'throne'* ]]
	then
		IMG_PAL='throne-palette'
	fi

	FILE_NAME=$(echo "$FILE" | awk -F'/' '{ print $2 }')
	IMG_NAME=$(echo "$FILE_NAME" | awk -F'.' '{ print $1 }')

	RES=$(file "$FILE" | awk -F',' '{ print $2 }' | tr -d ' ' | tr -d '\r' | tr -d '\n')
	#gfx/80-walk2.png: PNG image data, 21 x 50, 8-bit colormap, non-interlaced
	IMG_W=$(echo "$RES" | awk -F'x' '{ print $1 }' | tr -d '\r' | tr -d '\n')
	IMG_H=$(echo "$RES" | awk -F'x' '{ print $2 }' | tr -d '\r' | tr -d '\n')
	IMG_VAR=$(echo "$IMG_NAME" | tr '-' '_')
	if [[ "$IMG_VAR" == *'80'* ]]; then IMG_VAR="${IMG_VAR:3:99}"; fi
	if [[ "$IMG_NAME" == *'80'* ]]; then IMG_NAME="${IMG_NAME:3:99}"; fi
	echo "[$IMG_NAME][$IMG_W][$IMG_H][$IMG_PAL][$IMG_VAR]"

	XML_DATA="$SPRITE_FORM_DATA"
	XML_DATA=$(echo "$XML_DATA" | sed "s/{{IMG_NAME}}/$IMG_NAME/g")
	XML_DATA=$(echo "$XML_DATA" | sed "s/{{IMG_W}}/$IMG_W/g")
	XML_DATA=$(echo "$XML_DATA" | sed "s/{{IMG_H}}/$IMG_H/g")
	XML_DATA=$(echo "$XML_DATA" | sed "s/{{IMG_PAL}}/$IMG_PAL/g")
	XML_DATA=$(echo "$XML_DATA" | sed "s/{{IMG_BG}}/$IMG_BG/g")
	XML_DATA=$(echo "$XML_DATA" | sed "s/{{IMG_VAR}}/$IMG_VAR/g")
	echo "$XML_DATA" > "../data/generated-xml/$IMG_VAR.xml"
	echo "GFX_RESOURCES += ../data/gfx/$IMG_VAR.inc" >>  ../default/makefile

REBUILD_STRING="../data/gfx/$IMG_VAR.inc: ../resources/gfx/$FILE_NAME ../data/generated-xml/$IMG_VAR.xml
	\$(UZEBIN_DIR)/gconvert ../data/generated-xml/$IMG_VAR.xml
"

	echo "$REBUILD_STRING" >> gfx-rebuild-rules.inc



done

echo "" >> ../default/makefile

#Build all music resources and setup makefile data
echo "MUSIC_RESOURCES = ../data/music-compression.cfg" >> ../default/makefile
echo "" >> ../default/makefile

> music-rebuild-rules.inc

echo "Setting up music.."
for FILE in midi/*
do
	if [ ! -f "$FILE" ]; then continue; fi
        FILE_NAME=$(echo "$FILE" | awk -F'/' '{ print $2 }')
        MUS_NAME=$(echo "$FILE_NAME" | awk -F'.' '{ print $1 }')
	echo "[$MUS_NAME]"
	echo "MUSIC_RESOURCES += ../data/music-uncompressed/$MUS_NAME.inc" >> ../default/makefile
	echo "MUSIC_UNCOMPRESSED_LIST += ../data/music-uncompressed$FILE_NAME" >> ../default/makefile
REBUILD_STRING="../data/music-uncompressed/$MUS_NAME.inc: ../resources/midi/$MUS_NAME.mid
	\$(UZEBIN_DIR)/midiconv -f 60 ../resources/midi/$MUS_NAME.mid ../data/music-uncompressed/$MUS_NAME.inc -vmus_track1$MUS_NAME
"

	echo "$REBUILD_STRING" >> music-rebuild-rules.inc
done

echo '#../data/music-compressed.inc: $(MIDI_UNCOMPRESSED_LIST)
	\$(UZEBIN_DIR)/mconvert ../data/music-compression.cfg' >> music-rebuild-rules.inc

#Build all sound effects resources, setup makefile data, and generate SD data
> sfx-resource-list.inc
> sfx-rebuild-rules.inc


echo '

## Build
all: $(GFX_RESOURCES) $(MUSIC_RESOURCES) $(SFX_RESOURCES) $(TARGET) $(GAME).hex $(GAME).eep $(GAME).lss $(GAME).uze size

## Rebuild resources' >> ../default/makefile

RES=$(cat gfx-rebuild-rules.inc)
echo "$RES" >> ../default/makefile
rm gfx-rebuild-rules.inc
echo "" >> ../default/makefile

RES=$(cat music-rebuild-rules.inc)
echo "$RES" >> ../default/makefile
rm music-rebuild-rules.inc
echo "" >> ../default/makefile

echo '## Compile Kernel files
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

UNAME := $(shell sh -c ''uname -s 2>/dev/null || echo not'')
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
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)' >> ../default/makefile

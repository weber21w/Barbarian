#ifndef _BARBARIAN_H
#define _BARBARIAN_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include <uzebox.h>
#include <spiram.h>
#include <sdBase.h>

#ifndef BARB_USE_UZEBOX_SOUND
#define BARB_USE_UZEBOX_SOUND 1
#endif

#ifndef BARB_ENABLE_SPRITE_FLASH_DATA
#define BARB_ENABLE_SPRITE_FLASH_DATA 0
#endif

#if BARB_USE_UZEBOX_SOUND
#include "data/patches.inc"
#endif

#if BARB_ENABLE_SPRITE_FLASH_DATA
#include "data/sprite-data.inc"
#include "data/sprite-defs.inc"
#else
#include "data/sprite-ids.inc"
#endif

#include "data/bg-defs.inc"

#ifndef M74_M4_BASE
#define M74_M4_BASE 0
#endif

#define BARB_LOGICAL_W		192u
#define BARB_LOGICAL_H		120u
#define BARB_ORIG_SCALE_SHIFT	2u

#if BARB_SFA_VIDEO
#define BARB_NUM_SPRITES 4u /* The compositor and submission limit are four. */
#else
#define BARB_NUM_SPRITES 8u
#endif
#define BARB_MAX_SIMUL_SFX	2u
#define BARB_SPRITE_FLIP_X	0x80u
#define BARB_SPRITE_PLAYER_B	0x01u

#ifndef BARB_PLAYER_B_COLOR_INDEX
#define BARB_PLAYER_B_COLOR_INDEX	3u
#endif

/*
 * Mode 748 row mode 4, matching the prepared 192x120 fight backgrounds.
 * Each scanline is split exactly like the reference demo:
 *   first 12 bytes / 24 pixels are stored in SRAM row data
 *   next 84 bytes / 168 pixels are streamed from SPI RAM
 */
#define BARB_VIEW_W			192u
#define BARB_BG_SRC_H			120u
#ifndef BARB_BG_CROP_TOP
#define BARB_BG_CROP_TOP		8u
#endif
#if (BARB_BG_CROP_TOP >= BARB_BG_SRC_H)
#error BARB_BG_CROP_TOP must be less than BARB_BG_SRC_H
#endif
#define BARB_VIEW_H			(BARB_BG_SRC_H - BARB_BG_CROP_TOP)
#define BARB_FB_ROW_BYTES		(BARB_VIEW_W / 2u)          /* 96 */
#define BARB_M74_SRAM_ROW_BYTES	12u                       /* first 24 px */
#define BARB_SPI_ROW_BYTES		(BARB_FB_ROW_BYTES - BARB_M74_SRAM_ROW_BYTES) /* 84 */
#define BARB_M74_ROWS			((BARB_VIEW_H + 7u) / 8u)
#define BARB_FB_BYTES			((u32)BARB_FB_ROW_BYTES * (u32)BARB_VIEW_H)


#ifndef BARB_DEBUG_WALK_SPRITE
#define BARB_DEBUG_WALK_SPRITE 0
#endif

#ifndef BARB_DEBUG_FRAME_VSYNC_COUNTER
#define BARB_DEBUG_FRAME_VSYNC_COUNTER 0
#endif

#ifndef BARB_DEBUG_SKIP_GAMEPLAY
#define BARB_DEBUG_SKIP_GAMEPLAY 0
#endif


#ifndef BARB_DEBUG_BG_AUTO_FRAMES
#define BARB_DEBUG_BG_AUTO_FRAMES 240u
#endif

#ifndef BARB_M748_RENDER_START
#define BARB_M748_RENDER_START	104u
#endif
#ifndef BARB_M748_RENDER_LINES
#define BARB_M748_RENDER_LINES	112u
#endif
#if (BARB_M748_RENDER_LINES < BARB_VIEW_H)
#error BARB_M748_RENDER_LINES must be at least BARB_VIEW_H
#endif

/*
 * Mode 748 row-mode-4 still consumes a larger right-side SPI stream than
 * the cropped 120-line playfield in some timing configurations. Keep the
 * active SPI display allocation large enough that scanout never runs into
 * the clean BG / sprite banks. The SRAM left strip is still only allocated
 * for BARB_M74_ALLOC_ROWS; tail-row SRAM writes must be bounds checked.
 */
#define BARB_M74_SCAN_H		216u
#define BARB_SPI_DISPLAY_BYTES	((u32)BARB_SPI_ROW_BYTES * (u32)BARB_M74_SCAN_H)

#define BARB_SPIVRAM_DISPLAY	((u32)M74_M4_BASE)
/*
 * Active Mode 748 display consumes a compact right-side SPI stream for all
 * scanlines. The 24-pixel left strip is still in SRAM vramrow[].
 *
 * Active-direct renderer keeps one full 192x120 clean BG restore copy in
 * SPI RAM. Sprites are restored/drawn directly against the active split
 * Mode 748 framebuffer.
 */
#define BARB_SPIVRAM_BG_CLEAN	(BARB_SPIVRAM_DISPLAY + BARB_SPI_DISPLAY_BYTES)
#define BARB_SPIVRAM_SPRITES	(BARB_SPIVRAM_BG_CLEAN + BARB_FB_BYTES)
#define BARB_SPIVRAM_RESTORE	BARB_SPIVRAM_BG_CLEAN
#define BARB_SPIROFF_SFX_DATA	((u32)65536UL)

#if BARB_SFA_VIDEO
/* SFA2's 160-pixel packed SPI raster; game coordinates remain 192 pixels. */
#undef BARB_VIEW_W
#undef BARB_VIEW_H
#undef BARB_FB_ROW_BYTES
#undef BARB_FB_BYTES
#undef BARB_SPIVRAM_DISPLAY
#undef BARB_SPIVRAM_BG_CLEAN
#undef BARB_SPIVRAM_SPRITES
#include "data/platform-layout.h"
#endif
#ifndef BARB_WORKBUF_BYTES
#define BARB_WORKBUF_BYTES 512u
#endif

#define BARB_MAX_DIRTY_RECTS	8u
#define BARB_MAX_DIRTY_SPANS	128u
#define BARB_SPI_SEQ_SETUP_CYCLES	100u
#define BARB_SPI_BYTE_CYCLES	18u
#ifndef BARB_SPI_FRAME_BUDGET
#define BARB_SPI_FRAME_BUDGET	24000u
#endif

#ifndef BARB_SPI_YIELD_ENABLE
#define BARB_SPI_YIELD_ENABLE	0u
#endif

#ifndef BARB_DEBUG_SPI_SPRITE_SOURCE
#define BARB_DEBUG_SPI_SPRITE_SOURCE 1u
#endif

#ifndef BARB_DEBUG_FLASH_SPRITE_SOURCE
#define BARB_DEBUG_FLASH_SPRITE_SOURCE 0u
#endif

#include "data/sprite-spanfmt.inc"


/*
 * packrom writes a 512-byte .uze header first, then up to 60 KiB of padded
 * program data, then the raw dependency bytes. sdBase reads raw sectors from
 * the .UZE file on SD, so dependency offsets are byte offsets inside that file.
 */
#ifndef BARB_PACKROM_HEADER_BYTES
#define BARB_PACKROM_HEADER_BYTES	512UL
#endif
#ifndef BARB_PACKROM_ROM_BYTES
#define BARB_PACKROM_ROM_BYTES		(60UL * 1024UL)
#endif
#ifndef BARB_PACKROM_DEP_BASE
#define BARB_PACKROM_DEP_BASE		(BARB_PACKROM_HEADER_BYTES + BARB_PACKROM_ROM_BYTES)
#endif

#ifndef SDOFF_GFX_DATA
#define SDOFF_GFX_DATA		BARB_PACKROM_DEP_BASE
#endif
#ifndef SDOFF_SPRITE_DATA
#define SDOFF_SPRITE_DATA	(BARB_PACKROM_DEP_BASE + BG_DATA_SIZE)
#endif
#ifndef SDOFF_SFX_DATA
#if BARB_SFA_VIDEO
#define SDOFF_SFX_DATA          (SDOFF_SPRITE_DATA + BARB_SPR_FILE_BYTES)
#else
#define SDOFF_SFX_DATA		(SDOFF_SPRITE_DATA + BARB_SPR_DATA_SIZE)
#endif
#endif


#define BARB_SFX_BELL		0u
#define BARB_SFX_BLOCK1		1u
#define BARB_SFX_BLOCK2		2u
#define BARB_SFX_BLOCK3		3u
#define BARB_SFX_DEATHFALL	4u
#define BARB_SFX_DECAPITATED	5u
#define BARB_SFX_FALL		6u
#define BARB_SFX_FIRE		7u
#define BARB_SFX_FOOTCUT	8u
#define BARB_SFX_GUARD		9u
#define BARB_SFX_HEAD1		10u
#define BARB_SFX_HEAD2		11u
#define BARB_SFX_HEADHIT	12u
#define BARB_SFX_HIT1		13u
#define BARB_SFX_HIT2		14u
#define BARB_SFX_HIT3		15u
#define BARB_SFX_LAUGH		16u
#define BARB_SFX_MISS		17u
#define BARB_SFX_NECKSWING	18u
#define BARB_SFX_PAIN1		19u
#define BARB_SFX_PAIN2		20u
#define BARB_SFX_PREPARE	21u
#define BARB_SFX_ROLL		22u
#define BARB_SFX_WAIT		23u
#define BARB_SFX_COUNT		24u

#ifndef BARB_AUDIO_STAGE_SLOT_BYTES
#define BARB_AUDIO_STAGE_SLOT_BYTES	12288u
#endif
#ifndef BARB_AUDIO_STAGE_CHUNK_BYTES
#define BARB_AUDIO_STAGE_CHUNK_BYTES	64u
#endif
#ifndef BARB_AUDIO_STAGE_CHUNKS_PER_FRAME
#define BARB_AUDIO_STAGE_CHUNKS_PER_FRAME	2u
#endif
#if (BARB_AUDIO_STAGE_CHUNK_BYTES > BARB_FB_ROW_BYTES)
#error BARB_AUDIO_STAGE_CHUNK_BYTES must fit in barbLineBuf
#endif
#if (BARB_AUDIO_STAGE_CHUNK_BYTES > 255u)
#error BARB_AUDIO_STAGE_CHUNK_BYTES must fit in u8
#endif
#if (BARB_SPRITE_CACHE_BYTES > BARB_WORKBUF_BYTES)
#error BARB_SPRITE_CACHE_BYTES must fit in workBuf
#endif

#define BARB_PAD_NONE		0u
#define BARB_PAD_UP		0x0001u
#define BARB_PAD_DOWN		0x0002u
#define BARB_PAD_LEFT		0x0004u
#define BARB_PAD_RIGHT		0x0008u
#define BARB_PAD_ATTACK		0x0010u
#define BARB_PAD_START		0x0020u
#define BARB_PAD_SELECT		0x0040u


typedef struct{
        s16 x;
        s16 y;
        u8 f;
        u8 p;
} BarbSprite;

typedef struct{
        u8 x;
        u8 y;
        u8 w;
        u8 h;
} BarbRect;

typedef struct{
        BarbRect rect[BARB_MAX_DIRTY_RECTS];
        u8 count;
} BarbDirtyList;

typedef struct{
        u16 p1;
        u16 p2;
} BarbarianInput;

typedef struct{
        /* Keep hot fields near the start of the struct. AVR indexed addressing is much
         * smaller for offsets below 64 bytes, and the frame logic touches these fields
         * constantly. This is layout-only; all gameplay access is by field name.
         */
        s16 temps;
        s16 reftemps;
        s16 reftempsB;
        s16 attenteA;
        s16 attenteB;
        s16 chrono;
        s16 ySPRt;
        s16 xSPRt;
        s16 refserpentA;
        s16 refserpentB;
        u16 frame;
        u16 randSeed;

        s8 GOTOU;
        s8 xA;
        s8 xB;
        s8 joueurB;
        s8 joueurA;
        s8 spriteB;
        s8 spriteA;
        s8 Boccupe;
        s8 Aoccupe;
        s8 infocoupB;
        s8 xAttB;
        s8 choixCLING;
        s8 songrogne;
        s8 xAttA;
        s8 distance;
        s8 xMB;
        s8 xMA;
        s8 xGA;
        s8 xGB;
        s8 xTA;
        s8 xTB;
        s8 numerotete;
        s8 xFA;
        s8 xFB;
        s8 demo;
        s8 IA;
        s8 sens;
        s8 infocoupA;
        s8 levier2;
        s8 levier1;
        s8 decorS;
        s8 yGA;
        s8 attaqueA;
        s8 attaqueB;
        s8 yAttB;
        s8 yGB;
        s8 yAttA;
        s8 sangA;
        s8 sangB;
        s8 choixTOUCHE;
        s8 yTA;
        s8 partie;
        s8 transition;
        s8 spritereculeA;
        s8 spritereculeB;
        s8 infoBdegatG;
        s8 decapiteA;
        s8 decapiteB;
        s8 sortieA;
        s8 serpentA;
        s8 x1;
        s8 x2;
        s8 xGNOME;
        s8 y1;
        s8 y2;
        s8 entree;
        s8 sortieB;
        s8 spriteavanceA;
        s8 spriteavanceB;
        s8 yTB;
        s8 serpentB;
        s8 chronometre;
        s8 gnome;
        s8 vieA;
        s8 vieB;
        s8 positionSANG;
        s8 sangdecapA;
        s8 sangdecapB;
        s8 sorcier;
        s8 transitionfinie;
        s8 yMA;
        s8 yMB;
        s8 yFA;
        s8 yFB;
        s8 infoAdegatG;
        s8 entreesorcier;
        s8 infoBdegatT;
        s8 jeu;
        s8 protegeDA;
        s8 protegeDB;
        s8 protegeHA;
        s8 protegeHB;
        s8 infoBdegatF;
        s8 assisA;
        s8 assisB;
        s8 marianna;
        s8 couleurok;
        s8 gnometire;
        s8 infoAdegatF;
        s8 infoAdegatT;
        s8 numerognome;
        s8 tempsfini;
        s8 arret;
        s8 football;
        s8 bonusA;
        s8 bonusB;
        s8 lancerintro;
        s8 version;
        s8 continuer;
        s8 lancerfin;

        s32 scoreA;
        s32 scoreB;
        s8 chronoPart; /* Remaining tenths of the Amiga round counter, 0..9. */
} BarbarianState;

extern unsigned char workBuf[BARB_WORKBUF_BYTES];
extern BarbSprite spr[BARB_NUM_SPRITES];
extern u8 nextSprite;

void BarbarianInit(BarbarianState *g);
void BarbarianReadInput(BarbarianInput *in);
void BarbarianTick(BarbarianState *g, const BarbarianInput *in);
void BarbarianRenderFrame(const BarbarianState *g);
void BarbarianAudioVsync(void);

void CleanupSprite(u8 s);
void ResetSprites(void);
void BlitSprite(s16 x, s16 y, u8 f, u8 p);
void LoadPalette(const unsigned char *p);
void LoadBG(u8 b);
void PlaySfx(u8 s);

#endif

#ifndef BARB_RUNTIME_BASE
#define BARB_RUNTIME_BASE (BARB_SPIVRAM_SPRITES+BARB_SPR_DATA_SIZE)
#endif

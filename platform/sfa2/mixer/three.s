/* Three independent SRAM-fed decoders: resident PCM4 music + two ADPCM2 voices.
 * One packed-byte fetch per line: music, fighter 0, music, fighter 1.
 * No SPI access in this ISR. All 262 whole-line calls output a sample,
 * including VSYNC half-line pairs; ZL retains the stock sync-mode ABI. */
#include <avr/io.h>
#include "../../audio.h"
#include <defines.h>
#if AUDIO_OUT_HSYNC_CYCLES != 240 || AUDIO_OUT_VSYNC_CYCLES != 240
#error Three-channel mixer requires the 240/240 cycle contract
#endif
#if !BARB_SID
#error Continuous 240-cycle mixer requires resident PCM4 music (BARB_SID=1)
#endif
#define PHASE _SFR_IO_ADDR(GPIOR2)
#define MUSIC _SFR_IO_ADDR(GPIOR0)
#define SFX0 _SFR_IO_ADDR(GPIOR1)
#define SYNC _SFR_IO_ADDR(SYNC_PORT)
.global update_sound,update_sound_buffer,update_sound_buffer_2,update_sound_buffer_fast,process_music,MixSound,sound_enabled
.section .bss
sound_enabled: .space 1
.section .text
process_music:
MixSound: ret
update_sound_buffer:
update_sound_buffer_fast:
 ldi ZL,6
1: dec ZL
 brne 1b
 nop
 nop
 ret
update_sound_buffer_2:
 ldi ZL,19
1: dec ZL
 brne 1b
 nop
 ret
.macro PAD N
 .if (\N)>=3
 ldi r19,((\N)/3)
1: dec r19
 brne 1b
 .endif
 .rept ((\N)%3)
 nop
 .endr
.endm
.macro DECODE NAME, STATE, ACC, DEST, PACK, BITS, SRAM=0, DEFER=0
 .if \SRAM
 lds r16,\PACK
 .else
 in r16,\PACK
 .endif
 .if \BITS==4
 mov ZL,r16
 andi ZL,15
 swap r16
 andi r16,15
 .else
 mov r17,r16
 andi r17,3
 lsr r16
 lsr r16
 .endif
 .if \SRAM
 sts \PACK,r16
 .else
 out \PACK,r16
 .endif
 .if \BITS==4
 lds r18,\STATE
 or ZL,r18
 ldi ZH,hi8(audio_music_delta)
 .else
 lds ZL,\STATE
 lsl ZL
 ldi ZH,hi8(audio_aux_delta)
 adc ZH,r1
 or ZL,r17
 .endif
 lpm r0,Z
 .if \BITS==4
 inc ZH
 .else
 subi ZH,-2
 .endif
 lpm r18,Z
 sts \STATE,r18
 lds \DEST,\ACC
 .if !\DEFER
 add \DEST,r0
 sts \ACC,\DEST
 .endif
.endm
.macro DECODE_MUSIC
#if BARB_SID
 /* Nine-cycle PCM4 decode leaves room for the early VSYNC sync edge. */
 in r16,MUSIC
 mov r20,r16
 andi r20,15
 swap r20
 swap r16
 andi r16,15
 out MUSIC,r16
 sts spiram_adpcm_music_accumulator,r20

#else
 DECODE music,spiram_adpcm_music_adpcm_state,spiram_adpcm_music_accumulator,r20,MUSIC,4
#endif
.endm
.macro DECODE_SFX0
 DECODE aux,spiram_adpcm_sfx_adpcm_state,spiram_adpcm_sfx_accumulator,r21,SFX0,2
.endm
.macro DECODE_SFX1
 DECODE aux,spiram_adpcm_sfx_adpcm_state+1,spiram_adpcm_sfx_accumulator+1,r22,audio_sfx_packed1,2,1,1
.endm
/* T marks an empty ring; ZL preserves the tail, r17 the next packed byte.
 * Prefetch is read-only. Commit below checks active/pending publication. */
.macro PREFETCH HEAD,TAIL,RING
 lds r18,\TAIL
 lds r16,\HEAD
 cp r18,r16
 breq .Lempty\@
 mov ZL,r18
 ldi ZH,0
 subi ZL,lo8(-(\RING))
 sbci ZH,hi8(-(\RING))
 ld r17,Z
 clt
 rjmp .Lprefetched\@
.Lempty\@:
 set
 clr r17
 PAD 6
.Lprefetched\@:
 mov ZL,r18
.endm
.macro SCALE DEST,VOL
 lds r18,\VOL
 mul \DEST,r18
 mov \DEST,r1
 lsr r18
 sub \DEST,r18
 subi \DEST,128
.endm
audio_mix_three:
 SCALE r20,spiram_adpcm_music_volume
 SCALE r21,spiram_adpcm_sfx_volume
 SCALE r22,spiram_adpcm_sfx_volume+1
 clr r1
 ldi r19,255
 add r20,r21
 adc r19,r1
 add r20,r22
 adc r19,r1
 tst r19
 breq 1f
 ldi r20,255
 sbrc r19,7
 clr r20
2: sts _SFR_MEM_ADDR(OCR2A),r20
 ret
1: rjmp 2b
.macro COMMIT_TAIL TAIL,SIZE,PACK,SRAM=0
 mov r18,ZL
 inc r18
 cpi r18,\SIZE
 brne 1f
 clr r18
1: sts \TAIL,r18
 .if \SRAM
 sts \PACK,r17
 .else
 out \PACK,r17
 .endif
.endm
/* All post workers are padded to the same linked instruction budget. */
audio_commit_music:
 brts .Lm_empty
 lds r16,spiram_adpcm_music_active
 tst r16
 breq .Lm_inactive
 lds r16,audio_music_reset_pending
 tst r16
 breq .Lm_regular
 lds r16,audio_music_reset_at
 cp ZL,r16
 brne .Lm_other
 sts spiram_adpcm_music_adpcm_state,r1
 ldi r16,128
 sts spiram_adpcm_music_accumulator,r16
 sts audio_music_reset_pending,r1
 ldi r16,1
 sts spiram_adpcm_music_boundary_pending,r16
 PAD 2
 rjmp .Lm_commit
.Lm_other: PAD 11
 rjmp .Lm_commit
.Lm_regular: PAD 15
 rjmp .Lm_commit
.Lm_commit:
 COMMIT_TAIL audio_music_tail,BARB_MUSIC_FIFO_BYTES,MUSIC
 rjmp audio_return_normal
.Lm_inactive: PAD 29
 rjmp audio_return_normal
.Lm_empty:
 lds r16,spiram_adpcm_music_active
 tst r16
 breq .Lm_empty_idle
 lds r16,audio_music_eof
 tst r16
 breq .Lm_starved
 sts spiram_adpcm_music_active,r1
 sts spiram_adpcm_music_volume,r1
 PAD 21
 rjmp audio_return_normal
.Lm_starved:
 ldi r16,1
 sts audio_music_underruns,r16
 PAD 21
 rjmp audio_return_normal
.Lm_empty_idle: PAD 28
 rjmp audio_return_normal
.macro SFX_COMMIT SLOT,PACK,SRAM=0
 brts .Ls_empty\@
 lds r16,spiram_adpcm_sfx_active+\SLOT
 tst r16
 breq .Ls_inactive\@
 cpi r16,2
 brne .Ls_regular\@
 sts spiram_adpcm_sfx_adpcm_state+\SLOT,r1
 ldi r16,128
 sts spiram_adpcm_sfx_accumulator+\SLOT,r16
 ldi r16,1
 sts spiram_adpcm_sfx_active+\SLOT,r16
 lds r16,spiram_adpcm_sfx_pending_volume+\SLOT
 sts spiram_adpcm_sfx_volume+\SLOT,r16
 rjmp .Ls_commit\@
.Ls_regular\@: PAD 11
 rjmp .Ls_commit\@
.Ls_commit\@:
 COMMIT_TAIL audio_sfx_tail+\SLOT,96,\PACK,\SRAM
 PAD (6-\SRAM)
 rjmp audio_return_normal
.Ls_inactive\@: PAD 29
 rjmp audio_return_normal
.Ls_empty\@:
 lds r16,spiram_adpcm_sfx_active+\SLOT
 cpi r16,1
 brne .Ls_empty_idle\@
 lds r16,audio_sfx_eof+\SLOT
 tst r16
 breq .Ls_starved\@
 sts spiram_adpcm_sfx_active+\SLOT,r1
 sts spiram_adpcm_sfx_volume+\SLOT,r1
 PAD 21
 rjmp audio_return_normal
.Ls_starved\@:
 ldi r16,1
 sts audio_sfx_underruns+\SLOT,r16
 PAD 21
 rjmp audio_return_normal
.Ls_empty_idle\@: PAD 28
 rjmp audio_return_normal
.endm
audio_commit_sfx0: SFX_COMMIT 0,SFX0
audio_commit_sfx1: SFX_COMMIT 1,audio_sfx_packed1,1
.macro RETURN_NORMAL
 pop r23
 pop r22
 pop r21
 pop r20
 pop r19
 pop r18
 pop r17
 pop r16
 ret
.endm
.macro PHASE_BODY HEAD,TAIL,RING,WORKER
 PREFETCH \HEAD,\TAIL,\RING
 PAD 8
 sbrc r23,1
 sbi SYNC,SYNC_PIN
 sbrs r23,1
 rjmp .
 PAD 3
 add r22,r0
 sts spiram_adpcm_sfx_accumulator+1,r22
 rcall audio_mix_three
 rjmp \WORKER
.endm
audio_phase_music: PHASE_BODY audio_music_head,audio_music_tail,audio_music_ring,audio_commit_music
audio_phase_sfx0: PHASE_BODY audio_sfx_head,audio_sfx_tail,audio_sfx_ring,audio_commit_sfx0
audio_phase_sfx1: PHASE_BODY audio_sfx_head+1,audio_sfx_tail+1,audio_sfx_ring+96,audio_commit_sfx1
audio_return_normal: RETURN_NORMAL
audio_phase_table:
 jmp audio_phase_music
 jmp audio_phase_sfx0
 jmp audio_phase_music
 jmp audio_phase_sfx1
update_sound:
 .irp reg,16,17,18,19,20,21,22,23
 push r\reg
 .endr
 clr r1
 mov r23,ZL
 DECODE_MUSIC
 DECODE_SFX0
 PAD 4
 sbrs r23,0
 rjmp .Lno_early_sync
 sbi SYNC,SYNC_PIN
 rjmp .Learly_sync_done
.Lno_early_sync:
 PAD 3
.Learly_sync_done:
 DECODE_SFX1
 in r16,PHASE
 inc r16
 andi r16,3
 out PHASE,r16
 ldi ZL,lo8(pm(audio_phase_table))
 ldi ZH,hi8(pm(audio_phase_table))
 lsl r16
 add ZL,r16
 adc ZH,r1
 ijmp
#include "decoder_tables.inc"

#pragma once
#define BARB_MUSIC_FIFO_BYTES 192
#define BARB_AUDIO_DATA_BYTES 49392UL
#ifndef __ASSEMBLER__
/* Q0.8 gains: all three full-scale inputs must fit with DAC headroom. */
#define BARB_AUDIO_MUSIC_GAIN 32U
#define BARB_AUDIO_SFX_GAIN 88U
#if BARB_AUDIO_MUSIC_GAIN + 2 * BARB_AUDIO_SFX_GAIN > 240
#error Audio gains exceed the shared headroom budget
#endif
void AudioRefill(void);
void AudioPlay(s8 channel,u8 sample,s8 loops);
void AudioHalt(s8 channel);
extern u8 barbDeathBellFields;
extern u8 barbDeathBellRings;
/* The original decapitation recording leads its bells by about 0.15 s more. */
#define AudioDeathBell(sample) (barbDeathBellRings=8,barbDeathBellFields=((sample)==BARB_SFX_DECAPITATED?95:86))
void AudioAdvance(u8 fields);
void AudioBusWait(u16 bytes);
void SidPrepare(void);
void SidStart(void);
void SidSelect(u8 song);
u8 SidDiskRead(u32 offset,u8 *dest,u8 bytes);

#endif

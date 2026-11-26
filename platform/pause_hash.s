/* CRC32 of the canonical game, desired scene and three rollback pause bytes.
 * Fits the otherwise unreachable 152-byte auxiliary-next table tail. */
#if BARB_NET
.global BarbarianNetworkHash
.type BarbarianNetworkHash,@function
BarbarianNetworkHash:
 ldi r22,255
 mov r23,r22
 movw r24,r22
 ldi ZL,lo8(barbPauseCore)
 ldi ZH,hi8(barbPauseCore)
 ldi r20,133
.Lpause_hash_game:
 ld r18,Z+
 rcall .Lpause_crc_byte
 dec r20
 brne .Lpause_hash_game
 lds r18,barbPauseScene
 rcall .Lpause_crc_byte
 ldi ZL,lo8(barbPauseState)
 ldi ZH,hi8(barbPauseState)
 ldi r20,3
.Lpause_hash_menu:
 ld r18,Z+
 rcall .Lpause_crc_byte
 dec r20
 brne .Lpause_hash_menu
 com r22
 com r23
 com r24
 com r25
 ret
.Lpause_crc_byte:
 eor r22,r18
 ldi r19,8
.Lpause_crc_bit:
 lsr r25
 ror r24
 ror r23
 ror r22
 brcc .Lpause_crc_next
 ldi r18,0x20
 eor r22,r18
 ldi r18,0x83
 eor r23,r18
 ldi r18,0xb8
 eor r24,r18
 ldi r18,0xed
 eor r25,r18
.Lpause_crc_next:
 dec r19
 brne .Lpause_crc_bit
 ret
.size BarbarianNetworkHash,.-BarbarianNetworkHash
#endif

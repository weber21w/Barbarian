/* Uzebox standard SNES polling, specialized for Barbarian.
 * GPL-3.0-or-later; original kernel copyright Alec Bourque, 2008-2009.
 *
 * Both ports are sampled together with eight clocks. B is fire/confirm and
 * Y is menu back/offline test, so no high button bits are needed. The game
 * consumes button states only: omit the unused plug-detection probe, which
 * requires clocking the controller's tail. A fresh latch starts every poll.
 * High status bytes are always cleared. Preserve the stock latch and explicit
 * settling delays; do not mask the reentrant sync/audio interrupt.
 */
#include <avr/io.h>
#include "defines.h"
#if JOYSTICK != TYPE_SNES || SNES_MOUSE
#error Barbarian controller assembly requires standard SNES pads without mouse
#endif
.macro PAD_WAIT12
 lpm r0,Z
 lpm r0,Z
 lpm r0,Z
 lpm r0,Z
.endm
.global ReadControllers,ReadButtons
.section .text.ReadButtons,"ax",@progbits
ReadControllers:
ReadButtons:
 sbi _SFR_IO_ADDR(JOYPAD_OUT_PORT),JOYPAD_LATCH_PIN
 PAD_WAIT12
 cbi _SFR_IO_ADDR(JOYPAD_OUT_PORT),JOYPAD_LATCH_PIN
 ldi r20,8
.Lpad_bit:
 PAD_WAIT12
 cbi _SFR_IO_ADDR(JOYPAD_OUT_PORT),JOYPAD_CLOCK_PIN
 in r18,_SFR_IO_ADDR(JOYPAD_IN_PORT)
 lsr r18
 ror r24
 lsr r18
 ror r22
 sbi _SFR_IO_ADDR(JOYPAD_OUT_PORT),JOYPAD_CLOCK_PIN
 PAD_WAIT12
 dec r20
 brne .Lpad_bit
 com r24
 com r22
 sts joypad1_status_lo,r24
 sts joypad2_status_lo,r22
 sts joypad1_status_lo+1,r1
 sts joypad2_status_lo+1,r1
 cpi r24,15
 breq .Lpad_reset
 cpi r22,15
 brne .Lpad_done
.Lpad_reset:
 jmp SoftReset
.Lpad_done:
 ret

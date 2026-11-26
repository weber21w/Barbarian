/* Included inside the unused auxiliary-delta LUT tail. Only states 0..89
 * access the preceding 360 bytes; keep all executable code within 152 bytes.
 * u8 PauseStep(const BarbarianInput *): shared menu mode/owner and pad edges.
 */
.global PauseStep
.type PauseStep,@function
PauseStep:
 movw XL,r24
 ld r18,X+
 adiw XL,1
 ld r19,X
 lds r20,barbPauseState
 lds r21,barbPauseState+1
 lds r22,barbPauseState+2
 sts barbPauseState+1,r18
 sts barbPauseState+2,r19
 com r21
 and r21,r18
 com r22
 and r22,r19
 tst r20
 brne .Lpause_active
 or r22,r21
 andi r22,32
 breq .Lpause_zero
 ldi r20,1
 sbrs r21,5
 ori r20,128
 rjmp .Lpause_store
.Lpause_active:
 sbrs r20,7
 rjmp .Lpause_owner
 mov r18,r19
 mov r21,r22
.Lpause_owner:
 mov r22,r20
 andi r22,127
 cpi r22,3
 brne .Lpause_select
 andi r18,112
 brne .Lpause_one
 clr r20
 rjmp .Lpause_store
.Lpause_select:
 cpi r22,3
 brsh .Lpause_one
 mov r22,r21
 andi r22,3
 breq .Lpause_confirm
 ldi r22,3
 eor r20,r22
.Lpause_confirm:
 sbrc r21,6
 rjmp .Lpause_release
 andi r21,48
 breq .Lpause_store
 mov r22,r20
 andi r22,127
 cpi r22,1
 breq .Lpause_release
 andi r20,128
 ori r20,4
 rjmp .Lpause_store
.Lpause_release:
 andi r20,128
 ori r20,3
.Lpause_store:
 sts barbPauseState,r20
.Lpause_one:
 ldi r24,1
 ret
.Lpause_zero:
 clr r24
 ret
.size PauseStep,.-PauseStep

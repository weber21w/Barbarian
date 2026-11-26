"""Packed pixels at exactly nine clocks each, including the loop boundary."""
def generate(out):
    # Read the completed transfer before starting the next one. IN is 17 clocks
    # after the previous OUT, and successive OUTs remain 18 clocks apart.
    # Pixel DAC writes and the 18-cycle packed-pair cadence are unchanged.
    code=['walk_kernel_0:','ldi ZH,hi8(walk_palette)','ldi r21,78',
          'walk_pixel_pair:','mov ZL,r10','swap ZL','andi ZL,15','ld r16,Z','out VIDEO_PORT,r16',
          'nop','nop','mov ZL,r10','andi ZL,15','ld r16,Z','in r10,_SFR_IO_ADDR(SPDR)',
          'out _SFR_IO_ADDR(SPDR),r2','out VIDEO_PORT,r16','dec r21',
          'brne walk_pixel_pair']
    # The untaken branch saves the cycle spent initializing the loop. Two
    # unrolled final pairs finish the real prefetch and avoid a dummy SPI read.
    for pair in (78,79):
        code+=['mov ZL,r10','swap ZL','andi ZL,15','ori ZL,0','ld r16,Z','out VIDEO_PORT,r16',
               'nop','nop','nop','mov ZL,r10','andi ZL,15','ori ZL,0','ld r16,Z','out VIDEO_PORT,r16',
               'in r10,_SFR_IO_ADDR(SPDR)' if pair==78 else 'nop','nop']
    code+=['jmp walk_end']
    (out/'kernels.inc').write_text('\n'.join(code)+'\n')

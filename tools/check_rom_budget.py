"""Reject ROMs that overlap PackRom's 60 KiB appended-data boundary."""
from pathlib import Path
import subprocess
import sys

elf = Path(sys.argv[1])
sections = {}
for line in subprocess.check_output(['avr-size', '-A', str(elf)], text=True).splitlines():
    values = line.split()
    if len(values) >= 2 and values[0].startswith('.'):
        sections[values[0]] = int(values[1])
flash = sections.get('.text', 0) + sections.get('.data', 0)
sram = sum(sections.get(name, 0) for name in ('.data', '.bss', '.noinit'))
if not flash:
    raise SystemExit(f'No program sections found in {elf}')
if flash > 60 * 1024:
    raise SystemExit(f'Flash exceeds the 60 KiB application boundary by {flash - 60 * 1024} bytes')
if sram > 4096 - 256:
    raise SystemExit(f'Static SRAM ({sram} bytes) leaves less than 256 bytes for stack')
print(f'ROM budget: {flash}/61440 flash, {sram}/4096 static SRAM; '
      f'{61440-flash} flash bytes free, {4096-sram} bytes available for stack')

# Reproduction details

The supported release is the pair of complete `.uze` files in
`release/expected.json`. Byte equality covers the 512-byte header, AVR program,
FF padding to the 60 KiB program boundary, and all 181,149 embedded asset bytes.
The two ROMs differ in executable code and network compatibility identity;
their embedded asset payloads are identical.

Tested package versions on Ubuntu 22.04:

| Package | Version |
| --- | --- |
| gcc-avr | `1:5.4.0+Atmel3.6.2-3` |
| binutils-avr | `2.26.20160125+Atmel3.6.2-4` |
| avr-libc | `1:2.0.0+Atmel3.6.2-3` |
| Python | `3.10.12` |
| NumPy | `1.21.5` |
| Pillow | `11.3.0` |
| Native g++ | `11.4.0` |

Different compiler versions may produce different AVR code. Verification fails
if either complete output differs, even when it still fits the ROM budget.
The native PackRom executable is generated locally from `tools/packrom.cpp`.

The source cleanup removes the legacy Mode 748 backend, original-disk analysis
tools, unused kernel and SDK code, separate raw audio duplicates, frozen dumps,
generated XML/C asset copies, packed outputs, and old development documents.
Canonical logic and animation sources are retained directly, rather than
requiring an obsolete translation pipeline and its duplicate source tree.

The supported build is intentionally the published configuration: networking,
resident SID music, SPI rendering, and either full or public-display edition.
Historical alternative renderer/audio configurations are not supported here.

The original development workspace, source references, original ROMs and full
verification evidence remain outside this repository. The release's frozen
verification reports are also available alongside the public downloads. No
Kickstart ROM, ADF, emulator executable, private key or credential is included.

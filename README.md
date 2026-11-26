# Barbarian for Uzebox

This is the self-contained source for the published UzeNet build 22. It builds
both editions byte-for-byte, including the header, padded program and embedded
graphics, effects and music. No other checkout or original Amiga ROM is needed.

| Edition | Output | SHA-256 |
| --- | --- | --- |
| Full blood and decapitation | `default/BARBARIN.UZE` | `18f6150e15c6c70ad0c9d3fbaf0a687de8a354558f4356e02cfdd15b6a9fa944` |
| Public display, no blood or decapitation | `default/BARBCENS.UZE` | `0fbd5fefe28f134fca71c533ff7886e239af5d73c6682dfb0155fd446e27b3cd` |

Each output is 243,101 bytes. Copy the selected `.uze` to the SD root and keep
its filename. Each contains all required assets through PackRom `dependency=`.
Requires an SD card and 128 KiB SPI RAM. Both UzeNet players need the same
edition and release. [Downloads and release notes](https://uzenet.us/roms.html#barbarian).

## Reproduce the release

The verified toolchain is Ubuntu 22.04, AVR GCC 5.4.0,
AVR binutils 2.26.20160125 and avr-libc 2.0.0. Python asset dependencies are
pinned in `requirements.txt`. A native C++ compiler builds the included PackRom.

```sh
sudo apt-get install make g++ gcc-avr binutils-avr avr-libc python3-venv
python3 -m venv .venv
. .venv/bin/activate
python -m pip install -r requirements.txt
make verify
```

`make verify` regenerates the assets, builds both editions and checks the complete
ROM hashes against `release/expected.json`. `make clean` removes generated data
and build products. `make -C default` is also supported. No generated ROMs,
object files, listings, packed output assets or duplicate resource archives are
tracked.

## Source layout

- `game.inc`, `barbarian_*.inc` and the state headers are the canonical game
  logic, AI, contact, animation and scoring sources. The former translator
  outputs are now editable source; the obsolete remake translator is omitted.
- `data/animation-timelines.inc`, `source-round-reset.inc` and
  `startup-defaults.json` define the animation programs and initial state.
- `resources/` contains the authoritative edited PNGs, opponent palette values,
  icon and source SID. The compact reference ADPCM2 bank and its directory are
  retained in `data/`; the resampled effect pack is regenerated from them.
- `platform/` contains the private Uzebox kernel and optimized SPI video/audio
  runtime. `vendor/uzenet/` contains the required SDK sources and headers.
- `tools/` contains only the build generators, codecs, assembly outliner,
  PackRom, budget checker and release verification.

The published network compatibility bytes are pinned in
`data/network-release.json`. The previous source-tree hash included unused
files; retaining it would make cleanup alter the binary. Changes to gameplay
or the network protocol need new compatibility IDs and new expected hashes.
Do not reuse build 22's identity for changed gameplay.

## Controls

D-pad moves; B or Y is the action button. Start opens Back/Quit. Controller 1
Start+Select joins/leaves UzeNet. Title inactivity starts the demonstration after
30 seconds; either controller can exit it.

Simulation runs at nominal 50 Hz with approximately 60 Hz video presentation.
All three music tracks are resident in SPI RAM during play. Audio continues
through VSYNC at approximately 15.734 kHz. Published validation covers emulator
gameplay, audio, sync and paired networking; physical hardware testing remains
outstanding. The detailed frozen evidence accompanies the website release.

GPL-3.0 port sources; retain the included component copyright notices.

# Barbarian source

- Read README.md for the supported build and exact release hashes.
- Keep edited PNGs as authoritative inputs. Never reimport original artwork.
- Run `make verify` before publishing: both complete UZE files must match the
  hashes in release/expected.json for this reproduction of build 22.
- Always distribute a single padded `.uze` per edition, with its concatenated
  asset/data payload embedded using PackRom `dependency=`. No separate runtime
  files on the SD card.
- Preserve interrupt-enabled foreground timing, video-owned UART flow control,
  continuous audio, and the existing SPI cadence. Do not add EEPROM code.
- Network compatibility IDs are pinned to the published binaries. Gameplay or
  protocol changes require new identities and a new release, never build 22's.
- Generated assets and build products are ignored. `make clean` retains all
  source inputs. The original development/audit workspace is separate.

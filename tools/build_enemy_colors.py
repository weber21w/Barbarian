from pathlib import Path
import json
R=Path(__file__).resolve().parents[1]
colors=json.loads((R/'resources/palette/enemies.json').read_text())
assert len(colors)==8 and all(len(c)==2 and all(0<=v<256 for v in c) for c in colors)
words=[skin|(cloth<<8) for skin,cloth in colors]
(R/'data/enemy-palettes.inc').write_text('/* Original spritesB0..7 skin and clothing DAC values. */\nstatic const u16 barbEnemyPalettes[8] PROGMEM={'+','.join(str(w) for w in words)+'};\n')

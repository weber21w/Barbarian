"""Remove reproducible outputs, retaining every tracked source and asset input."""
from pathlib import Path
import shutil

R = Path(__file__).resolve().parents[1]
inputs = {'animation-timelines.inc', 'source-round-reset.inc', 'startup-defaults.json',
          'network-release.json', 'sfx-dir.inc', 'BARB-SND.DAT'}
for p in (R / 'data').iterdir():
    if p.name in inputs:
        continue
    if p.is_dir():
        shutil.rmtree(p)
    else:
        p.unlink()
for p in (R / 'default').iterdir():
    if p.name == 'Makefile':
        continue
    if p.is_dir():
        shutil.rmtree(p)
    else:
        p.unlink()
for p in R.rglob('__pycache__'):
    shutil.rmtree(p)

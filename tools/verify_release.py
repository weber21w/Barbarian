"""Verify complete PackRom output against the published build 22 hashes."""
from pathlib import Path
import hashlib
import json
import struct

R = Path(__file__).resolve().parents[1]
expected = json.loads((R / 'release/expected.json').read_text())
for edition, row in expected.items():
    p = R / 'default' / row['filename']
    blob = p.read_bytes()
    assert blob[:6] == b'UZEBOX'
    assert len(blob) == row['bytes']
    assert hashlib.sha256(blob).hexdigest() == row['sha256'], f'{edition}: published ROM differs'
    program = struct.unpack_from('<I', blob, 8)[0]
    assert blob[512 + program:61952] == b'\xff' * (61440 - program)
    assert hashlib.sha256(blob[61952:]).hexdigest() == row['payload_sha256']
    assert blob == (R / 'default/editions' / edition / p.name).read_bytes()
    print(f'{p.name}: identical to published build 22 ({len(blob):,} bytes)')

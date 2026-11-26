#!/usr/bin/env python3
"""Uzebox dconvert-compatible Precedent Scaling ADPCM helpers.

Precedent encoder/decoder excerpt from Joust's
audio/megabomber-reference/adpcm_codec.py; fast16 extensions are not included.

The 2-bit path matches dconvert transform 6 and Tower of Rubble's decoder.
The 4-bit path implements the intended transform-7 four-decision code layout
(bits 3..0) and matching decoder.
"""
from __future__ import annotations

from typing import Iterable


def _adjust(accum: int, delta: int, up: bool) -> int:
    return (accum + delta if up else accum - delta) & 0xFF


def encode_precedent(samples: bytes | bytearray | Iterable[int], bits: int) -> bytes:
    data = bytes(samples)
    if bits not in (2, 4):
        raise ValueError("bits must be 2 or 4")
    if not data:
        return b""

    symbols_per_byte = 8 // bits
    pad = (-len(data)) % symbols_per_byte
    if pad:
        data += data[-1:] * pad

    accum = 0x80
    slope = 2 if bits == 2 else 7
    up_count = down_count = neutral_count = 0
    out = bytearray()
    mask_weights = (1, 2) if bits == 2 else (1, 2, 4, 8)

    for base in range(0, len(data), symbols_per_byte):
        packed = 0
        for symbol_index in range(symbols_per_byte):
            target = data[base + symbol_index]
            code = 0
            for decision, divisor in enumerate(mask_weights):
                delta = slope // divisor
                up = accum < target
                if up:
                    code |= 1 << (bits - 1 - decision)
                    up_count += 1
                    down_count = 0
                else:
                    down_count += 1
                    up_count = 0
                accum = _adjust(accum, delta, up)

            neutral_count += 1
            if bits == 2:
                if neutral_count == 4:
                    up_count = down_count = neutral_count = 0
                    if slope > 2:
                        slope -= 1
                if up_count == 2 or down_count == 2:
                    up_count = down_count = neutral_count = 0
                    if slope < 16:
                        slope += 1
            else:
                if neutral_count > 2:
                    up_count = down_count = neutral_count = 0
                    if slope > 4:
                        slope -= 1
                if up_count > 3 or down_count > 3:
                    up_count = down_count = neutral_count = 0
                    if slope < 24:
                        slope += 2

            packed |= code << (symbol_index * bits)
        out.append(packed)
    return bytes(out)


def decode_precedent(packed: bytes | bytearray, bits: int, sample_count: int) -> bytes:
    if bits not in (2, 4):
        raise ValueError("bits must be 2 or 4")
    if sample_count < 0:
        raise ValueError("sample_count must be non-negative")

    accum = 0x80
    slope = 2 if bits == 2 else 7
    up_count = down_count = neutral_count = 0
    mask_weights = (1, 2) if bits == 2 else (1, 2, 4, 8)
    symbol_mask = (1 << bits) - 1
    result = bytearray()

    for value in packed:
        byte = value
        for _ in range(8 // bits):
            if len(result) >= sample_count:
                return bytes(result)
            code = byte & symbol_mask
            byte >>= bits
            for decision, divisor in enumerate(mask_weights):
                delta = slope // divisor
                up = bool(code & (1 << (bits - 1 - decision)))
                if up:
                    up_count += 1
                    down_count = 0
                else:
                    down_count += 1
                    up_count = 0
                accum = _adjust(accum, delta, up)

            neutral_count += 1
            if bits == 2:
                if neutral_count == 4:
                    up_count = down_count = neutral_count = 0
                    if slope > 2:
                        slope -= 1
                if up_count == 2 or down_count == 2:
                    up_count = down_count = neutral_count = 0
                    if slope < 16:
                        slope += 1
            else:
                if neutral_count > 2:
                    up_count = down_count = neutral_count = 0
                    if slope > 4:
                        slope -= 1
                if up_count > 3 or down_count > 3:
                    up_count = down_count = neutral_count = 0
                    if slope < 24:
                        slope += 2
            result.append(accum)
    return bytes(result)

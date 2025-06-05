#!/usr/bin/env python3

from dataclasses import dataclass
from typing import Tuple, List

@dataclass
class ColorMap:
    cpc_hw: int           # Amstrad CPC hardware code
    cpc_name: str         # Amstrad CPC color name
    cpc_rgb: Tuple[int,int,int]
    ent_code: int         # Enterprise-64 pen code
    ent_rgb: Tuple[int,int,int]

color_mappings: List[ColorMap] = [
    ColorMap(0x54, "Black",           (  0,   0,   0), 0x00, (  0,   0,   0)),
    ColorMap(0x44, "Blue",            (  0,   0, 128), 0x00, (  0,   0,   0)),
    ColorMap(0x55, "Bright Blue",     (  0,   0, 255), 0x00, (  0,   0,   0)),
    ColorMap(0x5C, "Red",             (128,   0,   0), 0x00, (  0,   0,   0)),
    ColorMap(0x58, "Magenta",         (128,   0, 128), 0x00, (  0,   0,   0)),
    ColorMap(0x5D, "Mauve",           (128,   0, 255), 0x00, (  0,   0,   0)),
    ColorMap(0x4C, "Bright Red",      (255,   0,   0), 0x00, (  0,   0,   0)),
    ColorMap(0x45, "Purple",          (255,   0, 128), 0x00, (  0,   0,   0)),
    ColorMap(0x4D, "Bright Magenta",  (255,   0, 255), 0x00, (  0,   0,   0)),
    ColorMap(0x56, "Green",           (  0, 128,   0), 0x00, (  0,   0,   0)),
    ColorMap(0x46, "Cyan",            (  0, 128, 128), 0x00, (  0,   0,   0)),
    ColorMap(0x57, "Sky Blue",        (  0, 128, 255), 0x00, (  0,   0,   0)),
    ColorMap(0x5E, "Yellow",          (128, 128,   0), 0x00, (  0,   0,   0)),
    ColorMap(0x40, "White",           (128, 128, 128), 0x00, (  0,   0,   0)),
    ColorMap(0x5F, "Pastel Blue",     (128, 128, 255), 0x00, (  0,   0,   0)),
    ColorMap(0x4E, "Orange",          (255, 128,   0), 0x00, (  0,   0,   0)),
    ColorMap(0x47, "Pink",            (255, 128, 128), 0x00, (  0,   0,   0)),
    ColorMap(0x4F, "Pastel Magenta",  (255, 128, 255), 0x00, (  0,   0,   0)),
    ColorMap(0x52, "Bright Green",    (  0, 255,   0), 0x00, (  0,   0,   0)),
    ColorMap(0x42, "Sea Green",       (  0, 255, 128), 0x00, (  0,   0,   0)),
    ColorMap(0x53, "Bright Cyan",     (  0, 255, 255), 0x00, (  0,   0,   0)),
    ColorMap(0x5A, "Lime",            (128, 255,   0), 0x00, (  0,   0,   0)),
    ColorMap(0x59, "Pastel Green",    (128, 255, 128), 0x00, (  0,   0,   0)),
    ColorMap(0x5B, "Pastel Cyan",     (128, 255, 255), 0x00, (  0,   0,   0)),
    ColorMap(0x4A, "Bright Yellow",   (255, 255,   0), 0x00, (  0,   0,   0)),
    ColorMap(0x43, "Pastel Yellow",   (255, 255, 128), 0x00, (  0,   0,   0)),
    ColorMap(0x4B, "Bright White",    (255, 255, 255), 0x00, (  0,   0,   0)),
]

# --- auto-update enterprise codes by JS formula nearest neighbor ---

def _ent_rgb(i: int):
    # match JS: r=((i<<2)&4)|((i>>2)&2)|((i>>6)&1) then *255/7
    r_bits = ((i << 2) & 4) | ((i >> 2) & 2) | ((i >> 6) & 1)
    g_bits = ((i << 1) & 4) | ((i >> 3) & 2) | ((i >> 7) & 1)
    b_bits = ((i >> 1) & 2) | ((i >> 5) & 1)
    r = round(r_bits * 255 / 7)
    g = round(g_bits * 255 / 7)
    b = round(b_bits * 255 / 3)
    return (r, g, b)

# precompute 256-entry palette
_ent_palette = {i: _ent_rgb(i) for i in range(256)}

# update each mapping
for cm in color_mappings:
    # find nearest enterprise index
    best_i = min(_ent_palette, key=lambda i: sum((a-b)**2 for a,b in zip(cm.cpc_rgb, _ent_palette[i])))
    cm.ent_code = best_i
    cm.ent_rgb = _ent_palette[best_i]

if __name__ == '__main__':
    import argparse
    import sys

    parser = argparse.ArgumentParser(
        description='Map CPC colors to Enterprise colors'
    )
    group = parser.add_mutually_exclusive_group()
    group.add_argument(
        '--hardware', action='store_true',
        help='Interpret codes as CPC hardware codes (default)'
    )
    group.add_argument(
        '--firmware', action='store_true',
        help='Interpret codes as CPC firmware codes (by list index)'
    )
    parser.add_argument(
        'codes',
        help='Comma-separated list of CPC color codes; prefix with 0x for hex, otherwise decimal'
    )
    args = parser.parse_args()

    use_firmware = args.firmware
    # build lookup dictionaries mapping to (firmware_index, ColorMap)
    hw_map = {cm.cpc_hw: (idx, cm) for idx, cm in enumerate(color_mappings)}
    fw_map = {idx: (idx, cm) for idx, cm in enumerate(color_mappings)}

    # parse input codes
    inputs = [c.strip() for c in args.codes.split(',') if c.strip()]
    mapped = []
    for code_str in inputs:
        # parse hex if prefixed with 0x, else decimal
        try:
            if code_str.lower().startswith('0x'):
                val = int(code_str, 16)
            else:
                val = int(code_str, 10)
        except ValueError:
            print(f"Invalid code: {code_str}", file=sys.stderr)
            continue
        info = fw_map.get(val) if use_firmware else hw_map.get(val)
        if not info:
            mode = 'firmware' if use_firmware else 'hardware'
            print(f"No mapping for {mode} code {code_str}", file=sys.stderr)
            continue
        fw_index, cm = info
        hw_code = cm.cpc_hw
        ent_hex = f"0x{cm.ent_code:02X}"
        mapped.append(ent_hex)
        # display firmware index and hardware code
        print(
            f"Firmware {fw_index} (0x{fw_index:02X}), "
            f"Hardware 0x{hw_code:02X} -> Enterprise {ent_hex} RGB{cm.ent_rgb}"
        )

    if mapped:
        print('Mapped Enterprise codes:', ','.join(mapped))

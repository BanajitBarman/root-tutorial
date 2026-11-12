#!/usr/bin/env python3
"""Make contact sheets (grids of page thumbnails) from PNG pages, for proof-reading."""
import sys, glob, os
from PIL import Image

src, out_prefix, cols, rows = sys.argv[1], sys.argv[2], int(sys.argv[3]), int(sys.argv[4])
pages = sorted(glob.glob(os.path.join(src, "*.png")))
per = cols * rows
for k in range(0, len(pages), per):
    imgs = [Image.open(p) for p in pages[k:k + per]]
    w, h = imgs[0].size
    sheet = Image.new("RGB", (cols * w, rows * h), "white")
    for i, im in enumerate(imgs):
        sheet.paste(im, ((i % cols) * w, (i // cols) * h))
    name = f"{out_prefix}_{k // per + 1:02d}.png"
    sheet.save(name)
    print(name, f"pages {k+1}-{k+len(imgs)}")

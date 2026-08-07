from PIL import Image
import sys

def profile(path, step=4):
    im = Image.open(path).convert('RGB')
    W, H = im.size
    px = im.load()
    rows = []
    for y in range(H):
        cnt = 0
        for x in range(0, W, step):
            r, g, b = px[x, y]
            if r < 200 or g < 200 or b < 200:
                cnt += 1
        rows.append(cnt)
    return H, rows, W

def content_bottom(rows, thr=5, W=1280):
    last = None
    for y in range(len(rows) - 1, -1, -1):
        if rows[y] > 5:
            last = y
            break
    return last

base = r'C:\Users\limit\OneDrive\Belgeler\Default Project\web_panel_normal.png'
fresh = r'C:\Users\limit\AppData\Local\Temp\opencode\fresh_900.png'
full = r'C:\Users\limit\AppData\Local\Temp\opencode\fresh_full.png'

for name, path in [('web_panel_normal', base), ('fresh_900', fresh), ('fresh_full', full)]:
    H, rows, W = profile(path)
    cb = content_bottom(rows, W=W)
    print(f"== {name}  {W}x{H}  content_bottom={cb}")
    # coarse bars every 30 rows
    mx = max(rows) or 1
    bars = []
    for y in range(0, H, 30):
        bar = '#' * max(1, round(rows[y] / (mx / 50)))
        bars.append(f"{y:4d}:{rows[y]:4d} {bar}")
    print('\n'.join(bars))
    print()

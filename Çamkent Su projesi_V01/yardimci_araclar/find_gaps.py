from PIL import Image

im = Image.open(r'C:\Users\limit\AppData\Local\Temp\opencode\pr2_nobudget.png').convert('RGB')
W, H = im.size
px = im.load()
# background sample from top area (dark navy)
vals = []
for y in range(4, 14):
    for x in range(4, 80):
        vals.append(px[x, y])
bg = tuple(sum(c[i] for c in vals) // len(vals) for i in range(3))
print('bg', bg, 'size', W, H)

step = 2
rows = []
for y in range(H):
    cnt = 0
    for x in range(0, W, step):
        r, g, b = px[x, y]
        if (r - bg[0]) ** 2 + (g - bg[1]) ** 2 + (b - bg[2]) ** 2 > 900:
            cnt += 1
    rows.append(cnt)
mx = max(rows)
print('max', mx)

# gap bands: runs where content is ~0
in_gap = False; start = 0
for y in range(H):
    low = rows[y] < 8
    if low and not in_gap:
        in_gap = True; start = y
    if not low and in_gap:
        in_gap = False
        if y - start >= 4:
            print('GAP %d-%d' % (start, y - 1))
if in_gap and H - start >= 4:
    print('GAP %d-%d' % (start, H - 1))

# profile
for y in range(0, H, 12):
    bar = '#' * max(1, round(rows[y] / (mx / 60)))
    print('%4d:%4d %s' % (y, rows[y], bar))

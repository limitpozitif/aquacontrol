from PIL import Image

a = Image.open(r'C:\Users\limit\OneDrive\Belgeler\Default Project\web_panel_normal.png').convert('RGB')
b = Image.open(r'C:\Users\limit\AppData\Local\Temp\opencode\pr2_nobudget.png').convert('RGB')
W, H = 1280, 900
pa, pb = a.load(), b.load()

# count differing pixels per 40-row band (ignore clock area which changes)
diffs = []
for y in range(0, H, 40):
    d = 0
    for yy in range(y, min(y + 40, H)):
        for x in range(0, W, 4):
            if pa[x, yy] != pb[x, yy]:
                d += 1
    diffs.append((y, d))
print('band(y, diffpx):')
for y, d in diffs:
    print('  %d: %d' % (y, d))

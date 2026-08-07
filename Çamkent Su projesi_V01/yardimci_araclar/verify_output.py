import struct
import zlib

BASE = open(r"C:\Users\limit\OneDrive\Belgeler\Default Project\upside_hmi.HMI", "rb").read()
OUT = open(r"C:\Users\limit\OneDrive\Belgeler\Default Project\upside_hmi_analog.HMI", "rb").read()

# 1) everything outside directory regions must be byte-identical
count_o = struct.unpack_from("<I", OUT, 0)[0]
dir_end = 4 + count_o * 28 + 4
diff = []
for off in range(0, len(BASE)):
    # skip primary dir, backup dir, and the old (unreferenced) 0.pa blob
    if off < dir_end:
        continue
    if 0x80000 <= off < 0x80000 + dir_end:
        continue
    if 0x799780 <= off < 0x799780 + 0xBF76:
        continue
    if off >= len(OUT):
        diff.append((off, "truncated"))
        break
    if BASE[off] != OUT[off]:
        diff.append((off, hex(BASE[off]), hex(OUT[off])))
        if len(diff) > 20:
            break
print("base len", len(BASE), "out len", len(OUT))
print("changed-region diffs (outside dir + backup + old 0.pa):", len(diff))
for d in diff[:20]:
    print("  ", d)

# 2) 4.is PNG data == source PNG
png_src = open(r"C:\Users\limit\OneDrive\Belgeler\Default Project\nextion-upside-p0.png", "rb").read()
start4is = 0x94948A
size4is = 0x6835
blob4is = OUT[start4is:start4is + size4is]
print("4.is header:", blob4is[:27].hex(" "))
print("4.is png == source:", blob4is[27:] == png_src, "len", len(png_src))

# 3) decode 4.is png and compare to 4.i raw (round-trip, same as 0.is/0.i test)
start4i = 0x88DC72
size4i = 0xBB818
blob4i = OUT[start4i:start4i + size4i]
print("4.i header:", blob4i[:24].hex(" "))

def png_pixels(data):
    pos = 8
    idat = b""
    while pos < len(data):
        ln, = struct.unpack_from(">I", data, pos)
        typ = data[pos + 4:pos + 8]
        pos += 8
        chunk = data[pos:pos + ln]
        pos += ln + 4
        if typ == b"IHDR":
            w, h, bitd, ct, _, _, il = struct.unpack(">IIBBBBB", chunk)
        elif typ == b"IDAT":
            idat += chunk
    raw_data = zlib.decompress(idat)
    ch = {0: 1, 2: 3, 3: 1, 4: 2, 6: 4}[ct]
    stride = w * ch
    rows = []
    p = 0
    prev = bytearray(stride)
    for _ in range(h):
        f = raw_data[p]
        p += 1
        line = bytearray(raw_data[p:p + stride])
        p += stride
        if f == 1:
            for i in range(ch, stride):
                line[i] = (line[i] + line[i - ch]) & 0xFF
        elif f == 2:
            for i in range(stride):
                line[i] = (line[i] + prev[i]) & 0xFF
        elif f == 3:
            for i in range(stride):
                a = line[i - ch] if i >= ch else 0
                line[i] = (line[i] + ((a + prev[i]) >> 1)) & 0xFF
        elif f == 4:
            for i in range(stride):
                a = line[i - ch] if i >= ch else 0
                b = prev[i]
                c = prev[i - ch] if i >= ch else 0
                pa = abs(b - c); pb = abs(a - c); pc = abs(a + b - 2 * c)
                pr = a if (pa <= pb and pa <= pc) else (b if pb <= pc else c)
                line[i] = (line[i] + pr) & 0xFF
        prev = line
        rows.append(bytes(line))
    return w, h, ct, rows

w, h, ct, rows = png_pixels(blob4is[27:])
print("4.is decoded:", w, "x", h, "colortype", ct)
mismatch = 0
for y in range(h):
    line = rows[y]
    for x in range(w):
        if ct == 6:
            r, g, b, a = line[x * 4:x * 4 + 4]
        else:
            r, g, b = line[x * 3:x * 3 + 3]
        v = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)
        lo, hi = v & 0xFF, v >> 8
        off = 24 + (y * w + x) * 2
        if blob4i[off] != lo or blob4i[off + 1] != hi:
            mismatch += 1
print("4.i vs decoded 4.is mismatches:", mismatch, "/", w * h)

# 4) new 0.pa at EOF is the live one, tail bytes preserved
i37, st, sz, d, t0, t1, t2 = None, None, None, None, None, None, None
count = struct.unpack_from("<I", OUT, 0)[0]
for i in range(count):
    off = 4 + i * 28
    nm = OUT[off:off + 16].split(b"\x00")[0].decode("latin-1")
    if nm == "0.pa":
        st, sz, d, t0, t1, t2 = struct.unpack_from("<IIBBBB", OUT, off + 16)
        i37 = i
print("0.pa dir#", i37, "start=0x%x" % st, "size=0x%x" % sz, "deleted", d,
      "tail", (t0, t1, t2), "expected tail (195,47,1)")
print("new 0.pa at EOF == live blob:", st == len(BASE))

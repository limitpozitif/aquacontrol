import re, sys, io
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8', errors='replace')
dom = open(sys.argv[1], encoding='utf-8', errors='replace').read()
for m in re.finditer(r'id="(ov\d+)"[^>]*>([^<]+)<', dom):
    print(m.group(1), '->', m.group(2))

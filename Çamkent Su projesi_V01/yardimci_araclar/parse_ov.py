import re
dom = open(r'C:\Users\limit\AppData\Local\Temp\opencode\klavuz_dom.txt', encoding='utf-8', errors='replace').read()
for m in re.finditer(r'id="(ov\d+)"[^>]*>([^<]+)<', dom):
    print(m.group(1), '->', m.group(2))

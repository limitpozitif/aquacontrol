@echo off
chcp 65001 >nul
net session >nul 2>&1
if %errorlevel% neq 0 (
  echo Yonetici izni isteniyor - lutfen UAC'de "Evet" e tikla...
  powershell -Command "Start-Process -FilePath '%~f0' -Verb RunAs"
  exit /b
)

echo [1/3] Python icin engelleme kurallari temizleniyor...
powershell -NoProfile -ExecutionPolicy Bypass -Command "& { Get-NetFirewallRule -ErrorAction SilentlyContinue | Where-Object {$_.Direction -eq 'Inbound' -and $_.Action -eq 'Block'} | ForEach-Object { $r = $_; $app = ($r | Get-NetFirewallApplicationFilter -ErrorAction SilentlyContinue); if($app -and $app.Program -match 'python'){ Remove-NetFirewallRule -Name $r.Name -ErrorAction SilentlyContinue } } }"

echo [2/3] Port 8000 izin kurali ekleniyor (guvence icin)...
powershell -NoProfile -ExecutionPolicy Bypass -Command "& { netsh advfirewall firewall delete rule name='Camenk Aqua 8000' >$null 2>&1; netsh advfirewall firewall add rule name='Camenk Aqua 8000' dir=in action=allow protocol=TCP localport=8000 profile=any | Out-Null }"

echo [3/3] Sunucu baslatiliyor...
cd /d "%~dp0"
echo.
echo Telefon icin:   http://192.168.1.195:8000
echo Laptop icin:    http://localhost:8000
echo.
echo Durdurmak icin bu pencereyi kapat.
echo.
"C:\Users\limit\AppData\Local\Programs\Python\Python312\python.exe" -u -m http.server 8000 --bind 0.0.0.0
pause

New-NetFirewallRule -DisplayName "Camenk Aqua Panel 8000" -Direction Inbound -Action Allow -Protocol TCP -LocalPort 8000 -Profile Public,Private
netsh advfirewall firewall add rule name="Camenk Aqua 8000 (fallback)" dir=in action=allow protocol=TCP localport=8000 profile=any

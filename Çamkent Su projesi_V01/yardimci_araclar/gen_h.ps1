$root = "C:\Users\limit\OneDrive\Belgeler\Default Project"
$dataDir = Join-Path $root "web_panel_data"
$gomulu  = Join-Path $root "web_gomulu"

$wrappers = [ordered]@{
  "index.html" = "WEB_INDEX_HTML"
  "login.html" = "WEB_LOGIN_HTML"
  "style.css"  = "WEB_STYLE_CSS"
  "app.js"     = "WEB_APP_JS"
}

$utf8 = New-Object System.Text.UTF8Encoding($true)

foreach ($file in $wrappers.Keys) {
  $name = $wrappers[$file]
  $content = [System.IO.File]::ReadAllLines((Join-Path $dataDir $file), [System.Text.Encoding]::UTF8)
  $out = New-Object System.Collections.Generic.List[string]
  $out.Add("// $file (web_panel_data) -> gomulu (PROGMEM) web dosyasi")
  $out.Add("#pragma once")
  $out.Add("")
  $out.Add("static const char $name[] PROGMEM = R`"CAMKENTP9(`"")
  foreach ($line in $content) { $out.Add($line) }
  $out.Add(')CAMKENTP9";')
  $dest = Join-Path $gomulu ("web_" + $file.Replace(".", "_") + ".h")
  [System.IO.File]::WriteAllLines($dest, $out.ToArray(), $utf8)
  Write-Output "$file -> $dest ($($out.Count) lines)"
}

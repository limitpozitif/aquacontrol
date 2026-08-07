$root = "C:\Users\limit\OneDrive\Belgeler\Default Project"
$src  = Join-Path $root "downside_web_entegre.txt"
$lines = [System.IO.File]::ReadAllLines($src, [System.Text.Encoding]::UTF8)

$closeRe = '^\)CAMKENTP9";$'

$blocks = [ordered]@{
  "WEB_INDEX_HTML" = "index.html"
  "WEB_LOGIN_HTML" = "login.html"
  "WEB_STYLE_CSS"  = "style.css"
  "WEB_APP_JS"     = "app.js"
}

$utf8 = New-Object System.Text.UTF8Encoding($true)

foreach ($name in $blocks.Keys) {
  $startLine = -1
  for ($i = 0; $i -lt $lines.Count; $i++) {
    if ($lines[$i] -match "static const char $name\b") { $startLine = $i + 1; break }
  }
  if ($startLine -lt 0) { Write-Output "MISSING START: $name"; continue }

  $endLine = -1
  for ($i = $startLine; $i -lt $lines.Count; $i++) {
    if ($lines[$i] -match $closeRe) { $endLine = $i; break }
  }
  if ($endLine -lt 0) { Write-Output "MISSING END: $name"; continue }

  $content = $lines[($startLine)..($endLine - 1)]
  $dest = Join-Path $root (Join-Path "web_panel_data" $blocks[$name])
  [System.IO.File]::WriteAllLines($dest, $content, $utf8)
  Write-Output "$name ($($content.Count) lines) -> web_panel_data/$($blocks[$name])"
}

$ErrorActionPreference = "Stop"
$srcDir = "C:\Users\limit\OneDrive\Belgeler\Default Project\web_panel_data"
$outDir = "C:\Users\limit\OneDrive\Belgeler\Default Project\web_gomulu"

if (-not (Test-Path -LiteralPath $outDir)) {
  New-Item -ItemType Directory -Path $outDir | Out-Null
}

$files = @(
  @{ src = "index.html";  out = "web_index_html.h";  var = "WEB_INDEX_HTML";  tag = "index.html (web_panel_data)" },
  @{ src = "login.html";  out = "web_login_html.h";  var = "WEB_LOGIN_HTML";  tag = "login.html (web_panel_data)" },
  @{ src = "style.css";   out = "web_style_css.h";   var = "WEB_STYLE_CSS";   tag = "style.css (web_panel_data)" },
  @{ src = "app.js";      out = "web_app_js.h";      var = "WEB_APP_JS";      tag = "app.js (web_panel_data)" }
)

foreach ($f in $files) {
  $src = Join-Path $srcDir $f.src
  $raw = [System.IO.File]::ReadAllText($src)
  $raw = $raw.TrimStart([char]0xFEFF)
  $raw = $raw.TrimStart("`r", "`n")

  $sb = New-Object System.Text.StringBuilder
  [void]$sb.AppendLine("// $($f.tag) -> gomulu (PROGMEM) web dosyasi")
  [void]$sb.AppendLine("#pragma once")
  [void]$sb.AppendLine("")
  [void]$sb.AppendLine('static const char ' + $f.var + '[] PROGMEM = R"CAMKENTP9(')
  [void]$sb.Append($raw)
  [void]$sb.AppendLine('')
  [void]$sb.AppendLine(')CAMKENTP9";')
  [void]$sb.AppendLine('')

  $out = Join-Path $outDir $f.out
  [System.IO.File]::WriteAllText($out, $sb.ToString(), [System.Text.UTF8Encoding]::new($false))
  Write-Output "yazildi: $out"
}

$total = 0
Get-ChildItem -LiteralPath $outDir -Filter *.h | ForEach-Object { $total += $_.Length }
Write-Output "toplam bytes (header): $total"

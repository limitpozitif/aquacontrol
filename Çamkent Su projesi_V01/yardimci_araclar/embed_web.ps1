# embed_web.ps1
# web_panel_data/* -> downside_web_entegre.txt (FLASH kaynagi) bloklarini gomuler.
# extract_web.ps1'in tersidir. Firmware govdesi disindaki hicbir seye dokunmaz (blok disi baytlar korunur).
# Dogrulama: extract_web.ps1 ile bloklari cikarip web_panel_data ile karsilastir (birebir olmali).

$ErrorActionPreference = "Stop"
$root = "C:\Users\limit\OneDrive\Belgeler\Default Project"
$src  = Join-Path $root "downside_web_entegre.txt"
$dataDir = Join-Path $root "web_panel_data"

$utf8Bom   = New-Object System.Text.UTF8Encoding($true)
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)

$rawBytes = [System.IO.File]::ReadAllBytes($src)
$hadBom = ($rawBytes.Length -ge 3 -and $rawBytes[0] -eq 0xEF -and $rawBytes[1] -eq 0xBB -and $rawBytes[2] -eq 0xBF)
$text = [System.Text.Encoding]::UTF8.GetString($rawBytes)
if ($hadBom) { $text = $text.Substring(1) }

# dominan satir sonu
$crlf = ([regex]::Matches($text, "`r`n")).Count
$lf   = ([regex]::Matches($text, "(?<!`r)`n")).Count
$nl = if ($crlf -ge $lf) { "`r`n" } else { "`n" }

function Replace-Block {
  param([string]$text, [string]$name, [string[]]$lines, [string]$nl)
  $openMark  = 'static const char ' + $name + '[] PROGMEM = R"CAMKENTP9('
  $closeMark = ')CAMKENTP9";'
  $openIdx = $text.IndexOf($openMark)
  if ($openIdx -lt 0) { throw "BLOK BULUNAMADI: $name" }
  $nlIdx = $text.IndexOf("`n", $openIdx)
  if ($nlIdx -lt 0) { throw "açılış satırı sonu bulunamadı: $name" }
  $contentStart = $nlIdx + 1
  $closeIdx = $text.IndexOf($closeMark, $contentStart)
  if ($closeIdx -lt 0) { throw "kapanış işareti bulunamadı: $name" }
  $newBlock = ($lines -join $nl) + $nl + $closeMark
  return $text.Substring(0, $contentStart) + $newBlock + $text.Substring($closeIdx + $closeMark.Length)
}

$blocks = [ordered]@{
  "WEB_INDEX_HTML" = "index.html"
  "WEB_LOGIN_HTML" = "login.html"
  "WEB_STYLE_CSS"  = "style.css"
  "WEB_APP_JS"     = "app.js"
}

foreach ($name in $blocks.Keys) {
  $file = Join-Path $dataDir $blocks[$name]
  # ReadAllLines UTF8, BOM'u kendisi siliyor; buraya BOM kontrolü EKLEME
  # (PS 5.1'de string.StartsWith([char]0xFEFF) hatalı TRUE döner ve içeriği bozar).
  $lines = [System.IO.File]::ReadAllLines($file, [System.Text.Encoding]::UTF8)
  $text = Replace-Block -text $text -name $name -lines $lines -nl $nl
  Write-Output "gömüldü: $name  ($($blocks[$name]), $($lines.Count) satır)"
}

$enc = if ($hadBom) { $utf8Bom } else { $utf8NoBom }
[System.IO.File]::WriteAllText($src, $text, $enc)
Write-Output "yazildi: $src"

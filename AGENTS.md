# AGENTS.md — Çamkent Su Projesi

Bu dosya opencode'un bu projede nasıl çalışacağını belirler. Her oturumda uygulanır.

## 📂 ÇALIŞMA MODELİ (aktif + onaylanmış sürüm)

- **Aktif çalışma alanı = `firmware_versiyon\` kökü.** Üzerinde oynadığımız/hazırladığımız
  dosyaların **son durumu** burada durur (`downside_web_entegre.txt`, `upside_web_entegre.txt`,
  `web_panel_data\`, `web_gomulu\`, sunum/kılavuzlar, görseller, `istatistik\`, `PROJE_NOTLARI.md`).
- **Onaylanmış sürüm = `firmware_versiyon\Çamkent Su projesi_V01\` (gerekiyorsa `_Vxx`).**
  Aktif alandaki değişiklikler kullanıcı tarafından **onaylandığında** ilgili dosyalar buraya
  kopyalanır (MD5 eşitliği doğrulanır). Böylece onaylanan sürüm garanti altına alınır.
- **Yeni versiyon planlanırsa** `Çamkent Su projesi_Vxx` numarası +1 arttırılır ve o versiyonla
  ilgili **tüm dosyalar** yeni `Vxx` klasörüne kopyalanır; çalışma yeni `Vxx` üzerinden onay döngüsüyle sürer.
- `Vxx` klasörü **sadece ilgili versiyon dosyalarını** içerir: analiz yapıları
  (server.py, collector.py, db.py, dashboard.*, readings.db*, templates\, raporlar\ vb.)
  `firmware_versiyon\istatistik\` altında durur, `Vxx` içinde **olmaz**.
- Her onaylanmış değişikliği ilgili klasörün `PROJE_NOTLARI.md` → "DEĞİŞİKLİK GÜNLÜĞÜ" bölümüne yaz.
- Git varsa değişiklikleri commit'le (mesaj Türkçe, açıklayıcı).

## 🎤 SUNUM / ADMIN / HANE BİLGİLENDİRME

Değişiklik **adminleri ya da hane sahiplerini ilgilendiriyorsa**
(kullanım, giriş, alarm, yeni özellik, komut değişikliği vb.) ayrıca sor:

> "Bu durumu sunumlarda belirtelim mi?"

İlgili dosyalar: `yangin-guvenligi-sunum.html/.pdf`, `Web_Panel_Giris_Sistemi_Kilavuzu.html/.pdf`,
`site-yangin-kontrol-sistemi-sunum-raporu.*`.

Sunum ve kılavuz kapaklarında **sürüm etiketi (Vxx)** gösterilir. Sunumun **slayt 2**'si
`last_update` ("📋 Son Güncelleme") slaytıdır: o sürümde yapılan değişiklikleri listeler ve
her versiyon değiştiğinde **güncellenir**; duyuru olarak ayrıca yayınlanabilir. Kullanıcı
bir değişiklik yaptırdığında `last_update` slaytına da eklemeyi unutma.

## 🏷 SÜRÜM ETİKETİ SENKRONİZASYONU (web = aktif versiyon)

⚠️ **KURAL:** Yeni versiyon yapıldığında versiyon numarası (Vxx) **TÜM gereken dosyalara**
aynı anda işlenir. Hiçbir dosyada eski numara bırakılmaz. Eksik işleme = flaşlı cihazda
eski sürüm görünür.

Web panelde görünen sürüm etiketi **her zaman aktif proje versiyonunu (Vxx)** gösterir.
Aktif versiyon değiştiğinde (yeni `Çamkent Su projesi_Vxx`) şu hepsi **birlikte** güncellenir:

1. `downside_web_entegre.txt` → firmware `/data` tarafı: `j["ver"]="Vxx"` **ve** gömülü
   index.html bloğu: hero-sub (`·&nbsp; Vxx`) + footer (`ÇAMKENT AQUA HMI Vxx`) — flash kaynağı
2. `web_panel_data/index.html` → hero-sub + footer + `v=Vxx` (3 satır) — kanonik kaynak
3. `web_gomulu/web_index_html.h` → aynı iki satır + `v=Vxx` (gömülü firmware başlığı)
4. Panel görselleri: `web_panel_normal.png`, `web_panel_fire.png`, `panel_upper.png`, `panel_lower.png`
   → webmock ile Chrome `--screenshot` (budget'sız) + `--window-size=1280,1700`; kırpma: üst 0–848, alt 858–1480
5. Sunum/kılavuz PDF'leri → `--print-to-pdf` ile yeniden üret (önce Temp, sonra Remove-Item + Move-Item)

Değişiklikleri aktif alan (`firmware_versiyon\`) + onaylanmış versiyon klasörünün **ikisine de**
uygula, günlüğe işle, commit'le. `upside_web_entegre.txt` web sunmadığı için sürüm etiketi taşımaz (dokunma).

**⚠️ AQUAKONTROL.COM İSTİSNASI:** `aquakontrol-web\docs\` (canlı tanıtım sitesi, Vercel yayınlıyor)
**hariçtir** — sürüm etiketi taşımaz, senkronizasyon/versiyon kuralları ona uygulanmaz ve GitHub'da
**halka açık** kalır (public repo). Firmware/web panel source'ları ise GitHub'da **asla** olmaz;
yalnızca yerelde (`firmware_versiyon\`) tutulur.

**⚠️ İKİ AYRI PROJE KAVRAMI:** Bu çalışma alanında iki farklı proje var, karıştırma:
1. **Ticari web sayfası (aquakontrol.com)** → `aquakontrol-web\docs\` klasörü, Vercel yayınlıyor,
   GitHub'da **açık**. Sürüm etiketi taşımaz, ESP32 kuralları ona uygulanmaz. Yalnızca bu kısım GitHub'a gider.
2. **ESP32 web paneli** → `firmware_versiyon\web_panel_data\` + `web_gomulu\` + firmware
   (`downside_web_entegre.txt`, `upside_web_entegre.txt`). Bunlar ESP32'lerin yayınladığı web/paneldir;
   sürüm etiketi (Vxx) **burada** geçerlidir ve **GitHub'a asla gitmez** — yalnızca yerelde tutulur.

GitHub repo'su sadece ticari web sayfasını içerir: `aquakontrol-web\docs\`, `AGENTS.md`, `.gitignore`.
Diğer tüm kaynak/teknik dosyalar `.gitignore` ile korunur ve yerelde kalır.

**Yeni versiyon sonrası doğrulama kontrol listesi** (tümü `Vxx` ile eşleşmeli):
- `firmware_versiyon\downside_web_entegre.txt`: `j["ver"]="Vxx"` + gömülü hero-sub/footer
- `web_panel_data/index.html`: hero-sub + footer + `v=Vxx`
- `web_gomulu/web_index_html.h`: aynı satırlar
- `firmware_versiyon\` arşivindeki kopyalar (aktif ile MD5 eşit olmalı)
- Sunum/kılavuz kapakları `Vxx` + `last_update` slaytı güncel

## 🔥 FLASH KAYNAĞI (kritik — kullanıcı bu dosyaları flash ediyor)

Kullanıcı **flash'ı aşağıdaki dosyalardan yapıyor** (`firmware_versiyon\` kökü), `web_gomulu\*.h` DEĞİL:

- **`downside_web_entegre.txt`** — DOWNSIDE firmware + web panel **gömülü** (asıl flash dosyası).
  Web'i `R"CAMKENTP9(...)CAMKENTP9"` blokları olarak içinde taşır: `WEB_INDEX_HTML`
  (index.html), `WEB_LOGIN_HTML` (login.html), `WEB_STYLE_CSS` (style.css), `WEB_APP_JS`
  (app.js). Ayrıca `/data` → `j["ver"]="V01"` burada yapılır.
- **`upside_web_entegre.txt`** — UPSIDE firmware (Nextion + pompa kontrolü). **Web sunmaz**
  ("HTTP /data yolu kaldırıldı — veri ESP'ler üzerinden"), yani web embed gerekmez.
  Sürüm etiketi yalnızca downside'ın sunduğu web panelindedir (V01) — upside kodunda etiket yok.

Kural: **`web_panel_data\` içinde yapılan her değişiklik, `downside_web_entegre.txt` içindeki
ilgili bloğa da gömülmek zorundadır**; aksi hâlde flash edilen cihazda eski web kalır.

- Gömmek: `yardimci_aracilar\embed_web.ps1` (web_panel_data → downside_web_entegre.txt;
  blok dışındaki tüm baytlara dokunmaz; idempotent; dominan satır sonunu korur; script
  `PSScriptRoot` görelli çalışır → aktif alanda `firmware_versiyon` üzerinde çalıştır, sonra onayla).
- Doğrulamak: embed sonrası `git diff -- downside_web_entegre.txt` incele (yalnızca blok içi
  fark olmalı); bloklar birebir `web_panel_data` ile eşit olmalı.
- `web_gomulu\*.h` ayrı başlık biçimidir (`web2header.ps1` üretir) — flash kaynağı DEĞİLDİR.
- ⚠️ PS 5.1 tuzağı: `string.StartsWith([char]0xFEFF)` hatalı TRUE döner ve içeriği bozar;
  scriptlerde BOM kontrolü EKLEME (`ReadAllLines` BOM'u kendisi siler).
- `yardimci_aracilar\` scriptleri onaylanmış sürüm klasöründe yaşar; aktif çalışma sırasında
  `firmware_versiyon\` köküne kopyalanıp orada çalıştırılır ve tekrar V01'e gömülür.

**Web panel değişikliğinde tam akış (sırayla):**
1. `firmware_versiyon\web_panel_data\*` dosyasını düzenle — **kanonik kaynak burasıdır**.
2. `embed_web.ps1` çalıştır (web_panel_data → `firmware_versiyon\downside_web_entegre.txt`).
3. `web2header.ps1` çalıştır (web_panel_data → `firmware_versiyon\web_gomulu\*.h`).
4. Doğrula: `git diff -- downside_web_entegre.txt` (yalnızca blok içi fark); sonra
   `downside_web_entegre.txt`'yi aktif `firmware_versiyon\` kökünden onaylanmış `Vxx`'e kopyala
   ve **MD5 eşitliğini** kontrol et.
5. Commit (Türkçe, açıklayıcı).

**⚠️ Doğrulama tuzağı:** `git diff --no-index --stat` çıktısını `-match "diff --git"` ile test
etmek YANILTICI — farklı içerik "BIREBIR AYNI" görünebilir (stderr CRLF uyarısı + Out-String
etkileşimi). Güvenilir yöntemler:
- **Blok içeriği:** satırları `-join "\n"` ile normalize et, string eşitliğine bak (git kullanma).
- **Tam dosya:** `git diff --no-index <orijinal> <üretilen>` → hunk'ları incele.
- **İdempotentlik:** embed'i 2 kez çalıştır, dosya MD5'i değişmemeli.
- **root vs V01:** `Get-FileHash -Algorithm MD5` eşit olmalı.

**Kanoniklik şüphesinde:** hangi dosya daha güncel diye bak — `git log --oneline -- <dosya>`.
İlk snapshot'ta (60a790f) firmware'in gömülü `style.css`'i ile `web_panel_data/style.css`
`.status-leds` satır sırasında birbirinden farklıydı (işlevsel fark yok); gömme her zaman
`web_panel_data`'yı esas alır. Uptimum: `upside_web_entegre.txt` içinde web yok
(`<!DOCTYPE`, `CAMKENTP9`, `ver`, `sürüm` aranmaz) — upside veriyi UDP ile downside'a yollar.

## 🛠 ORTAM / ÜRETİM NOTLARI

- Shell: Windows PowerShell 5.1. Python: `C:\Users\limit\AppData\Local\Programs\Python\Python312\python.exe`
  (pypdf, pillow yüklü). Chrome: `C:\Program Files\Google\Chrome\Application\chrome.exe`.
  Git: `C:\Program Files\Git\cmd\git.exe` (PATH'e eklemeden `$env:Path = "C:\Program Files\Git\cmd;" + $env:Path`).
- **PDF üretimi:** `chrome --headless --disable-gpu --no-sandbox --print-to-pdf="<hedef>.pdf" --no-pdf-header-footer "file:///<url>.html"`
- **⚠️ Screenshot'ta `--virtual-time-budget` KULLANMA** — CSS boyanmadan boş resim üretir.
  Budget yalnızca `--dump-dom` ve `--print-to-pdf` için sorunsuz.
- **OneDrive kilit sorunu:** PDF'i önce `%TEMP%\opencode` içine üret; hedefteki eskiyi
  `Remove-Item` ile sil, `Move-Item` ile taşı. Doğrudan yazma kilit yaratır.
- **Panel ekran görüntüsü:** `web_panel_data` + mock `data` ile `python -m http.server` servis edip
  Chrome `--screenshot` al. Mock ortam: `%TEMP%\opencode\webmock`.
- **Taşma kontrolü (sunum/kılavuz):** `%TEMP%\opencode\diag_all.js` tüm slaytların
  `scrollHeight`'ını ölçer. Yöntem: HTML kopyasını temp'e al, görselleri yanına kopyala,
  `</body>` öncesine diag ekle, `--dump-dom` ile çalıştır. **A4 yatay slayt sH = 794 olmalı.**

### Panel bölüm sınırları (1280 px render)
- Üst bölüm (hero + özet + üst istasyon): satır **0–848**
- Alt bölüm (alt istasyon + alt bilgi): satır **858–1480**
- Kırpmalar: `panel_upper.png` = 0–848, `panel_lower.png` = 858–1480. Kenarlar boşlukta olmalı.

## 📁 DOSYA DİZİNİ

> Tüm yollar `firmware_versiyon\` köküne göredir (aktif çalışma alanı). Sunum/kılavuz/görseller
> ve analiz yapıları (`istatistik\`) burada; sürüm etiketi (Vxx) taşıyan her şey ayrıca
> `Çamkent Su projesi_V01\` içinde onaylanmış kopyasıyla yaşar.

| Dosya | Açıklama |
|---|---|
| `web_panel_data\` | **GÜNCEL** panel (app.js V01, index.html, login.html, style.css) |
| `web_gomulu\` | Panelin Arduino'ya gömülü halleri (`web_*.h`) |
| `istatistik\` | Analiz sunucusu (server.py, collector.py, db.py, config.py, dashboard.*, readings.db*, templates\, raporlar\) |
| `Web_Panel_Giris_Sistemi_Kilavuzu.*` | Kullanım kılavuzu (10 slayt) |
| `yangin-guvenligi-sunum.*` | Yangın güvenliği sunumu (12 slayt) |
| `site-yangin-kontrol-sistemi-sunum-raporu.*` | Teknik sunum raporu |
| `downside_web_entegre.txt` | **FLASH kaynağı** — DOWNSIDE firmware + web gömülü (`WEB_INDEX_HTML`, `WEB_LOGIN_HTML`, `WEB_STYLE_CSS`, `WEB_APP_JS` blokları, `j["ver"]="V01"`); `web_panel_data\` ile senkron tutulur (embed_web.ps1) |
| `upside_web_entegre.txt` | **FLASH kaynağı** — UPSIDE firmware (Nextion + pompa); web sunmaz, embed gerekmez |
| `downside_web_ek.txt`, `upside_web_ek.txt` | Firmware web ekleri |
| `upside_web_entegre.txt`, `downside_son_durum_*.txt`, `upside_*.txt` | Firmware entegrasyon dokümanları |
| `web_veri_semasi.txt` | `/data` sözleşmesi (`kullaniciSay`, `aktifSay`, `fire1Start`, `pressureOK`...) |
| `upside_hmi.HMI`, `upside_hmi_analog.HMI` | Nextion HMI projeleri (Nextion Editor'de açılır; ben düzenlemem) |
| `nextion-*.html/.png`, `nextion-kurulum-rehberi.md` | Nextion ekran diyagramları / kurulum |
| `upside-layer.*`, `downside-layer.*` | Katman diyagramları |
| `web_panel_normal.png`, `web_panel_fire.png` | Panel tam ekran görselleri (1280×900) |
| `panel_upper.png`, `panel_lower.png` | Üst/alt bölüm kırpmaları (kılavuz) |
| `login_card.png`, `login_screen.png` | Giriş ekranı görselleri |
| `upside.PNG` | Teknik panel görseli |
| `serve.bat` | Panel sunma yardımcısı |
| `PROJE_NOTLARI.md` | Aktif ana günlük (kök; V01'de sürüm kopyası) |

> Default Project kökü **yalnızca proje klasörlerini** barındırır: `firmware_versiyon\`,
> `aquakontrol-web\`, `altyapi-koordinasyon\` + meta dosyalar (`AGENTS.md`, `.gitignore`).
> Kökte başka proje dosyası bulunmaz; `Turkiye_Analizi_2002-2026.pdf` proje dışı kişisel belgedir,
> gitignore'dadır, dokunma.

> Not: `roll-off-*` dosyaları bu projeye ait değil (ayrı iş fikri). Onlara dokunma.
> Versiyon klasörlerinde her sürümün `PROJE_NOTLARI.md` dosyası bulunur — en güncel bilgi oradadır.

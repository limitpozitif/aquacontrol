# Çamkent Su Projesi — V01

Bu klasör projenin versiyonlanmış anlık görüntüsüdür. Root'taki dosyalar yedektir;
bundan sonra çalışmalar versiyon klasörü üzerinden yürütülür.

---

## 🔁 VERSİYONLAMA KURALI (OpenCode — HER DEĞİŞİKLİKTE UYGULA)

Kullanıcı bir değişiklik istediğinde, çalışmaya başlamadan önce **kendine not düş** ve
kullanıcıya ŞU SORUYU sor:

> **"Yeni bir versiyon mu yapalım, mevcut projeyi mi güncelleyelim?"**

- Cevap **"yeni proje / yeni versiyon"** → versiyon numarasını 1 arttır
  (`Çamkent Su projesi_V01` → `_V02` ...), **tüm dosyaların kopyasını** yeni klasöre al,
  çalışmayı orada yap.
- Cevap **"güncelle"** → aynı versiyon klasöründeki dosyalar üzerinde çalış.
- Yapılan her değişikliği aşağıdaki **DEĞİŞİKLİK GÜNLÜĞÜ** bölümüne işle
  (tarih + ne yapıldı + ilgili dosyalar).

Böylece kullanıcı geriye dönüşü kolayca yapabilir.

---

## 🎤 SUNUM / ADMIN / HANE BİLGİLENDİRME KURALI

Yapılan değişiklik **adminleri ya da hane sahiplerini ilgilendiriyorsa**
(kullanım, giriş, alarm, yeni özellik, komut değişikliği vb.) kullanıcıya şunu da sor:

> **"Bu durumu sunumlarda belirtelim mi?"**

İlgili sunum dosyaları:
- `yangin-guvenligi-sunum.html` / `.pdf` (yangın güvenliği sunumu)
- `Web_Panel_Giris_Sistemi_Kilavuzu.html` / `.pdf` (kullanım kılavuzu)
- `site-yangin-kontrol-sistemi-sunum-raporu.*` (teknik sunum raporu)

---

## 🛠 ORTAM / ÜRETİM NOTLARI (OpenCode için)

- **Windows + PowerShell 5.1.** Python: `C:\Users\limit\AppData\Local\Programs\Python\Python312\python.exe`
  (pypdf, pillow yüklü). Chrome: `C:\Program Files\Google\Chrome\Application\chrome.exe`.
- **PDF üretimi:** `chrome --headless --disable-gpu --no-sandbox --print-to-pdf="<hedef>.pdf" --no-pdf-header-footer "file:///<url>.html"`
- **⚠️ Screenshot alırken `--virtual-time-budget` KULLANMA** (CSS boyanmadan boş resim üretir).
- **OneDrive kilit sorunu:** PDF önce `%TEMP%\opencode` içine üret; sonra hedefteki eskiyi
  `Remove-Item` ile sil ve `Move-Item` ile taşı. Doğrudan yazmak kilit yaratır.
- **Panel ekran görüntüsü:** `web_panel_data` + mock `data` dosyasıyla `python -m http.server`
  kullanıp Chrome `--screenshot` al. Mock ortam referansı: `%TEMP%\opencode\webmock`.
- **Taşma kontrolü (sunum/kılavuz):** `%TEMP%\opencode\diag_all.js` tüm slaytların
  `scrollHeight`'ını ölçer. Yöntem: HTML kopyasını temp'e al, görselleri yanına kopyala,
  `</body>` öncesine diag ekle, `--dump-dom` ile çalıştır. **A4 yatay slayt için sH = 794 olmalı.**

### Panel bölüm sınırları (1280 px render, güncel web_panel_data)
- Üst bölüm (hero + özet + üst istasyon): satır **0–848**
- Alt bölüm (alt istasyon + alt bilgi): satır **858–1480**
- Kırpmalar: `panel_upper.png` = 0–848, `panel_lower.png` = 858–1480.
  Kenarlar boşlukta olmalı — içerik kesilmemeli.

---

## 📁 DOSYA DİZİNİ

| Dosya | Açıklama |
|---|---|
| `web_panel_data\` | **GÜNCEL** panel (app.js V01, index.html, login.html, style.css) |
| `web_gomulu\` | Panelin Arduino'ya gömülü halleri (`web_*.h`) |
| `Web_Panel_Giris_Sistemi_Kilavuzu.*` | Kullanım kılavuzu (10 slayt) |
| `yangin-guvenligi-sunum.*` | Yangın güvenliği sunumu (12 slayt) |
| `site-yangin-kontrol-sistemi-sunum-raporu.*` | Teknik sunum raporu |
| `downside_web_entegre.txt` | Firmware komutları: `web_sifirla`, `web_sil_tel`, `web_engelli`, `web_talep_bekliyor`/`web_talep_aksiyon` (onay kuyruğu) |
| `downside_web_ek.txt`, `upside_web_ek.txt` | Firmware web ekleri |
| `upside_web_entegre.txt`, `downside_son_durum_*.txt`, `upside_*.txt` | Firmware entegrasyon dokümanları |
| `web_veri_semasi.txt` | `/data` sözleşmesi (`kullaniciSay`, `aktifSay`, `fire1Start`, `pressureOK`...) |
| `upside_hmi.HMI`, `upside_hmi_analog.HMI` | Nextion HMI projeleri |
| `nextion-*.html/.png`, `nextion-kurulum-rehberi.md` | Nextion ekran diyagramları / kurulum |
| `upside-layer.*`, `downside-layer.*` | Katman diyagramları |
| `web_panel_normal.png`, `web_panel_fire.png` | Panel tam ekran görselleri (1280×900) |
| `panel_upper.png`, `panel_lower.png` | Üst/alt bölüm kırpmaları (kılavuz) |
| `login_card.png`, `login_screen.png` | Giriş ekranı görselleri |
| `upside.PNG` | Teknik panel görseli (sunum) |
| `index.html`, `app.js`, `style.css` | Eski kök panel (eski — **kullanılmıyor**, güncel: `web_panel_data\`) |
| `serve.bat` | Panel sunma yardımcısı |

> Not: `roll-off-*` dosyaları bu projeye ait değildir (ayrı iş fikri) — versiyon klasörüne
> alınmadı.

---

## 📝 DEĞİŞİKLİK GÜNLÜĞÜ

**V01 (2026-08-07) — İlk versiyonlanmış anlık görüntü**
- Kılavuz (10 slayt): giriş talebinin **kuyruğa alındığı**, anında görülmese de yöneticinin
  gördüğünde mutlaka gerekeni yapacağı ve hane başı 3 numara bilgisi eklendi (slayt 2 + 4).
- Üst/Alt bölüm görselleri temiz sınırlardan yeniden kırpıldı
  (`panel_upper.png` 0–848, `panel_lower.png` 858–1480) ve boyutları büyütüldü (87mm / 90mm).
- Sunum (12 slayt): 7/8/9 slayt taşması düzeltildi; 7b (uzaktan izleme) ve 7c (web panelin
  önemi + sakinlere faydaları) slaytları eklendi; 9. slayta kuyruk/onay notu eklendi.

**V01 güncelleme (2026-08-07) — Sürüm etiketi web'e senkron**
- Web panel sürüm etiketi aktif proje versiyonunu gösteriyor: hero-sub `v10` → `V01`,
  footer `HMI v3.0` → `ÇAMKENT AQUA HMI V01`.
- Değiştirilenler (root + bu klasörün ikisinde):
  - `web_panel_data/index.html` (hero-sub + footer)
  - `web_gomulu/web_index_html.h` (gömülü firmware — aynı iki satır)
  - Panel görselleri yeniden üretildi: `web_panel_normal.png`, `web_panel_fire.png`
    (1280×900), `panel_upper.png` (0–848), `panel_lower.png` (858–1480)
  - `Web_Panel_Giris_Sistemi_Kilavuzu.pdf` (10 sayfa) ve `yangin-guvenligi-sunum.pdf`
    (12 sayfa) görseller V01 etiketiyle yeniden basıldı
- Kural: aktif versiyon değiştiğinde (V02...) web etiketi + görseller + PDF'ler birlikte
  güncellenir (bkz. kök `AGENTS.md` → "SÜRÜM ETİKETİ SENKRONİZASYONU").

**V01 güncelleme (2026-08-07) — Sunum/kılavuza versiyon bilgisi + last_update slaytı**
- Sunum kapağına sürüm etiketi eklendi: `Çamkent Sitesi · Bilgilendirme Sunumu · Sürüm V01`.
- Sunuma **last_update** slaytı eklendi (slayt 2, "📋 Son Güncelleme · last_update"):
  V01 değişiklikleri listelenir (tek sürüm etiketi, anında şifre, talep kuyruğu, hane başı
  3 numara, güncel görseller); her yeni sürümde bu liste güncellenir ve duyuru olarak
  ayrıca yayınlanabilir. Sunum slayt sayısı 12 → 13.
- Kılavuz kapağına sürüm etiketi eklendi: `Bilgilendirme · Ağustos 2026 · Sürüm V01`.
- Taşma kontrolü yapıldı: tüm slaytlar `sH == 794` (A4 yatay) — `yardimci_araclar`
  check kopyaları diag_all.js ile yenilendi.
- PDF'ler yeniden basıldı: `yangin-guvenligi-sunum.pdf` (13 sayfa),
  `Web_Panel_Giris_Sistemi_Kilavuzu.pdf` (10 sayfa) — root + bu klasör.
- Kural: her versiyonda `last_update` slaytı güncellenir (bkz. kök `AGENTS.md` → "SUNUM /
  ADMIN / HANE BİLGİLENDİRME").

**V01 güncelleme (2026-08-07) — `downside_web_entegre.txt` web blokları web_panel_data ile senkron**
- `yardimci_araclar\embed_web.ps1` oluşturuldu: `web_panel_data\` içeriğini
  `downside_web_entegre.txt` içindeki `WEB_INDEX_HTML` / `WEB_LOGIN_HTML` / `WEB_STYLE_CSS` /
  `WEB_APP_JS` bloklarına gömüyor; blok dışına dokunmuyor (upsert), dominan satır sonunu
  koruyor. İdempotent (ardışık çalıştırmada dosya değişmiyor — MD5 doğrulandı).
- Gömme sırasında orijinal firmware'deki gömülü `style.css` ile `web_panel_data/style.css`
  arasında `.status-leds` satır sırası farkı bulundu ve firmware güncellendi (işlevsel fark yok,
  yalnız sıralama). Artık 4 blok da `web_panel_data` ile birebir aynı (doğrulandı).
- `downside_web_entegre.txt` root + bu klasörde senkron (MD5 aynı).
- Kök `AGENTS.md`'ye **"🔥 FLASH KAYNAĞI"** bölümü eklendi: kullanıcı flash'ı
  `downside_web_entegre.txt` (web gömülü) ve `upside_web_entegre.txt`'den yapıyor,
  `web_gomulu\*.h` değil; upside web sunmaz. `web_panel_data\` değişiklikleri firmware
  bloğuna da gömülmek zorunda (embed_web.ps1 + git diff doğrulama).

**V01 güncelleme (2026-08-07) — Kuyu kontrolü düzeltildi: yangın tankı hep dolu tutulur**
- Sorun: kuyu motoru, `fireUst=false` (tank dolu değil) iken çalışması gerekirken duruyordu;
  downside yangın tankı hep dolu kalmıyordu.
- Neden: 07/08'de eklenen histerezis mantığı (`fireAlt` kuru olmadan başlatmıyor, iki şamandıra
  arasında son durumu koruyor) tankı yalnızca %100→%50 aralığında dolduruyordu. Ayrıca 1 saatlik
  max çalışma kilidi `fireUst=true` olmadan asla açılamıyordu (deadlock: pompa kapalıyken tank
  dolamadığı için `fireUst` hiç `true` olamıyordu).
- Düzeltme (`downside_web_entegre.txt` → `kuyuControl()`):
  - Kuyu artık **`fireUst` kuru olduğu sürece çalışır, tank dolu (fireUst ıslak) olunca durur**.
    `fireAlt`'a bakılmaz → tank %100'den düşen su her seferinde tekrar doldurulur.
  - Kontak zıplaması `readInputs()` içindeki 300 ms debounce ile zaten korunuyor.
  - 1 saatlik max çalışma koruması KORUNDU ama kilit artık takılı kalmıyor: `fireUst` görülünce
    anında, değilse 10 dk bekleyiş sonrası otomatik açılıp yeniden deniyor (deadlock yok).
- `downside_web_entegre.txt` root + bu klasörde senkron (MD5: `E0232B5A66127FCA6839F1E1A4015A6D`).

**V01 güncelleme (2026-08-07) — opencode çalışma notları netleştirildi**
- Kök `AGENTS.md` → "FLASH KAYNAĞI" bölümüne **"Web panel değişikliğinde tam akış"** eklendi:
  web_panel_data → embed_web.ps1 → web2header.ps1 → doğrula → V01 kopyası (MD5) → commit.
- **Doğrulama tuzağı notu:** `git diff --no-index --stat` + `-match` ile doğrulama YANILTICI
  (CRLF uyarısı nedeniyle farklı dosya "AYNI" görünebilir). Güvenilir: blokları `-join "\n"`
  ile normalleştirip string karşılaştırması yap; tam dosya için hunk incele; idempotentlik için
  2 kez çalıştırıp MD5; root vs V01 için MD5.
- Kanoniklik şüphesinde `git log --oneline -- <dosya>`; örnek: ilk snapshot'ta gömülü
  style.css ile web_panel_data/style.css `.status-leds` sırası farklıydı — gömme her zaman
  web_panel_data'yı esas alır.

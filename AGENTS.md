# AGENTS.md — Çamkent Su Projesi

Bu dosya opencode'un bu projede nasıl çalışacağını belirler. Her oturumda uygulanır.

## 🔁 VERSİYONLAMA PROTOKOLÜ (HER DEĞİŞİKLİKTE UYGULA)

Kullanıcı bir değişiklik istediğinde çalışmaya başlamadan önce **kullanıcıya sor**:

> "Yeni bir versiyon mu yapalım, mevcut projeyi mi güncelleyelim?"

- "Yeni versiyon" → `Çamkent Su projesi_Vxx` numarasını +1 arttır (`V01` → `V02`...),
  **tüm dosyaların kopyasını** yeni klasöre al, çalışmayı orada yap, değişiklik günlüğünü işle.
- "Güncelle" → aynı versiyon klasöründeki dosyalar üzerinde çalış.
- Her değişikliği ilgili klasörün `PROJE_NOTLARI.md` → "DEĞİŞİKLİK GÜNLÜĞÜ" bölümüne yaz.
- Git varsa değişiklikleri commit'le (mesaj Türkçe, açıklayıcı).

## 🎤 SUNUM / ADMIN / HANE BİLGİLENDİRME

Değişiklik **adminleri ya da hane sahiplerini ilgilendiriyorsa**
(kullanım, giriş, alarm, yeni özellik, komut değişikliği vb.) ayrıca sor:

> "Bu durumu sunumlarda belirtelim mi?"

İlgili dosyalar: `yangin-guvenligi-sunum.html/.pdf`, `Web_Panel_Giris_Sistemi_Kilavuzu.html/.pdf`,
`site-yangin-kontrol-sistemi-sunum-raporu.*`.

## 🏷 SÜRÜM ETİKETİ SENKRONİZASYONU (web = aktif versiyon)

Web panelde görünen sürüm etiketi **her zaman aktif proje versiyonunu (Vxx)** gösterir.
Aktif versiyon değiştiğinde (yeni `Çamkent Su projesi_Vxx`) şu hepsi **birlikte** güncellenir:

1. `web_panel_data/index.html` → hero-sub (`·&nbsp; Vxx`) + footer (`ÇAMKENT AQUA HMI Vxx`)
2. `web_gomulu/web_index_html.h` → aynı iki satır (gömülü firmware)
3. Panel görselleri: `web_panel_normal.png`, `web_panel_fire.png`, `panel_upper.png`, `panel_lower.png`
   → webmock ile Chrome `--screenshot` (budget'sız) + `--window-size=1280,1700`; kırpma: üst 0–848, alt 858–1480
4. Sunum/kılavuz PDF'leri → `--print-to-pdf` ile yeniden üret (önce Temp, sonra Remove-Item + Move-Item)

Değişiklikleri root + aktif versiyon klasörünün **ikisine de** uygula, günlüğe işle, commit'le.

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

| Dosya | Açıklama |
|---|---|
| `web_panel_data\` | **GÜNCEL** panel (app.js v12, index.html, login.html, style.css) |
| `web_gomulu\` | Panelin Arduino'ya gömülü halleri (`web_*.h`) |
| `Web_Panel_Giris_Sistemi_Kilavuzu.*` | Kullanım kılavuzu (10 slayt) |
| `yangin-guvenligi-sunum.*` | Yangın güvenliği sunumu (12 slayt) |
| `site-yangin-kontrol-sistemi-sunum-raporu.*` | Teknik sunum raporu |
| `downside_web_entegre.txt` | Firmware komutları (`web_sifirla`, `web_sil_tel`, `web_engelli`, `web_talep_*` onay kuyruğu) |
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
| `index.html`, `app.js`, `style.css` | Eski kök panel (v7/v10 — **kullanılmıyor**, güncel: `web_panel_data\`) |
| `serve.bat` | Panel sunma yardımcısı |

> Not: `roll-off-*` dosyaları bu projeye ait değil (ayrı iş fikri). Onlara dokunma.
> Versiyon klasörlerinde her sürümün `PROJE_NOTLARI.md` dosyası bulunur — en güncel bilgi oradadır.

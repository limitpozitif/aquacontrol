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
| `web_panel_data\` | **GÜNCEL** panel (app.js v12, index.html, login.html, style.css) |
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
| `index.html`, `app.js`, `style.css` | Eski kök panel (v7/v10 — **kullanılmıyor**, güncel: `web_panel_data\`) |
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

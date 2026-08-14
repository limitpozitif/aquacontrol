# PROJE NOTLARI — Çamkent Su Projesi

## DEĞİŞİKLİK GÜNLÜĞÜ

### 11.08.2026 — Downside status/alarm mesajları iyileştirildi (commit `ebfe091`, firmware_versiyon)
- Geçici MCP sapması düzelince "in site/kuyu/hidr control" alarmı artık temizleniyor (önceden ekranda kalıcı kalırdı).
- `mcpSetup()` başarılı olursa "MCP Baglanti Hatasi" alarmı temizleniyor (recovery sonrası eski alarm kalmıyor).
- Yeni alarm mesajları eklendi:
  - `Yangin aktif - Hidrant calisiyor` (gerçek yangın / fire1_start geldiğinde)
  - `Yangin basinc sensoru bayat!`
  - `Upside Baglanti Yok`
- `MAX_ALARM` 10 → 20 (aynı anda 10'dan fazla alarm artık sessizce düşmüyor).
- Dosyalar: `downside_web_entegre.txt` (root) ↔ `firmware_versiyon/downside_web_entegre.txt` (MD5 eşit).
- ~~**BEKLEMEDE:** Bu yeni alarm mesajları sunumda/kılavuzda belirtilecek — kullanıcı "şimdilik sunumu elleme, sonra güncelleyeceğiz" dedi. Sonraki sunum güncellemesinde `last_update` slaytına işlenecek.~~ → 11.08 tarihli inceleme kararı: sunumlara bu alarm mesajları işlenmedi, değişiklik yok (aşağıda).

### 11.08.2026 — Downside `messages` ring buffer eklendi (cloud Messenger widget)
- Upside'daki `MsgRing` yapısı downside'a taşındı: `MSG_BUF_SIZE 20`, `MSG_MAX_LEN 64` (downside alarm metinleri uzun olduğu için upside'ın 32'sinden büyük; 20 = MAX_ALARM ile uyumlu, açılış/patlama senaryosunda ring dolup eski mesajlar ezilmesin).
- `pushMessage()` + `updateCloudMessage()` (1 sn aralıkla ring'den `messages`'a sırayla gönderir) eklendi; loop'un cloud bloğunda çağrılıyor.
- **Filtre kuralı (kullanıcı kararı):** `messages` bir Messenger widget'ı — push edilen kalıcı birikir, silinemez. Bu yüzden **yalnızca sistemi gerçekten engelleyen durumlar** gider; "kontaktör çekti/bıraktı", "in site/kuyu/hidr control" geçişleri, wifi gibi operasyonel gürültü **gitmez**.
- `isBlockingAlarm()` beyaz listesi: faz hatası, inverter hatası, ADS/MCP bağlantı + I2C kopmaları, OUTPUT FROZEN'lar, site/yangın basınç sensörü bayat, kuyuda su yok, site/yangın tankı boş, kuyu max çalışma süresi aşıldı, yangın aktif-hidrant çalışıyor.
- `addAlarm` → blocking ise `pushMessage`; `clearAlarm` → blocking ise `"TEMIZ: ..."` push.
- Cloud komutları da ring'e gidiyor: limit değişimi (alt/üst), sistem/kuyu/hidrant start-stop, MCP yeniden kurulum, upside reset isteği, tüm şifrelerin sıfırlanması.
- Boot'ta `pushMessage("System Started")` (setup sonunda).
- **Cloud reset sonrası mesaj:** restart öncesi ring gönderilemediği için `clr` bayrağı NVS'e yazılır; boot'ta `cloudResetSonrasi` varsa "Cloud reset sonrasi baslatildi" push edilir, sonra bayrak temizlenir. Firmware içi resetler (I2C koptu/FROZEN) bayrak yazmaz.
- **UDP kopukluk eşiği:** "Upside Baglanti Yok" yalnızca `UPSIDE_KOPUK_ESIK_MS` (30 sn) üstü sürekli kopuklukta messages'e gider; kısa kopmalar yalnızca ekranda görünür. Düzelince "TEMIZ:" push edilir (upsideKopukMesajGonderildi bayrağı tekrarı engeller).
- **Yangın sensörü bayat UDP izolasyonu:** `fireSensorBayat` yalnızca `remoteDataOK` iken değerlendirilir — kısa UDP kopması artık "Yangin basinc sensoru bayat!" alarmını tetiklemez. Yangın pompası güvenlik kapısı (`basincGecerli`) değişmedi.
- Root ↔ `firmware_versiyon` senkronlandı, MD5 eşit. Commit'ler: `e3e0385`, `b54f74b`, `ac8005c`, `3dbcaf8`, `ef24b85`.

### 11.08.2026 — Sunum/kılavuz incelemesi (KAPANDI: değişiklik yok)
- Web_Panel_Giris_Sistemi_Kilavuzu + yangin-guvenligi-sunum incelendi; mevcut haliyle geçerli, sürüm etiketi V01'de kalıyor.
- Cloud Messenger bildiriminin sunumlarda anlatılmasına **gerek duyulmadı** (kullanıcı kararı) — yalnızca web panelin `d.alarms` kanalı sunumlarda zaten anlatılıyor; `messages` kanalı web'de görünmez.
- BEKLEMEDE notu kapatıldı.

### 11.08.2026 — yangin-guvenligi-sunum'a "Tel örgü fıskiyeleri" slaytı eklendi
- Yeni slayt 5b (7. slayt): Siteyi çevreleyen tel örgüde 10–15 m'de bir fıskiye bulunur. Yangın anında **yalnızca yangın bölgesindeki fıskiyenin vanası** açılır; diğerleri açılırsa basınç düşer, sistem zora girer. Vanayı açan hane sakini mutlaka yönetimi, görevliyi ve 112'yi arar.
- `last_update` slaytına (slayt 2) fıskiye maddesi eklendi. Sürüm etiketi V01'de kaldı.
- **Taşma düzeltmeleri:** Yeni slayt eklenirken diag ile tüm slaytlar 794px'e çekildi. Önceden var olan 4 taşma da düzeltildi: yangın-çıktı (822), kötü durum (804), webpanel-uzaktan (812), webpanel-önemi (821) → tümü 794. Görseller küçültüldü (upside 44→40mm, panel 36→34mm), li/gap/padding daraltıldı, metinler kompaktlaştırıldı.
- PDF yeniden üretildi (14 sayfa, chrome `--print-to-pdf`, Temp→Remove-Item→Move-Item), diag doğrulaması yapıldı.
  - HTML + PDF güncel; sunum GitHub'da değil (AGENTS.md kuralı).

### 11.08.2026 — Web panelde "Yangın anında ne yapmalıyım?" butonu + gömülü rehber (V01)
- ESP32 web panelde (header) kırmızı <b>"YANGIN ANINDA NE YAPMALIYIM?"</b> butonu eklendi; tıklanınca <b>bilgi penceresi (modal)</b> açılır.
- Modal içeriği: 6 adımlı yangın çıktısı + tel örgü fıskiyelerinin doğru kullanımı (sadece yangın bölgesi açılır; yönetim/112 aranır).
- ESP32 flash'ına PDF gömülemeyeceği için bilgi site HTML/modal olarak gömüldü.
- Dosyalar: `web_panel_data\{index.html, style.css, app.js}` (kanonik) → `downside_web_entegre.txt` (R"CAMKENTP9 blokları) + `web_gomulu/*.h` (PROGMEM başlıklar).
- Senkron: root ↔ `firmware_versiyon\` (downside_web_entegre.txt, web_panel_data, web_gomulu) MD5 eşit.
- Panel görselleri yeniden üretildi (mock server + Chrome --screenshot 1280x1700): `web_panel_normal.png`, `web_panel_fire.png`, `panel_upper.png`, `panel_lower.png`.
- `Web_Panel_Giris_Sistemi_Kilavuzu.html` — k2 (Neler Var) 7. kart + k5 (Başlık çubuğu) buton bilgisi + k10 (Yangın ekranları) not. CSS sıkılaştırma (img max-height, grid4/field gap, card padding); tüm 10 slayt 794px (diag doğrulandı). PDF yeniden üretildi (Temp→Remove→Move).
  - `yangin-guvenligi-sunum.html` `last_update` slayatında (slayat 2) buton maddesi — (kılavuza işlendi).

### 11.08.2026 — Kılavuz şifre oturumu notu düzeltildi (login oturumu)
- `Web_Panel_Giris_Sistemi_Kilavuzu` k8 "5-hatalı kilit" kartı kaldırıldı; "Şifre anlık gelir, hatalı giriş yoktur" yapıldı.
- k4 "Dikkat: 5 kez yanlış = kilit" notu kaldırıldı; yerine şifre anlık oluşturulur/otomatik yazılır ipucu kondu (login.html fetch `/login` hatalı-giriş sayacı yoktur).
- PDF yeniden üretildi; diag 10/10 = 794 OK, metin kontrolü (`kilit` yok, `hatalı giriş yok` var).

### 11.08.2026 — Kılavuz şifre kartı düzeltildi (login oturumu)
- `Web_Panel_Giris_Sistemi_Kilavuzu` k8 "5 yanlış = kilit" kartı kaldırıldı; şifre otomatik alanı
  tanımına göre "Şifre anlık gelir, hatalı giriş yoktur" kartı yapıldı (login.html fetch `/login`
  sadece doğrular; hatalı giriş sayacı firmware'de yoktur).

### 14.08.2026 — Upside firmware: `stop_sys` durumunda RELAY1 kurtarma tetiklemesi + boot gecikmesi
- `loop()` içindeki `if(stop_sys)` bloğuna geçici relay kurtarma eklendi: `digitalWrite(RELAY1,HIGH)` → `delay(200)` → `LOW` → `delay(200)` → `return`.
- Neden: role kartındaki röleler üretim hatası nedeniyle bazen açıldığı halde kontağı temas etmiyor; bu çift tetikleme kontak temassızlığını gidermek için.
- Boot gecikmesi 15 sn → 5 sn (`bootDelayDone` eşiği `millis() >= 15000` → `5000`), sistem daha hızlı ayağa kalkıyor.
- Kapsam: Yalnızca upside firmware (`upside_web_entegre.txt`, flash kaynağı). Web panelde görünen sürüm etiketi değişmedi — upside web sunmuyor, sürüm etiketi taşımıyor; V01 geçerli kalıyor.
- Sunum/kılavuz güncellenmedi (kullanıcı kararı: admin/hane sahiplerini etkileyen görünür davranış değişikliği yok).
- Senkron: root ↔ `firmware_versiyon\upside_web_entegre.txt` ↔ `firmware_versiyon\Çamkent Su projesi_V01\upside_web_entegre.txt` (MD5 eşit).

### 14.08.2026 — Upside: otomatik dolum valfi kurtarma (`dolumRescue`) + `role_hatasi` (V01)
- Sorun: Rölay kontağı üretim hatası nedeniyle valf açılması gerektiğinde temasa gelmiyor; valf açılmıyor, yangın basıncı 1.0 bar üzerinde kalıyor.
- Teşhis: `oto_dolum` aktifken yangın basıncı **5 sn** içinde **1.0 bar** altına inmezse valf açılmamıştır (koşullar: sensör sağlıklı `fireSensorOK` + tank < %99).
- Kurtarma: RELAY1 **12 kez** tıklatılır (her loop turunda 1 pulse → `ArduinoCloud.update()` loop üstünde olduğu için bağlantı canlı kalır).
- **2 deneme** sonrası hâlâ düşmezse → **`role_hatasi = true`** (cloud alert). Latch: kullanıcı müdahale edene kadar tekrar denenmez.
- **`role_hatasi`**: yeni cloud bool — Arduino Cloud'da eklenip alert'e bağlanacak. Sistem resetlenince `false` olur (setup), sonraki dolumda yeniden değerlendirilir.
- **messages bildirimleri** (pushMessage): "Dolum valfi kurtarma basladi" / "Dolum valfi kurtarildi" / "Dolum valfi acilmadi!".
- **fire1Blind guard:** sensör ölü + ekrandan zorla `fire1_start` iken (pompa 40 Hz basarken) dolum başlatılmaz, aktifse durdurulur — kısır döngü önlenir.
- Dosya: `upside_web_entegre.txt` (flash kaynağı). Sürüm etiketi değişmedi (V01). Sunum/kılavuz güncellenmedi.
- Senkron: root ↔ `firmware_versiyon\upside_web_entegre.txt` ↔ `firmware_versiyon\Çamkent Su projesi_V01\upside_web_entegre.txt` (MD5 eşit).

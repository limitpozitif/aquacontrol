# PROJE NOTLARI — Çamkent Su Projesi

## DEĞİŞİKLİK GÜNLÜĞÜ

### 21.08.2026 — Upside: updateDisplay stack taşması düzeltildi + dolumRescue hayalet-latch teşhisi (RoleHatasi READ_ONLY)
- **Stack taşması (düzeltildi):** `updateDisplay()` içindeki mesaj karşılaştırma tamponu `prevMsg[32]` idi; `lastMsg` ise 128 char taşıyabiliyor (`MSG_MAX_LEN=128`). Kopya sınır kontrolsüz `strcpy(prevMsg, lastMsg)` ile yapılıyordu — `"Yangin (fire1) pompasi calistirildi (ekran)"` gibi 32+ karakterli her mesajda ~12 byte stack üzerine yazılıyordu (sessiz hasar/rastgele davranış riski). Düzeltme: `strncmp` karşılaştırma + `strncpy(..., sizeof(prevMsg)-1)` + NULL sonlandırma.
- **dolumRescue hiç çalışmıyordu — kök neden:** Bulutta `RoleHatasi` READ_WRITE olduğu için bağlantı handshake'inde sunucu sakladığı eski `true` değerini cihaza geri yazıyordu. `setup()`'ta temizlenen latch böylece her boot'ta hayalet olarak geri geliyordu ve satır `if (role_hatasi) return;` tüm kurtarma teşhisini sessizce blokluyordu. Semptomlar: telefon trigger'ı geliyordu ama bulutta mesaj görünmüyordu, valf takılıyken alarm/tıklama yoktu, reset sonrası hemen role_hatasi oluşuyordu. **Çözüm: RoleHatasi Arduino IoT Cloud Thing'de READ_ONLY yapıldı** (cihaz→bulut tek yön; sync-back bitti). Düzeltme sonrası kurtarma mesajları gelmeye başladı — doğrulandı.
- **Bilinen kalıcı riskler (şimdilik dokunulmadı):** dolumRescue mutlak 1.0 bar eşiği / tek-loop `oto_dolum` sıfırlaması / sensör bayatken sessiz atlama.
- **`messages` da bulutta READ_ONLY yapıldı:** Handshake'te eski metnin cihaza geri yazılması ortadan kalktı → aynı-değer bastırmasının ana mekanizması (boot arası) çözüldü. Oturum içinde arka arkaya birebir aynı metin tekrarlanırsa yine yayınlanmaz (pratikte arada başka mesajlar geçtiğinden önemsiz).
- **`udpReceive` reset_upside davranışı BİLİNÇLİ:** Downside/panel tarafında `reset_upside` bayrağı, uzaktan reset komutu olarak kullanılıyor ve gerçekten reset olduğu görünce elle kapatılıyor — sürekli-true restart döngüsü riski değil, tasarım. Çalıştırma notu: bayrağı kapatmak için upside'ın boot'u bitmesini BEKLEMEYE GEREK YOK; tam tersine, upside ayağa kalkıp ilk paketi almadan önce kapatılırsa ikinci restart asla oluşmaz. Bayrak true iken cihaz online göründüyse ilk gelen paket yeniden restart tetikler.
- **broadcastIP maddesi değerlendirildi ve geçildi:** IP değişikliği aynı subnet'te (sahada sabit `192.168.1.x`) olduğunda broadcast adresi değişmediğinden sorun oluşturmaz; yalnızca modem değişimi/subnet değişiminde etkisiz hale gelir (belirti: panel verisi sessizce donar). Sahada risk yok kabul edildi — dokunulmadı.
- Not: Bu txt tek başına derlenmez — bulut tarafındaki READ_ONLY dahil üretim için `thingProperties.h`'nin de firmware arşivinde saklanması gerekir.
- Senkron: root ↔ `firmware_versiyon\upside_web_entegre.txt` ↔ `firmware_versiyon\Çamkent Su projesi_V01\upside_web_entegre.txt` MD5 eşit (`ED418ED7...`).
- Güvenli yedek: 21.08.2026 22:54 itibarıyla upside + downside son durumları `firmware_versiyon\SAFE_BACKUP\*_GUVENLI_2026-08-21_2254.txt` altına alındı (MD5 doğrulamalı; upside `ED418ED7...`, downside `169A776E...`).

### 17.08.2026 — Upside: dolumRescue delay→millis() dönüşümü + cloud veri filtresi
- **dolumRescue delay→millis():** `delay(200)` × 2 × 12 pulse = **4.8 sn** loop blok. `millis()` tabanlı alt durum makinesi ile loop kilitlenmesi kaldırıldı (~2.5 sn, blok yok).
- **Cloud veri trafiği filtresi:** Akım ve frekans değişkenlerine eşik eklendi (`CLOUD_CURR_ESIK=0.05A`, `CLOUD_FREQ_ESIK=0.05Hz`). Küçük dalgalanmalar cloud'a gönderilmiyor → Maker planı 10 MB/ay limitineprise. Tank yüzdeleri zaten `round(...*10)/10` ile yuvarlanmış, hatalar (bool) nadiren değişiyor — onlara dokunulmadı.
- Yeni değişkenler: `kurtarmaSub`, `kurtarmaPulseZaman`, `CLOUD_CURR_ESIK`, `CLOUD_FREQ_ESIK`.
- MD5: `2B5BF01B...`, root ↔ firmware_versiyon ↔ V01 eşit.

### 17.08.2026 — Downside: WEB_PORT 8080→8888 (Zyxel NAT bug çözümü) + statik IP .167 + kılavuz güncellendi
- **Neden:** Zyxel VMG3312-T20A modemde manuel port forwarding kuralı aynı porttaki iç erişimi engelliyor (bilinen bug). 4 farklı port (80, 8081, 8888, 9090) içeriden çalışırken port 8080 çalışmıyordu. Kural inactive yapılınca 8080 çalışmaya başladı → modem NAT mühendisliği hatası doğrulandı.
- **Çözüm:** `WEB_PORT` 8080→8888 + firmware'de `WiFi.config(192.168.1.167, ...)` statik IP eklendi. Modemde `esp: TCP 8888→192.168.1.167:8888` port forwarding kuralı ayarlandı. **Modemde static DHCP rezervasyonu YAPILMAYACAK** — daha önce 8080 için yapılan modem değişiklikleri herşeyi bozdu. Statik IP firmware tarafında, modemde değişiklik yok.
- Panel içeriden test edildi (`192.168.1.167:8888` çalışıyor), dışarıdan test henüz yapılmadı.
- `WiFi.setSleep(false)` ve `ipCakismaKontrol()` kaldırılması (16.08) korunuyor — test edildi ve sorunsuz.
- Kılavuz HTML + PDF güncellendi (`8080→8888`). Sunum dosyalarında port referansı yok.
- MD5: `3A546663...`, root ↔ firmware_versiyon ↔ V01 eşit (3'lü doğrulama).

### 16.08.2026 — Downside: statik IP geri alındı (DHCP'ye dönüş) — tanı testi
- Statik `192.168.1.245` (14.08, commit `1553cf4`) kaldırıldı; ESP IP'yi modem DHCP'sinden alıyor. `WiFi.setSleep(false)` duruyor.
- **Neden:** Panel 14.08'den beri ölü; statik IP geçişi DHCP lease/ARP bağını kopardı (modemin `.245` eşleşmesi süresi doldu). Modemde `.245` DHCP rezervasyonu hâlâ duruyorsa ESP aynı adresi alır ve 8080 NAT kuralı çalışmaya devam eder.
- **Test sonucu:** Panel açılırsa statik IP suçlu (kalıcı çözüm: modem DHCP rezervasyonu `.245` → ESP MAC). Açılmazsa bugünkü hâle dönülür — geri dönüş etiketi: `son-durum-statik-ip-16-08`.
- **✅ Test sonucu (kullanıcı):** ESP DHCP'ten `192.168.1.243` aldı → modem `.245`'i ESP'ye vermiyor, `.245` bağı modemde kaybolmuş (kamera `.17`, rezervasyon yok). **Sorun modem tarafında doğrulandı.** Çözüm: modemde DHCP rezervasyonu `.245` → ESP MAC (MAC modem istemci listesinden veya kalp atışı `ben=` alanından), NAT 8080→`.245` kuralı duruyor. Flash gerekmez.
- **ARP tanı dedektörü kaldırıldı:** Teşhis tamamlandı; fonksiyon ve `delay(600)` kaldırıldı — web sunucusu artık her loop'ta kesintisiz çalışır.
- Kalp atışı MAC/BSSID/RSSI eklenmesi (`e003125`) DHCP'ye dönünce de geçerli — flash sonrası logda ESP'nin aldığı IP + MAC + AP görünür.
- Senkron: root ↔ `firmware_versiyon\downside_web_entegre.txt` ↔ `firmware_versiyon\Çamkent Su projesi_V01\downside_web_entegre.txt` MD5 eşit (`1D3DD083...`), brace 790/790.

### 16.08.2026 — Downside: IP çakışması tanı dedektörü (ARP) eklendi
- **Amaç:** Sahada web paneli (`http://85.104.57.50:8080`) aralıklı yanıt vermiyor; ana şüphe statik `192.168.1.245` adresinin bir kamera/Xmeye cihazıyla çakışması. Cloud'a yeni değişken eklenemez (`thingProperties.h` generated) → rapor mevcut `messages` (Messenger) property'si üzerinden gider.
- **Düzeltme:** `ipCakismaKontrol()` eklendi — boot'ta WiFi bağlandıktan sonra bir kez kendi IP'si için ARP sorgusu atar (`etharp_request` + `etharp_find_addr`, gmag11 yöntemi); dönen MAC kendi MAC'imiz değilse `pushMessage("IP CAKISMASI! <MAC> rakip")` gönderilir.
- **Tanı amaçlıdır;** çözüm bulununca kaldırılacak (çakışma doğrulanırsa: modem DHCP rezervasyonu veya statik IP havuz dışına alınır).
- **Derleme düzeltmesi:** `esp_netif_get_netif_impl` ESP32 core 2.0.17 header'ında tanımlı değildi; lwIP `netif_find("en0")` + `netif_list`/`netif_is_up` fallback'ine geçildi (gmag11 gist yöntemi).
- **Periyodik kontrol:** Tek seferlik çalışma boot anında periyodik ARP duyurusuyla IP kapabilen cihazı (kamera) yakalayamadı — kontrol 60 sn'de bir tekrarlanacak şekilde güncellendi. Çakışma bulununca bir kez raporlanır, temizlenince yeniden raporlamaya hazır olur.
- **Teşhis kalp atışı:** "Çakışma mesajı çıkmadı" sonucu belirsizdi (kontrol çalışmıyor olabilir) — kontrol artık sonucu DEĞİŞTİĞİNDE Messenger'a basıyor: `ARP: ... kendi MAC (temiz)` / `IP CAKISMASI! ...` / `cozumlenemedi`. İlk kontrol her zaman basar → kontrolün çalıştığını ve ARP tablosunu görürüz. (Saha: port 80 kamera açık 347ms, port 8080 panel ölü → sorun `.245`'e gelen yol, modem ARP/NAT şüphesi.)
- **Sonuç 1:** ESP'nin ARP'si `cozumlenemedi` döndü → kontrol çalışıyor, `.245`'i sahiplenen rakip cihaz yok (ARP çakışması elendi). Periyodik duyurular modemi iyileştirmedi. TCP 8080 SYN bile yanıt almıyor (8 sn), TCP 80 ise 28 ms'de bağlanıyor → gelen paketler ESP'ye varmıyor.
- **Sonuç 2 (modem sleep):** Firmware'de `setSleep`/`WIFI_PS` yoktu → ESP32 core'un varsayılan `WIFI_PS_MIN_MODEM` (modem sleep) aktifti. "Boot'ta bir kez açılıp sonra ölme" desenini açıklayabilir (boot sonrası radyo sürekli uyanık, sonra uyku pencerelerinde inbound SYN kaçar; outbound MQTT çalıştığı için cloud canlı kalır). `WiFi.setSleep(false)` eklendi — web sunucusu için zorunlu. Bu düzeltme çözmezse sorun kesin modem tarafında (NAT kuralı kalkmış/yanlış MAC, DHCP rezervasyonu gerek).
- **Kalp atışı + ağ bilgisi:** Mesajlara ESP'nin **kendi MAC adresi**, bağlı olduğu **AP'nin BSSID'i** ve **RSSI'si** eklendi (`| ben=%s AP=%s RSSI=%d dBm`) — modemde DHCP rezervasyonu kurmak için ESP'nin MAC'i logdan okunur, önceden bilinmesi gerekmez. Ayrıca hangi AP'ye bağlandığı (BSSID) ve sinyal gücü görülür.
- **Mesaj kesilmesi düzeltildi:** `MSG_MAX_LEN 64` → `128` — kalp atışı mesajı (~91 karakter) 64'te kesiliyordu (cloud'da yarım görünüyordu, örn. `... AP=30` diye bitiyordu). Ring belleği 20×128 = 2,5 KB (ESP32 için önemsiz).
- Senkron: root ↔ `firmware_versiyon\downside_web_entegre.txt` ↔ `firmware_versiyon\Çamkent Su projesi_V01\downside_web_entegre.txt` MD5 eşit (`59921B6F...`), brace 790/790.

### 14.08.2026 — Upside: dolum başlangıç/bitiş mesajı reboot'ta kaybolmasın (boot raporu)
- **Sorun:** `pushMessage` ring'i RAM'de; reboot/flash olursa cloud'a iletilmemiş "Otomatik dolum basladi" mesajı kayboluyor (kullanıcı doğruladı).
- **Düzeltme:** `dolumRaporlandi` bayrağı eklendi. Dolum valfi bloğu artık `dolum != oto_dolum || (!dolumRaporlandi && oto_dolum)` koşulunda çalışır — boot'ta valf aktifse (oto_dolum=true) durum mesajı bir kez yeniden gönderilir; normalde yalnızca durum değişiminde gider.
- Senkron: `firmware_versiyon\upside_web_entegre.txt` ↔ `firmware_versiyon\Çamkent Su projesi_V01\upside_web_entegre.txt` MD5 eşit (`D2FA8AC0...`), brace 299/299.

### 14.08.2026 — Upside: tank yüzde karşılaştırmaları 1 ondalığa yuvarlandı
- `tank1_yuzde` ve `fire_yuzde` karşılaştırma/atama değerleri `round(x * 10) / 10` ile 1 ondalığa yuvarlanıyor — 4 tank ortalaması (örn. 45.25) artık 1 ondalıkla (45.3/45.2) karşılaştırılır, float hassasiyeti yüzünden gereksiz değişiklik tetiklenmez.

### 14.08.2026 — Downside Rst_sys kilitli kalma sorunu çözüldü (cloud'a false iletimi)
- **Belirti:** Cloud'da `Rst_sys` sürekli `true` görünüyor, ESP32 reset döngüsüne giriyor (I2C/FROZEN alarmı yok).
- **Kök neden:** `onRstSysChange()` cloud'dan `true` alınca `hardreset` kuruyor ama firmware `ESP.restart()` öncesi `Rst_sys=false` değerini **cloud'a iletmiyordu**. Cloud'da `true` kilitli kalınca ESP her yeniden bağlandığında handshake o değeri geri yolluyor → sonsuz reset döngüsü.
- **Düzeltme:** Loop reset bloğunda `Rst_sys = false;` hemen ardına `ArduinoCloud.update();` eklendi — restart öncesi false değeri cloud'a iletilir, cloud'daki kilitli true temizlenir, döngü kırılır.
- **Acil çözüm:** Cloud dashboard'da `Rst_sys` OFF yapılana kadar döngü sürer (flash sonrası da dashboard'daki eski true tekrar tetikleyebilir — firmware düzeltmesi ilk reset'te kendini temizler).
- Senkron: `firmware_versiyon\downside_web_entegre.txt` ↔ `firmware_versiyon\Çamkent Su projesi_V01\downside_web_entegre.txt` MD5 eşit (`ACAE5B3E...`), brace 780/780.

### 14.08.2026 — Downside'a statik IP eklendi (192.168.1.245)
- Downside web paneli artık DHCP yerine **sabit `192.168.1.245`** adresini kullanır — modem/NAT kuralları değişse de ESP aynı IP'de kalır, modeme yeniden yönlendirme yapmak gerekmez.
- `setup()` başına `WiFi.config()` eklendi: IP `192.168.1.245`, gateway `192.168.1.1`, subnet `255.255.255.0`, DNS `192.168.1.1` (modem/gateway). `ArduinoCloud.begin()` öncesi çağrıldığı için statik yapılandırma uygulanır.
- Kart: FireBeetle ESP32 V4.0 (ESP32-WROOM-32E) — `WiFi.config()` API'si ESP32 core'da tüm kartlarda aynıdır.
- ⚠️ Modemde `.245` IP'si **DHCP havuzundan çıkarılmalı** — aynı IP başka bir cihaza atanırsa IP çakışması olur.
- Sunum/kılavuz güncellenmedi: panel adresi (`http://85.104.57.50:8080`) değişmiyor, kullanıcıyı etkileyen görünür davranış değişikliği yok.
- Senkron: `firmware_versiyon\downside_web_entegre.txt` ↔ `firmware_versiyon\Çamkent Su projesi_V01\downside_web_entegre.txt` MD5 eşit (`814BE1F9...`).

### 14.08.2026 — System Started bağlantı handshake'ine takılıyordu (3 sn bekleme eklendi)
- **Kök neden:** Mesaj nereden push edilirse edilsin (5 sn boot gecikmesi, boot sayacı, `setup()`), `updateCloudMessage` bağlanır bağlanmaz ring'i tüketiyordu. ESP32 cloud'a bağlandığı anda sunucu tüm property değerlerini geri gönderir (ilk senkron/handshake); bu pencerede `messages`'a atanan ilk değer sunucunun eski değeriyle üzerine yazılıyordu. Bu yüzden System Started hiç görünmüyor ama dakikalar sonra gelen `RESET ...` mesajları görünüyordu.
- **Düzeltme (`upside_web_entegre.txt`):** `updateCloudMessage`'a `connectedSince` eklendi — `ArduinoCloud.connected()` ilk kez true olduktan **3 saniye sonrasına** kadar ring tüketilmez; mesajlar ring'de birikir, handshake bittikten sonra temiz pencerede sırayla gönderilir. (Kullanıcı testi: boot sayacı ve `setup()` push'u dahil hiçbir yöntem çalışmıyordu → bu mekanizma kanıtlandı.)
- Doğrulama: brace 299/299; root ↔ `firmware_versiyon` ↔ V01 MD5 eşit (`A32FD29A...`).

### 14.08.2026 — Downside geri alındı (boot sayacı kaldırıldı)
- Kullanıcı isteği: downside ile uğraşılmıyor. `firmware_versiyon\downside_web_entegre.txt` içindeki `systemStartedMsg()` (boot sayacı) eklentisi geri alındı, `pushMessage("System Started")` orijinal haline döndürüldü; V01 kopyasıyla MD5 eşit (`7F1D4DA8...`).
- Upside'daki boot sayacı (System Started (n)) duruyor.

### 14.08.2026 — "System Started" mesajı her boot'ta görünmez oluyordu (boot sayacı eklendi)
- **Kök neden:** Arduino Cloud, `messages` String'ine atanan değer bir öncekiyle **aynıysa değişiklik saymaz** ve Messenger widget'ına göndermez. `"System Started"` sabit string olduğundan ilk boot'ta kaydedildikten sonra sonraki boot'larda sessizce kayboluyordu (diğer mesajlar farklı olduklarından geliyor).
- **Düzeltmeler:**
  - Upside (`upside_web_entegre.txt`): `systemStartedMsg()` eklendi — NVS'teki `bootSayac` sayacı her boot'ta artırılır, mesaj `System Started (n)` biçiminde push edilir; `:2103` `pushMessage("System Started")` → `systemStartedMsg()`.
  - Downside (`firmware_versiyon\downside_web_entegre.txt`): aynı `systemStartedMsg()` eklendi, `pushMessage("System Started")` çağrısı onunla değiştirildi.
- Doğrulama: upside brace 298/298; root ↔ `firmware_versiyon` ↔ V01 MD5 eşit (`84A13758...`); downside fv ↔ V01 MD5 eşit (`DD443E3F...`).

### 14.08.2026 — Upside mesaj kaybının asıl kök nedeni çözüldü (cloud bağlantı kilidi)
- **Kök neden:** `messages` tek bir Arduino Cloud String. Cloud bağlı değilken (boot'un ilk 5 sn'si dahil) ring'den `messages`'a atanan değerler üst üste biniyor ve cloud'a yalnızca **sonuncusu** ulaşıyordu — üç reset mesajından yalnızca inv 8'in görünmesi bundandı. Ring tarafındaki overwrite zaten kaldırılmıştı; kayıp cloud gönderimindeydi.
- **Düzeltmeler (`upside_web_entegre.txt`):**
  - `updateCloudMessage()`: `if (!ArduinoCloud.connected()) return;` eklendi — bağlı değilken ring tüketilmez, mesajlar ring'de birikir; bağlanınca 500ms aralıkla sırayla gönderilir (downside'daki `ArduinoCloud.connected()` deseni).
  - `:2103` `messages = "System Started"` doğrudan ataması kaldırıldı → `pushMessage("System Started")` (bekleyen mesajları ezmez, ring'den sırayla gider).
- **✅ Saha doğrulaması (kullanıcı):** mesajların hepsi geldi — sorun çözüldü.
- Doğrulama: brace 297/297; root ↔ `firmware_versiyon` ↔ V01 MD5 eşit (`C8DE91BD...`).

### 14.08.2026 — Upside bayat uyarılarına TEMIZ (kurtarma) mesajı eklendi
- `sensorSaglikKontrol()`: site/yangın sensörü bayat uyarısı artık **bir kez** push ediliyor (aktif mesaj `siteAktifMsg`/`fireAktifMsg`'de hatırlanıyor); sensör düzelince ring'e bekleyen hata mesajı kalmadığından **`TEMIZ: ...`** push ediliyor (downside'daki clearAlarm davranışının upside karşılığı).
- `fireAlarmOn` dalında 40Hz blind modu mesajı da düzelince TEMIZ ile kapatılıyor.
- **Ring dolmaz:** `updateCloudMessage` gönderdikçe ring'i boşaltıyor (count=0); TEMIZ de gönderilince kalkar. Tekrar eden bayat uyarıları da dedupe ile zaten 1 kez giriyor.
- Doğrulama: brace 297/297; root ↔ `firmware_versiyon` ↔ V01 MD5 eşit (`BD816563...`).

### 14.08.2026 — Upside mesaj ring'inde overwrite kaldırıldı (hiçbir mesaj ezilmez)
- `pushMessage`: ring doluysa yeni mesaj **eklenmiyor** (reddedilir) — önceki davranışta `head` dönüp **en eski bekleyen mesajın üzerine yazıyordu** (`count` 20'de sabit kalıyordu). Artık hiçbir bekleyen mesaj üzerine yazılamaz; aynı mesaj ring'de zaten varsa da eklenmez.
- Böylece "üç reset mesajından yalnızca inv 8 görünüyor" senaryosu kalıcı olarak çözüldü: 6/7 gönderilmeden ezilemez.
- Doğrulama: brace 295/295; root ↔ `firmware_versiyon` ↔ V01 MD5 eşit (`68FA0C7A...`).

### 14.08.2026 — Upside mesaj ring ezilmesi düzeltildi (reset mesajları kayboluyordu)
- **Kök neden:** Boot'ta 3 inverter de (`resetted=false` → site/fire/fire1 control) sırayla resetleniyor; her biri "RESET COMPLETE SLAVE x" push ediyor. `updateCloudMessage` 1 sn'de yalnızca 1 mesaj gönderirken, tekrarlayan `UYARI: Sensor bayat!` mesajları 500ms dedupe ile ring'i tıka basa dolduruyordu → 6 ve 7'nin reset mesajları gönderilmeden eziliyor, yalnızca son eklenen inv 8 görünüyordu.
- **Düzeltmeler (`upside_web_entegre.txt`):**
  - `pushMessage`: dedupe yalnızca son mesaja değil **ring'in tamamına** bakıyor — aynı mesaj zaten ring'deyse eklenmiyor (bayat uyarıları ring'i tıkamaz).
  - `updateCloudMessage`: 1 sn → **500 ms** aralık (ring 2× hızlı boşalıyor).
  - `RESET FAIL`, `RESET CORRUPT RESP`, `RESET TIMEOUT` durumlarına da `pushMessage` eklendi (önceden yalnızca `rstmsg` string'ine atanıyor, hiç gönderilmiyordu).
- Doğrulama: brace 295/295 dengeli; root ↔ `firmware_versiyon` ↔ V01 MD5 eşit (`45EDE482...`).

### 14.08.2026 — Upside `MSG_BUF_SIZE` 8 → 20 (kullanıcı tarafından)
- Kullanıcı upside firmware'inde `#define MSG_BUF_SIZE 20` yaptı (downside ile uyumlu; downside zaten 20).
- Senkron: root ↔ `firmware_versiyon` ↔ `firmware_versiyon/Çamkent Su projesi_V01` upside kopyaları MD5 eşit (`34FE448A...`).

### 14.08.2026 — Upside tank okuma 65535 koruması (EMA filtresi)
- Nano'lardan gelebilecek `level == 65535` (veya `> 4500`) değeri upside'ın EMA filtresine karışmasın diye koruma eklendi (`upside_web_entegre.txt`, PHASE_TANK cevap bloğu).
- Geçersiz okumada `tankLevelSm` filtresine **dokunulmuyor**, yalnızca `tankLevel = 65535` işaretleniyor; mevcut "okunamayan tankları ortalama ile doldur" mekanizması (adet>0) tamamlıyor.
- Nedeni: hem ORİJİNAL hem yeni üst blok Nano firmware'inde ara sıra 65535 düşebiliyor; 65535 filtraya girerse `tankYuzde`/`tankTonaj` saçma değer üretip sonraki döngüleri de bozuyordu.
- Downside tank verisini işlemediği için (UDP/tank yok) değişiklik yalnızca upside'ta.
- Senkron: root ↔ `firmware_versiyon` ↔ `firmware_versiyon/Çamkent Su projesi_V01` MD5 eşit (`941E2B64...`).

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

### 14.08.2026 — `dolumRescue` derleme hatası düzeltildi (bayat kopya callback'leri içermiyordu)
- **Hata (linker):** `undefined reference to onFireFrqSetChange() / onSiteFrqSetChange() / onRoleHatasiChange()` — Arduino Cloud Thing'inde tanımlı READ_WRITE property'lerin callback fonksiyonları firmware'de eksikti.
- **Kök neden:** Düzenlemeler `firmware_versiyon\` repo kopyası üzerinden yapılmıştı; o kopya **bayattı** ve Thing'in gerektirdiği 3 callback'i içermiyordu. Kullanıcının gerçek (derlenen) firmware'i **root** dosyasıydı.
- **Düzeltme:** `dolumRescue` özelliği **root'taki gerçek firmware'e** yeniden uygulandı — callback'ler (`onFireFrqSetChange`, `onSiteFrqSetChange`, `onRoleHatasiChange`) korunarak. Ayrıca bayat kopyanın başındaki API key satırları da temizlendi.
- **Değişken adı:** kullanıcı onayıyla property adı **`role_hatasi` (küçük harf)** — firmware bu adı kullanıyor.
- **fire1Blind guard** artık mevcut `fire1Blind` değişkeniyle yazıldı (root'ta zaten tanımlı, `fire1_start && fire1_local && (!fireSensorOK || !ads1115Ok)`).
- Root ↔ `firmware_versiyon` ↔ V01 yeniden senkronlandı, **MD5 eşit (FFE...)**.

### 14.08.2026 — `dolumControl` fire1Blind scope hatası düzeltildi
- Derleme hatası: `fire1Blind` yalnızca `fire1Control()` içinde yerel değişken (satır 1432); `dolumControl()`'de tanımsızdı → `'fire1Blind' was not declared in this scope`.
- Düzeltme: guard inline ifadeye çevrildi: `if (fire1_start && fire1_local && (!fireSensorOK || !ads1115Ok))` — `fire1Control()`'deki tanımla birebir aynı mantık.
- Root ↔ `firmware_versiyon` ↔ V01 senkron, MD5 eşit (497...).

### 14.08.2026 — Tank seviye okuma stabilitesi iyileştirildi (`tank_seviye_okuma.txt` üst aktif blok)
- Kapsam: **yalnızca dosyanın üstündeki aktif blok** — ORİJİNAL bölüme dokunulmadı.
- `ORNEK_SAYISI` 3 → **5** (daha sağlam median).
- **Sıçrama koruması eklendi:** yeni değer son iyi değerden `SIPRAMA_ESIK_MM` (300mm) fazla saparsa şüpheli sayılır, hemen uygulanmaz; sonraki ölçüm aynı bölgede kalırsa gerçek değişim kabul, eski seviyeye dönerse sıçrama yok sayılır.
- İlk geçerli okuma doğrudan kabul edilir (kalibrasyon).
- Mevcut lastGood/65535 (3 ardışık hata) ve watchdog mantığı korundu.
- `tank_seviye_okuma.txt` `.gitignore`'a eklendi (yerel kaynak dosyası, GitHub'a gitmez).

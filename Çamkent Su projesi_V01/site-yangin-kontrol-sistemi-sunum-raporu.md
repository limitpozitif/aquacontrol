# Site Suyu ve Yangın Hattı Kontrol Sistemi
## Sunum Raporu — En Basit Dille

---

## 1. Bu sistem ne işe yarar?

Bu sistem bir sitenin iki kritik işini **otomatik** yapar:

1. **Kullanma suyu (site suyu):** Siteyi öncelikle **şehir şebeke suyu** besler. Şebeke basıncı yeterliyse (site basıncını istenen seviyeye çıkarabiliyorsa) **her iki istasyondaki site motorları boştadır**; basınç düşünce pompa otomatik devreye girer ve basıncı ideal aralıkta tutar.
2. **Yangın hattı:** Bir yangın olursa (hidrant açılırsa) basınç düşer, yangın pompaları otomatik devreye girer ve yangın hattını basınçlı tutar. Yangın tankları biterse otomatik doldurulur.

Sistem iki ayrı cihazdan oluşur: **UPSIDE** (üst istasyon) ve **DOWNSIDE** (alt istasyon). İkisi kablosuz olarak birbiriyle haberleşir.

> Benzetme: Downside, suyu kuyudan çekip yukarı iten "kuyu başı"dır. Upside, tankların ve yangın hattının bulunduğu "depo yönetimi"dir. İkisi **aynı boru hatlarını** besler ve basınç kontrolünde **birbirini tamamlar**.

---

## 2. Kot farkı (60 metre) — bu sistemin anahtarı

- Upside ile Downside arasında **yaklaşık 60 metre yükseklik (kot) farkı** vardır. Downside altta, Upside üsttedir.
- **60 metre su sütunu ≈ 6 bar basınç** demektir. Yani aynı hat üzerinde, su dolu bir borunun **alt ucundaki sensör, üst ucundakinden ~6 bar daha yüksek** okur.
- Bu yüzden limitler iki cihazda farklıdır ama **aynı fiziksel durumu** temsil eder:
  - Upside (üst): **2.3–3.0 bar**
  - Downside (alt): **7–9 bar** (≈ 1–3 bar + 6 bar kot payı)
- Her iki taraf da kendi sensörünü kendi limitleriyle değerlendirir; sonuç aynı hattı korumaktır.

---

## 3. Donanım haritası

| Cihaz | Görevi | Bağlı olanlar |
|-------|--------|----------------|
| **UPSIDE** (Cihaz 1, üst) | Tank yönetimi + hattı üstten basınçlı tutma | 5 ultrasonik tank sensörü, 2 basınç sensörü (site + yangın), 3 sürücülü pompa (site + küçük yangın + büyük yangın), dolum valfi, RS485 (Modbus), Nextion dokunmatik ekran, Arduino IoT Cloud |
| **DOWNSIDE** (Cihaz 2, alt) | Kuyu + hattı alttan destekleme | MCP23017 röle kartı (4 pompa çıkışı), 2 adet 4-20mA basınç sensörü (site hattı + belediye giriş suyu), **site tankı 10 ton (şehir şebekesiyle dolar, alt sensör) + yangın tankı 5 ton (kuyuyla dolar, alt/üst sensör)**, faz kontrolü, Nextion ekran, Arduino IoT Cloud |

---

## 4. İki cihaz nasıl konuşuyor? (UDP)

- Her cihaz **1 saniyede bir** diğerine durum paketi gönderir (aynı ağda).
- **Upside → Downside:** **Yangın hattı basıncını** gönderir. Çünkü yangın hattının **tek basınç sensörü Upside'dadır**. Downside, hidrant pompasını bu gelen değere göre çalıştırır.
- **Downside → Upside:** "kendini yeniden başlat" komutu gönderir (uzaktan reset için).
- **Basınç bilgisi tek yönlüdür:** Downside kendi basınç değerlerini Upside'a göndermez; site hattında zaten kendi sensörü vardır.
- Downside, Upside'dan 20 saniye boyunca veri alamazsa "uzak cihaz yok" kabul eder ve **zamanlayıcı moda** geçer (hidrant 2 dk aç / 2 dk kapat).

> Birbirini göremeyen iki telefon gibi: biri diğerinin ölçtüğü yangın basıncını görüp ona göre davranır, bağlantı koparsa kendi başına karar verir.

---

## 5. DOWNSIDE (Alt İstasyon) — Ne yapar?

### 5.1 Kuyu pompası — yangın tankını (5 ton) doldurur
Downside'da iki tank vardır: **site suyu tankı 10 ton** ve **yangın tankı 5 ton**.

- Kuyu pompası **yangın tankını (5 ton)** doldurur.
- Site tankını (10 ton) kuyu **doldurmaz** — onu **şehir şebekesi (belediye suyu)** doldurur.
- Kuyu, yangın tankının **üst seviye sensörü ıslandığında** durur — kod bu mantığı `!fireUst` koşuluyla yapar.
- **Güvenlik eşikleri:** Yangın tankı alt seviyesi kurursa (`fireAlt` kuru) hidrant pompası çalışmaz; site tankı alt seviyesi kurursa (`siteAlt` kuru) site pompaları çalışmaz.
- Kuyuda su yoksa, faz yoksa veya kuyu başlatma (`kuyu_start`) kapalıysa pompa çalışmaz.

### 5.2 Site basıncı — şebeke birincil, pompa destek
- Siteyi **şehir şebeke suyu** besler. Şebeke aynı zamanda **her iki istasyondaki site tanklarını da doldurur** (Downside'daki 10 tonluk tank + Upside'daki Tank 1).
- **Kritik mantık:** Şebeke basıncı yeterliyse (site basıncını istenen seviyeye çıkarabiliyorsa) **hem Upside hem Downside'daki site besleme motorları çalışmaya ihtiyaç duymaz** — hepsi boştadır.
- Downside'ın **kendi site sensörü** (4-20mA) aynı site hattına bakar; limitleri **alt 7.0 bar / üst 9.0 bar**'dır (60 m kot ≈ 6 bar dahil).
- Basınç **7.0 bar altına** düşerse (şebeke yetişemiyor demektir) site pompası çalışır, **9.0 bar**a ulaşırsa durur.
- **Upside ile tamamlayıcı çalışma:** Normalde site basıncını Upside'daki site pompası üstten tutar. Tüketim yüksek olur, Upside tek başına yetişemez ve basınç düşerse, Downside kendi alt sensöründe 7.0 bar altını görür ve **site pompası otomatik devreye girerek hattı alttan destekler**. Basınç toparlayınca (9.0 bar) yine otomatik durur.
- **Öncelik:** Sürücü (inverter) pompası sağlamsa o çalışır; bozuksa **yedek pompa** devreye girer. (Birisi bozulsa diğeri bakar.)
- (Downside'ın diğer sensörü `line_bar`, **belediyeden gelen şebeke suyunun basıncını** ölçer; yalnızca ekranda gösterilir, kontrol bu değerle yapılmaz. Bu şebeke suyu site hattını besler ve site tanklarını doldurur.)

### 5.3 Hidrant pompası — suyu yukarı (yangın hattına) basar
- **"Yukarı su basmak" demek hidrant motorunun çalışması demektir.** Hidrant çalışınca suyu yukarı, **yangın hattına** basar.
- Upside'dan UDP ile gelen **yangın basıncı 2.0 bar altına** düşerse hidrant pompası çalışır, **2.5 bar** üstüne çıkarsa durur (Upside'daki yangın pompalarıyla aynı mantık, aynı değer).
- **Dolum bağlantısı:** Eğer üst yangın tankları **dolum aşamasına geçmişse** (Upside'da `oto_dolum` aktif → dolum valfi açık), hidrantın yukarı bastığı su otomatik olarak **yangın tanklarının dolumuna** gider (tanklar dolar).
- Upside ile bağlantı koparsa **zamanlayıcı moduna** geçer: 2 dk çalış, 2 dk bekle (yangın hattını asla tamamen bırakmaz).
- Hidrant, yangın tankından beslenir: tank **alt seviyenin altına** inerse (`fireAlt` kuru) güvenlik gereği çalışmaz.

### 5.4 Güvenlik katmanı (MCP röle kartı)
- Tüm çıkışlar her saniye **geri okunur** (istediğimiz = gerçek mi?).
- Çıkış takılırsa (FROZEN) önce kart **otomatik onarılır**; düzelmezse cihaz kendini yeniden başlatır ve alarm verir.
- Elektrik kesintisi sonrası **tüm çıkışlar "hepsi kapalı"** güvenli durumdan başlar.

---

## 6. UPSIDE (Üst İstasyon) — Ne yapar?

### 6.1 Site pompası (sürücülü, kademeli hız) — hattı üstten tutar
- Siteyi **şehir şebeke suyu** besler (hem hattı hem Upside'daki site tankını). Şebeke basıncı yeterliyse site pompası **boştadır**.
- Basınç düşerse kendi üst sensörüyle basıncı **2.3–3.0 bar** aralığında tutar.
- Basınç düşerse frekansı **35 → 50 Hz** kademe kademe arttırır (0.5 Hz adım), yükselirse azaltır. **Ani aç/kapa yok, su darbesi yok.**
- Kullanma suyu tankı %30'un altındaysa site pompası durur (hava basmaz).
- Bu pompa normalde hattı tutan "birincil" güçtür; yetersiz kalırsa Downside'ın site pompası alttan destek verir (bkz. 5.2).

### 6.2 Yangın pompaları (sürücülü) — küçük ve büyük
Upside'da **iki yangın pompası** vardır:
- **Küçük yangın pompası** (sürücü 7, `fire_start`): açık olduğu sürece yangın hattını **sürekli olarak 2.3–3.0 bar (fire_min/fire_max) aralığında tutmaya çalışır**; hız 35–50 Hz kademeli.
- **Büyük yangın pompası** (sürücü 8, `fire1_start`): butonu **bulutta** yer alır; açılması **gerçek yangın** anlamına gelir ve büyük pompa da devreye girer (25–50 Hz kademeli).
- İkisi de aynı basınç sensörünü (`yangin_basinc`) ve aynı `fire_min/fire_max` değerlerini kullanır.
- Ölçülen değer aynı anda UDP ile Downside'a gönderilir; Downside hidrant pompası da alttan destek verir.
- Yangın tankları ortalaması **%15'in altındaysa** pompa tank korumasına girer (kuru çalışma yok).

### 6.3 Tank seviyeleri + otomatik dolum valfi
- 5 tank ultrasonik ile ölçülür: **Tank 1 = kullanma suyu**, **Tank 2-5 = yangın suyu**.
- **Normal modda:** Yangın tankları %95'in altına düşerse dolum valfi açılır, **%99.5'e** ulaşınca kapanır (sürekli dolu tutar).
- **Yangın modunda:** Tanklar **%15'in altına** düşerse valf acil açılır, **%60** olunca kapanır.
- Dolum yapılırken yangın pompası uyutulur (aynı anda hem dolum hem çekme yok).
- Dolum valfi açıkken, Downside'daki **hidrant pompası** suyu yukarı basar; bu su yangın tanklarının dolumuna gider.
- Dolum başladığında/bittiğinde **buluta "Otomatik dolum basladi" / "Otomatik dolum bitti" mesajı** yazılır.

### 6.4 RS485 / Modbus sürücü yönetimi
- Pompaların frekansı, akımı ve hata durumu Modbus ile okunur.
- Sürücü hataya düşerse **otomatik reset** atılır ve pompa kontrollü yeniden başlatılır.

---

## 7. Normal günlük çalışma (senaryo)

1. Şebeke suyu siteyi besliyor; **basınç yeterliyse site pompaları boşta**.
2. Tüketim artıyor, şebeke basıncı yetişemiyor → basınç düşüyor → **Upside site pompası** devreye giriyor, basıncı üstten tutuyor.
3. Tüketim daha da artıyor, Upside tek başına yetişemiyor → **Downside site pompası** da otomatik devreye giriyor, alttan destek veriyor.
4. Tüketim azalıyor → basınç limitlere ulaşınca pompalar sırayla kapanıyor, şebeke tekrar yeterli geliyor.
5. Yangın tankı azalınca **dolum valfi** açılıyor; hidrantın yukarı bastığı su tanklara doluyor, tanklar dolunca valf kapanıyor (buluta mesaj gidiyor).
6. Cihazlar her saniye birbirine "yangın basıncı = X bar" diyor.
7. Her şey normal → ekranda ve bulutta durum izleniyor, kimse bir şey yapmıyor.

---

## 8. GERÇEK YANGIN ANINDA NE OLUR?

### Otomatik olarak (kimse dokunmadan):
1. Hidrant açılır → **yangın hattı basıncı düşer** → Upside'ın tek yangın sensörü bunu ölçer.
2. **Küçük yangın pompası** (`fire_start` açık) kademeli olarak tam güce çıkar; **gerçek yangın için buluttan `fire1_start` açıldıysa büyük yangın pompası** da devreye girer.
3. Upside aynı değeri UDP ile Downside'a iletir → **Downside hidrant pompası** da otomatik devreye girer (2.0 bar altı) ve hattı alttan destekler.
4. Yangın tankı biterse **dolum valfi acil açılır** (yangın modu, %15 eşiği); hidrantın bastığı su doğrudan tanklara dolar.
5. Yangın söner, hidrant kapanır → basınç 2.5 bar üstüne çıkar → pompalar kendiliğinden durur.

### Operatörün yapması gerekenler (checklist):
- [ ] Küçük yangın pompası için **`fire_start` açık mı?** (Sürekli basınç tutar; ekrandan/buluttan "Yangin pompa calistirildi" mesajı görülmeli.)
- [ ] **Gerçek yangında buluttan `fire1_start` aç** → büyük yangın pompası da devreye girer.
- [ ] Hidrant hattı için **`hidrant_start`** açık mı?
- [ ] Yangın tankı seviyelerini kontrol et (ekranda %).
- [ ] Yangın basıncı Upside ekranında 2.3–3.0 bar civarında mı yükseliyor? (Bu değer Downside'a da UDP ile gidiyor.)
- [ ] Dolum valfi açık mı? (Mesaj: "Otomatik dolum basladi")
- [ ] Faz ve kuyu suyu var mı? (Yoksa alarm gelir.)
- [ ] Yangın söndükten sonra **`fire_start` ve `fire1_start` kapat**, tankları normal modda doldur, basınç değerlerini doğrula.

> Sistem ne kadar akıllı olursa olsun, yangın anında **basıncı ekrandan takip edin**; düşmüyorsa hat dolu demektir. Sürücü hatası görünürse sistem zaten otomatik reset atar, tekrar deneyin.

---

## 9. Kontrol gücü ve özellikler özeti

| Özellik | UPSIDE (üst) | DOWNSIDE (alt) |
|---------|--------------|----------------|
| Konum | Tanklar + hattın üst ucu | Kuyu + hattın alt ucu |
| Site basınç sensörü | Kendi üst sensörü (2.3–3.0 bar) | Kendi alt sensörü (7–9 bar, kot dahil) |
| Yangın basınç sensörü | **Tek sensör burda** → UDP ile paylaşır | Sensörü yok; UDP'den alır |
| Site pompası | 1 sürücülü (35–50 Hz, kademeli) | 2 röleli (inverter + yedek, aç/kapa) |
| Site kaynağı | **Şebeke birincil**; basınç yetmezse pompa destek | **Şebeke birincil**; basınç yetmezse pompa destek |
| Yangın desteği | Küçük pompa (fire_start, sürekli basınç) + büyük pompa (fire1_start, gerçek yangın) + acil dolum | 1 hidrant pompası + zamanlayıcı yedek |
| Tank izleme | 5 ultrasonik + tonaj hesaplama | Site tankı 10 ton (şebekeyle dolar, alt sensör) + yangın tankı 5 ton (kuyuyla dolar, alt/üst sensör) |
| Otomatik dolum | Valf ile (mesaj + alarm) | Kuyu ile yangın tankı (5 ton); hidrant ile üst yangın tankları (valf açıkken) |
| Sürücü yönetimi | RS485 Modbus + otomatik reset | — |
| Çıkış güvenliği | — | Geri okuma + otomatik onarım + reset |
| Ekran | Nextion dokunmatik | Nextion dokunmatik |
| Bulut | Arduino IoT Cloud | Arduino IoT Cloud |
| Cihazlar arası | UDP 1 sn paket (yangın basıncı gönderir) | UDP 1 sn paket (reset komutu gönderir) |

---

## 10. Güvenlik katmanları (hiyerarşi)

1. **Faz yoksa / kuyuda su yoksa → pompalar hiç çalışmaz.**
2. **Tank koruması:** Tank boşken pompa asla kuru çalışmaz (Upside: site %30 / yangın %15; Downside: site tankı alt seviyesi kuruyunca site pompaları, yangın tankı alt seviyesi kuruyunca hidrant durur).
3. **MCP geri okuma:** Röle takıldıysa 10 sn içinde tespit + otomatik onarım; onarılamazsa sistem reset + alarm.
4. **UDP yoksa:** Taraflar bağımsız karar verir (hidrant zamanlayıcı modu gibi).
5. **Uzaktan reset:** Downside, Upside'ı buluttan yeniden başlatabilir.
6. **Güvenli açılış:** Elektrik gelince tüm çıkışlar kapalı başlar, sonra kontrollü devreye alınır.

---

## 11. İzleme ve mesajlar (bulut)

- **`isLive`** değişkeni 3 saniyede bir güncellenir → dashboard'da cihazın "çevrimiçi" olduğu görünür.
- **`messages`** değişkeni (Upside) önemli olayları yazar:
  - "Otomatik dolum basladi" / "Otomatik dolum bitti"
  - "Site/Yangin pompa calistirildi/durduruldu (ekran)"
  - "Gecersiz site/yangin basinc degeri!" (yanlış değer girilirse)
  - "Sifirlandi" (sürücü reset mesajları)
- **Uyarı:** `isLive`'ın her 3 saniyede bir gönderilmesi bulut kotasını kullanır. Ücretli (Maker) planda bu güvenlidir (~864.000 kayıt/ay, limit 1 milyon/gün). **Değer 3 saniyede bırakıldı.**

---

## 12. Önemli teknik notlar

- **UDP paketleri "paketlenmiş" (packed) DEĞİLDİR:** Her iki cihaz aynı ESP32 derleyicisini kullandığı için hizalama aynıdır (8 ve 12 bayt). `packed` yapılırsa paketler birbirini reddeder, haberleşme kesilir.
- **Basınç eşleşmesi:** Downside 7–9 bar, Upside 2.3–3.0 bar'dır — aynı hattın alt ve üst ucu. Kot (≈6 bar) eklenince fiziksel olarak aynı durumu anlatırlar.
- **Yangın hattının tek sensörü:** Downside'ın hidrant kararı UDP'den gelen Upside değeriyle verilir; kendi `line_bar` sensörü (belediye giriş suyu basıncı) yalnızca ekrandadır.
- **Cloud gecikmesi:** Bulut bağlantısı açılışta 3 saniye sonra başlar; `ArduinoCloud.update()` her döngüde çağrılır (mesajlar gecikmez).
- **Kod referansları:**
  - Downside: `downside_son_durum_fixed.txt` (düzeltilmiş son sürüm)
  - Upside: `upside_with_nex.txt` (ekranlı son sürüm)

---

## 13. Önerilen sunum akışı (5 dk)

1. "Bu sistem ne yapar?" (bölüm 1)
2. **Kot farkı: neden iki farklı basınç limiti?** (bölüm 2) — sunumun kilit noktası
3. Canlı akış şeması: kuyu → tank → site/yangın hattı, iki pompanın aynı hatta ortak çalışması (bölüm 3-7)
4. Yangın senaryosu: tek sensör, UDP paylaşımı, iki pompa desteği (bölüm 8)
5. Güvenlik katmanları 6 maddede (bölüm 10)
6. Soru-cevap: tank, basınç, mesajlar, bulut (bölüm 9-11)

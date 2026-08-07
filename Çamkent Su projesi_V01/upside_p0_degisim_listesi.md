# upside_hmi.HMI — page0 Analog Gösterge Paneli (Yeni Tasarım)

Bu liste, `upside_hmi.HMI` dosyasının **page0** sayfasını yeni **analog gösterge paneline**
dönüştürmek ve Arduino kodundaki (`upside_with_nex.txt`) küçük ID uyumsuzluklarını düzeltmek içindir.

Arka plan görseli: **`nextion-upside-p0.png`** (800x480, bu paketle birlikte üretildi).

---

## 0. Tasarım Özeti

Kontrol-odası stili: üstte başlık + 3 durum LED'i, ortada **5 tank silüeti**, altında
**iki büyük dairesel gösterge** (site / yangın basıncı), en altta **kumanda butonları +
durum satırı**. Eski akış diyagramı (borular/oklar) tamamen kalkıyor.

```
┌────────────────────────────────────────────────────────────────────────────┐
│ ÇAMKENT SU KONTROL SİSTEMİ     [ HAT ● ] [ SİTE ● ] [ YANGIN ● ]          │
├────────────────────────────────────────────────────────────────────────────┤
│  SİTE SUYU   YANGIN 1    YANGIN 2    YANGIN 3    YANGIN 4                  │
│  ┌───┐       ┌───┐        ┌───┐        ┌───┐        ┌───┐                 │
│  │▓▓▓│       │▓▓▓│        │▓▓▓│        │▓▓│         │▓░│                  │
│  └───┘       └───┘        └───┘        └───┘        └───┘                 │
│  67.3%-12.4t 45.1%-8.3t   38.2%-7.0t   22.0%-4.0t   11.5%-2.1t            │
├─────────────────────────────────────┬──────────────────────────────────────┤
│  SİTE BASINCI (gösterge)            │  YANGIN BASINCI (gösterge)           │
│         ╭──────────╮                │         ╭──────────╮                 │
│         │   12.3   │                │         │   9.8    │                 │
│         │  BAR     │                │         │  BAR     │                 │
│         ╰──────────╯                │         ╰──────────╯                 │
├─────────────────────────────────────┴──────────────────────────────────────┤
│ [SİTE START]  [YANGIN (FİRE)]      DURUM: (t0)              [Detay >]     │
└────────────────────────────────────────────────────────────────────────────┘
```

> Göstergelerin ortasındaki sayı, canlı XFloat bileşenidir (x0/x1). Gösterge kadranı
> (renk bölgeleri, tik işaretleri, BAR etiketi) arka plan resminde çizilmiştir.
> İstenirse ileride Nextion'ın **Gauge** bileşeniyle hareketli ibre eklenebilir (bunun için
> yeni bileşen + kod satırı gerekir, ID doğrulaması yapılır — şimdilik eklenmedi).

---

## 1. Kritik Bilgi: Kod ID İle Erişiyor

NextionX2 kütüphanesi komutları `p[0].b[57].val=26` biçiminde **ID numarasıyla** gönderir.
Bu yüzden **bileşenler SİLİNMEZ** (silinirse sonraki ID'ler kayar); kullanılmayanlar ekran
dışına taşınır. Böylece tüm ID'ler sabit kalır.

Mevcut HMI ile kodun page0 uyumsuzlukları (kod satırı → düzeltme):

| Satır | Kod | HMI'da olan | Düzeltme |
|---|---|---|---|
| 73 | `siteP(0,57)` | 57 = Text (sayı gösterilmiyor) | `siteP(0,56)` → **x0** XFloat |
| 74 | `fireP(0,60)` | 60 = ProgressBar (çubuk doluyor) | `fireP(0,59)` → **x1** XFloat |
| 49 | `fire_pump_led(0,46)` | 46 = çubuk | `(0,45)` → **fire_pump** radyosu |
| 48 | `site_pump_led(0,39)` | 39 = line_pump (swap) | `(0,33)` → **site_pump** radyosu |
| 47 | `line_ledpump_led(0,33)` | 33 = site_pump (swap) | `(0,39)` → **line_pump** radyosu |

Detaylar Bölüm 5'te.

---

## 2. Arka Plan Resmi

1. `nextion-upside-p0.png` dosyasını Nextion Editör'de **Resimler** sekmesine içe aktar.
2. Sayfaya **Picture** bileşeni ekle: `x=0, y=0, w=800, h=480`, resim = bu PNG.
3. Resme **sağ tık → En Arkaya Taşı** (tüm bileşenlerin altına).

Resim şunları içerir: başlık yazısı, üst bant, LED çipleri (HAT/SİTE/YANGIN + daire),
tank silüetleri (kapak + gövde + taban), iki gösterge kadranı (renk bölgeleri + tikler + BAR),
alt ayırıcı çizgi, "DURUM" etiketi, dış çerçeve. Bileşen metinleri (tank adları, seviyeler,
gösterge başlıkları, buton yazıları) resimde YOKTUR — onlar canlı bileşenlerdir.

---

## 3. TAŞINACAK / DÜZENLENECEK BİLEŞENLER

Mevcut x/y/w/h değerlerinin yerine aşağıdakiler yazılacak. Hiçbir şey silinmez.

### Başlık — LED'ler (çip dairelerinin tam ortasına)
| Bileşen (ID) | x | y | w | h | Açıklama |
|---|---|---|---|---|---|
| **line_pump (39)** | 462 | 17 | 20 | 20 | HAT çipi, daire merkezi (472,27) |
| **site_pump (33)** | 577 | 17 | 20 | 20 | SİTE çipi, daire merkezi (587,27) |
| **fire_pump (45)** | 692 | 17 | 20 | 20 | YANGIN çipi, daire merkezi (702,27) |

> Bu radyoların `State=pressed` hali kalır; kod `bco/pco` ile renk verir (çalışıyor=yeşil, durdu=kırmızı).

### Tank Silüetleri (üst sıra) — kolon başlangıcı her 160 px'te bir
| Bileşen (ID) | x | y | w | h | Açıklama |
|---|---|---|---|---|---|
| **t1 (8)** | 8 | 58 | 144 | 22 | Metin: **"SİTE SUYU"**, Font 1, orta |
| **tank1 (7)** | 8 | 84 | 144 | 96 | **dikey** bar (doluluk), bco=0, pco koddan |
| **level1 (13)** | 8 | 184 | 144 | 16 | Font 1, orta; `"67.3% - 12.40 ton"` |
| **t2 (9)** | 168 | 58 | 144 | 22 | Metin: **"YANGIN 1"** |
| **tank2 (3)** | 168 | 84 | 144 | 96 | dikey bar |
| **level2 (14)** | 168 | 184 | 144 | 16 | |
| **t3 (10)** | 328 | 58 | 144 | 22 | Metin: **"YANGIN 2"** |
| **tank3 (4)** | 328 | 84 | 144 | 96 | dikey bar |
| **level3 (15)** | 328 | 184 | 144 | 16 | |
| **t4 (11)** | 488 | 58 | 144 | 22 | Metin: **"YANGIN 3"** |
| **tank4 (5)** | 488 | 84 | 144 | 96 | dikey bar |
| **level4 (16)** | 488 | 184 | 144 | 16 | |
| **t5 (12)** | 648 | 58 | 144 | 22 | Metin: **"YANGIN 4"** |
| **tank5 (6)** | 648 | 84 | 144 | 96 | dikey bar |
| **level5 (17)** | 648 | 184 | 144 | 16 | |

> Tank gövdesi resimde çizili; bar, gövdenin içine oturur ve aşağıdan yukarı dolar.

### Göstergeler (orta band)
| Bileşen (ID) | x | y | w | h | Açıklama |
|---|---|---|---|---|---|
| **t11 (57)** | 8 | 210 | 380 | 24 | Metin: **"SİTE BASINCI"**, Font 2, orta |
| **x0 (56)** | 128 | 272 | 140 | 60 | **Basınç değeri** (site). Font 4, orta, `bco=0`, `pco=65535`, sol hane 3 / sağ hane 1. Dokunma → page 2 **kalır** |
| **t12 (58)** | 408 | 210 | 380 | 24 | Metin: **"YANGIN BASINCI"**, Font 2, orta |
| **x1 (59)** | 528 | 272 | 140 | 60 | **Basınç değeri** (yangın). Font 4, orta. Dokunma → page 3 **kalır** |

> Kadran merkezi site için (198,302), yangın için (598,302) — XFloat tam ortaya oturur.
> Göstergeye dokunmak ayar sayfasını açar (x0→page2, x1→page3).

### Kumanda + Durum (alt bant)
| Bileşen (ID) | x | y | w | h | Açıklama |
|---|---|---|---|---|---|
| **bt0 (27)** | 8 | 414 | 170 | 56 | "SİTE START" dual-state. Olaylar aynen |
| **bt1 (34)** | 186 | 414 | 170 | 56 | "YANGIN (FİRE)" dual-state. Olaylar + `yangin` zamanlayıcı **dokunulmaz** |
| **t0 (1)** | 368 | 418 | 320 | 48 | Durum metni. Font 2, orta, `bco=0`, `pco=65535` |
| **b0 (55)** | 700 | 420 | 90 | 44 | Metin: **"Detay >"**. Olay aynen (Touch Release → page 1) |

---

## 4. EKRAN DIŞINA TAŞINACAKLAR (Gizlenecekler)

`x=1000, y=1000` yaz → ekran dışında kalır, ID sabit. **SİLME.**

- Resimler: **p0 (2), p1 (28), p2 (30), p3 (35), p4 (44)**
- Eski başlık/akış etiketleri: **t8 (40)**, **t6 (32), t9 (41), t10 (53), t13 (63)**
  > t8 başlığı resimde çizili olduğu için gizlenir.
- Eski akış çubukları: **j0 (18), j3 (19), j1 (20), j4 (21), j5 (22), j6 (23), j7 (24),
  j2 (25), j8 (26), j9 (31), j10 (36), j11 (37), j12 (38), j13 (42), j14 (43), j15 (46),
  j16 (47), j17 (48), j18 (49), j19 (50), j20 (51), j21 (52), j22 (60), j23 (61), j24 (62),
  j25 (64)**
- Artık kullanılmayan radyolar: **r0 (29), r1 (65)**

Dokunulmayacaklar (çalışmaya devam ediyor): **yangin (54)** zamanlayıcı.

---

## 5. KOD DEĞİŞİKLİKLERİ — `upside_with_nex.txt`

| Satır | Mevcut | Yeni | Neden |
|---|---|---|---|
| 47 | `line_ledpump_led(Ekran, 0, 33)` | `line_ledpump_led(Ekran, 0, 39)` | 39 = line_pump radyosu |
| 48 | `site_pump_led(Ekran, 0, 39)` | `site_pump_led(Ekran, 0, 33)` | 33 = site_pump → **LED swap düzeltmesi** |
| 49 | `fire_pump_led(Ekran, 0, 46)` | `fire_pump_led(Ekran, 0, 45)` | 45 = fire_pump radyosu |
| 73 | `siteP(Ekran, 0, 57)` | `siteP(Ekran, 0, 56)` | 56 = x0 → **site basıncı görünür** |
| 74 | `fireP(Ekran, 0, 60)` | `fireP(Ekran, 0, 59)` | 59 = x1 → **yangın basıncı görünür** |

İsteğe bağlı:
- Satır 66 `firetimer(Ekran, 0, 55)` kullanılmıyor → silinebilir.
- Seviye metni 144 px'te taşarsa (font 1) level fontunu 0'a düşür veya kodu
  `"%.1f%%"` olarak kısalt (tonaj düşer).
- `line_ledpump_led` kodda hiç sürülmüyor; istersen inverter 8 `running` bilgisiyle
  aynı `bco/pco` satırını ekle (HAT LED'i de yanar).

> page1/2/3 bileşenleri kod ile birebir uyumlu — değişmiyor.

---

## 6. DOĞRULAMA TABLOSU

Değişikliklerden sonra şu ID'ler değişmemiş olmalı (hiçbir bileşen silinmediği için değişmez).
`.hmi` dosyasını metin editörüyle açıp `"id":` alanından kontrol edebilirsin.

| Bileşen | ID | | Bileşen | ID |
|---|---|---|---|---|
| t0 | 1 | | tank2 | 3 |
| tank3 | 4 | | tank4 | 5 |
| tank5 | 6 | | tank1 | 7 |
| t1 | 8 | | level1 | 13 |
| level2 | 14 | | level3 | 15 |
| level4 | 16 | | level5 | 17 |
| bt0 | 27 | | site_pump | 33 |
| bt1 | 34 | | line_pump | 39 |
| fire_pump | 45 | | yangin | 54 |
| b0 | 55 | | **x0** | **56** |
| t11 | 57 | | t12 | 58 |
| **x1** | **59** | | | |

`x0`=56 ve `x1`=59 özellikle önemli (basınç değerleri bu ID'lerle kodlanıyor).

---

## 7. Sayfalar 1/2/3

Değişiklik **yok**. Kodla birebir eşleşiyor. Görsel olarak da yeni temaya uygun
yeniden kurmak istersen ayrıca liste çıkarılır.

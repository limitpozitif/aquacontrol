# Nextion Editor Kurulum Rehberi — 7.5" (800x480) Ekran

Bu paket, tasarlanan **DOWNSIDE** ve **UPSIDE** ekran görüntülerini Nextion
Editor'de hızlıca kurmanızı sağlar. .hmi dosyaları binary olduğu için elle
üretilemez; editörün desteklediği yol **arka plan PNG'si + bileşen yerleşimi**
yöntemidir. Bileşen adları kodunuzla birebir aynıdır, Arduino kodunda değişiklik
gerekmez.

## İçindekiler

1. [Dosyalar](#1-dosyalar)
2. [Genel kurulum adımları](#2-genel-kurulum-adımları)
3. [Renkler ve fontlar](#3-renkler-ve-fontlar)
4. [DOWNSIDE sayfa 0 (ana ekran)](#4-downside-sayfa-0-ana-ekran)
5. [DOWNSIDE sayfa 1 (ayarlar)](#5-downside-sayfa-1-ayarlar)
6. [UPSIDE sayfa 0 (ana ekran)](#6-upside-sayfa-0-ana-ekran)
7. [UPSIDE sayfa 1/2/3 (inverter + ayarlar)](#7-upside-sayfa-123-inverter--ayarlar)
8. [Buton olay kodları (downside)](#8-buton-olay-kodları-downside)

---

## 1. Dosyalar

| Dosya | Açıklama |
|---|---|
| `nextion-downside-p0.png` | DOWNSIDE ana ekran arka planı (800x480) |
| `nextion-upside-p0.png` | UPSIDE ana ekran arka planı (800x480) |
| `downside-layer.png` / `upside-layer.png` | Dolu görünümlü mockup (sunum/kıyaslama) |
| `site-yangin-kontrol-sistemi-sunum-raporu.md` | Sistem raporu |

> Temiz arka planlarda kesikli çerçeveler "bileşenin buraya geleceği" alanları
> gösterir. Bileşeni üzerine yerleştirdiğinizde kesikli kutu görünmez (bileşen
> kapatır).

---

## 2. Genel kurulum adımları

1. **Proje aç / yeni:** Nextion Editor'de `File > New`, cihaz modelini **7.0
   (800x480)** seçin (7.5" panel 800x480 çözünürlüktedir; model adı
   `NX8048P070...` benzeri olabilir).
2. **Görseli içe aktar:** Üst araç çubuğundaki **Resim (Picture)** aracını açın →
   `Add` → `nextion-downside-p0.png` seçin. Görsel resim listesine eklenir.
3. **Sayfa arka planı yap:** Araç kutusundan **Picture** bileşenini sayfaya
   sürükleyin, özniteliklerde:
   - `obj name`: `pic0` (isterseniz `downside_bg`)
   - `x`: 0, `y`: 0, `w`: 800, `h`: 480
   - `pic`: içe aktardığınız görsel
   Bu bileşen sayfanın arka planı olur.
4. **Bileşenleri ekle:** Aşağıdaki tablolardaki her bileşeni araç kutusundan
   ilgili türle sürükleyin, **obj name**, **x**, **y**, **w**, **h** ve renk
   özniteliklerini birebir girin.
5. **Buton olaylarını yaz:** Bölüm 8'deki kodu ilgili butonun
   `Touch Release Event` penceresine yapıştırın.
6. **Derle & yükle:** `Compile` ile `.tft` üretin, FAT32 microSD ile ekrana
   yükleyin. Görüntü 9600 baud, `SERIAL_8N1` ile ESP32'ye bağlı kalır.

> **Türkçe karakter:** "İ", "Ş", "Ü" gibi karakterlerin görünmesi için
> bileşenlerde kullanacağınız fontu **UTF-8** kodlamasıyla üretin
> (Font Creator'da encoding = UTF-8). ASCII fontla bu harfler bozuk çıkar.

---

## 3. Renkler ve fontlar

Kodda kullanılan RGB565 (ondalık) değerleri:

| Renk | Değer | Kullanım |
|---|---|---|
| Yeşil | 2016 | OK / çalışıyor |
| Kırmızı | 63488 | Arıza / alarm |
| Sarı | 65504 | Uyarı |
| Koyu yeşil | 1024 | Normal bar dolgu rengi (kod `pco=1024` yazar) |
| Gri/mavi (bar arka planı) | 2212 | `#0A1526` panel koyusu |
| Siyah | 0 | Arka plan koyusu |

Önerilen başlangıç öznitelikleri:

- **ProgressBar:** `pco=1024`, `bco=2212`, `dez=0` (yatay)
- **Text LED:** `pco=2016`, `bco=0` (kod pco/bco'yu kendisi değiştirir)
- **Button:** `bco=0`, `bco2=63488`, `pco=65535`
- Font önerisi: değerler için `Consolas`-tipi tek aralıklı font 16–28 px;
  etiketler için 11–13 px.

---

## 4. DOWNSIDE sayfa 0 (ana ekran)

Arka plan: `nextion-downside-p0.png`

| Bileşen | Tür | x | y | w | h | Not |
|---|---|---|---|---|---|---|
| `pic0` | Picture | 0 | 0 | 800 | 480 | Arka plan |
| `cfaz_ok` | Text | 353 | 18 | 96 | 28 | "FAZ OK" |
| `cinv_ok` | Text | 457 | 18 | 96 | 28 | "İNV OK" |
| `csu_var` | Text | 561 | 18 | 96 | 28 | "KUYU SU" |
| `bt0` | Button | 665 | 18 | 120 | 28 | START/STOP (metni kod yazar) |
| `xsite_bar` | ProgressBar | 17 | 99 | 240 | 36 | Site basıncı 0-100 |
| `xline_bar` | ProgressBar | 281 | 99 | 240 | 36 | Şebeke basıncı 0-100 |
| `xfire_bar` | ProgressBar | 545 | 99 | 240 | 36 | Yangın basıncı 0-100 |
| `jfire_tank` | ProgressBar | 17 | 245 | 372 | 34 | Yangın tankı 0/50/100 |
| `cfired_sens` | Text | 17 | 285 | 130 | 24 | Alt sensör |
| `cfireu_sens` | Text | 155 | 285 | 130 | 24 | Üst sensör |
| `jsite_tank` | ProgressBar | 413 | 245 | 372 | 34 | Site tankı 0/100 |
| `cs_sens` | Text | 413 | 285 | 130 | 24 | Site alt seviye |
| `csite_inv` | Text | 17 | 355 | 174 | 32 | Site·İnv durumu |
| `csite_yedek` | Text | 215 | 355 | 174 | 32 | Site·Yedek durumu |
| `cksu` | Text | 413 | 355 | 174 | 32 | Kuyu durumu |
| `chidr` | Text | 611 | 355 | 174 | 32 | Hidrant durumu |
| `tstatus` | Text | 8 | 410 | 784 | 26 | Alarm/durum satırı |

İsteğe bağlı (yalnızca ekran göstergesi, kodda yok):

| Bileşen | Tür | x | y | w | h | Not |
|---|---|---|---|---|---|---|
| `ccomm_ok` | Text | 8 | 17 | 80 | 28 | WiFi/iletişim LED'i |
| `jwifi` | ProgressBar | 88 | 17 | 96 | 28 | WiFi sinyali (isteğe bağlı) |

> Basınç değerlerinin **rakamsal** görünmesini istiyorsanız barın üstüne bir
> `Text` ekleyin (ör. `tsite_p`, x=16 y=70 w=240 h=30) ve Arduino'da
> `page0.xsite_bar.val=...` satırının yanına bir satır ekleyin:
> `nextionSend("page0.tsite_p.txt=\"" + String(site_bar,1) + "\"")` (line ~419).

---

## 5. DOWNSIDE sayfa 1 (ayarlar)

Mevcut projenizdeki düzeni koruyun; yalnızca yeni arka plan istiyorsanız aynı
bileşen adlarıyla yeniden kurun:

| Bileşen | Tür | Not |
|---|---|---|
| `x0` | Slider | Site alt limit 0-100 (0.1 bar cinsinden, kod `val/10`) |
| `x1` | Slider | Site üst limit 0-100 |
| `h0` | Slider | Site alt limit (görsel kopya, kod `h0.val` yazar) |
| `h1` | Slider | Site üst limit (görsel kopya) |
| `c0` | Button | Kuyu start (toggle) |
| `c1` | Button | Hidrant start (toggle) |

> `x0/x1/h0/h1` Slider ise kod `page1.x0.val=...` ile değerini yazar; Slider
> değerini değiştirip bırakınca Bölüm 8'deki olay koduyla Arduino'ya gider.
> Mevcut projenizde ProgressBar + `printh` kullanıyorsa aynen koruyun.

---

## 6. UPSIDE sayfa 0 (ana ekran) — Analog Gösterge Paneli

Arka plan: `nextion-upside-p0.png` (analog gösterge stili: tank silüetleri + iki dairesel
basınç göstergesi + LED çipleri).

İki yol vardır:

**A) Mevcut `upside_hmi.HMI`'yı dönüştür (önerilen):** `upside_p0_degisim_listesi.md`
belgesindeki adımları uygula. Bileşenler silinmez, taşınır; ID'ler sabit kalır. Kodda
yalnızca 5 satır güncellenir (satır 47-49 ve 73-74, ayrıntılar belgede).

**B) Sıfırdan kur:** Aşağıdaki bileşenleri yeni projeye ekle (adlar kodla birebir):

| Bileşen | Tür | x | y | w | h | Not |
|---|---|---|---|---|---|---|
| `pic0` | Picture | 0 | 0 | 800 | 480 | Arka plan |
| `line_pump` | Radio | 462 | 17 | 20 | 20 | HAT LED çipi (merkez 472,27) |
| `site_pump` | Radio | 577 | 17 | 20 | 20 | SİTE LED çipi (merkez 587,27) |
| `fire_pump` | Radio | 692 | 17 | 20 | 20 | YANGIN LED çipi (merkez 702,27) |
| `t1` | Text | 8 | 58 | 144 | 22 | "SİTE SUYU" |
| `tank1` | ProgressBar | 8 | 84 | 144 | 96 | dikey, doluluk |
| `level1` | Text | 8 | 184 | 144 | 16 | seviye + ton |
| `t2` | Text | 168 | 58 | 144 | 22 | "YANGIN 1" |
| `tank2` | ProgressBar | 168 | 84 | 144 | 96 | dikey |
| `level2` | Text | 168 | 184 | 144 | 16 | |
| `t3` | Text | 328 | 58 | 144 | 22 | "YANGIN 2" |
| `tank3` | ProgressBar | 328 | 84 | 144 | 96 | dikey |
| `level3` | Text | 328 | 184 | 144 | 16 | |
| `t4` | Text | 488 | 58 | 144 | 22 | "YANGIN 3" |
| `tank4` | ProgressBar | 488 | 84 | 144 | 96 | dikey |
| `level4` | Text | 488 | 184 | 144 | 16 | |
| `t5` | Text | 648 | 58 | 144 | 22 | "YANGIN 4" |
| `tank5` | ProgressBar | 648 | 84 | 144 | 96 | dikey |
| `level5` | Text | 648 | 184 | 144 | 16 | |
| `siteP` | XFloat | 128 | 272 | 140 | 60 | Site basıncı (x10), Font 4, bco=0 |
| `fireP` | XFloat | 528 | 272 | 140 | 60 | Yangın basıncı (x10), Font 4, bco=0 |
| `siteButton` | Dual-state Button | 8 | 414 | 170 | 56 | SİTE START |
| `fireButton` | Dual-state Button | 186 | 414 | 170 | 56 | YANGIN (FİRE) |
| `t0` | Text | 368 | 418 | 320 | 48 | Durum satırı, Font 2, bco=0 |
| `b0` | Button | 700 | 420 | 90 | 44 | "Detay >" → page 1 |

> Sıfırdan kurarken ID'ler editör tarafından atanır; `upside_with_nex.txt` içindeki
> `(Ekran, sayfa, ID)` satırlarını **kurulum sonrası doğrulayıp** güncelle
> (NextionX2 ID ile erişir). A yolunda ID'ler zaten sabittir.

> UPSIDE butonları `NextionX2` kütüphanesini kullanır: butonların **Touch Press ve Touch
> Release** sekmelerindeki **"Send Component ID"** kutusunu **işaretleyin** — kütüphane
> 0x65 çerçevelerini bu sayede alır. (`fireButton.release(firebtn)` ve
> `siteButton.release(sitebtn)` kodda zaten kayıtlı.)

---

## 7. UPSIDE sayfa 1/2/3 (inverter + ayarlar)

Bunlar mevcut projenizde duruyor; aynı isimlerle koruyun:

**Sayfa 1 — İnverter verileri (tümü Text):**
`siteFault`, `fireFault`, `siteLineVoltage`, `fireLineVoltage`, `siteCurrent`,
`fireCurrent`, `siteOutFreq`, `fireOutFreq`, `siteACC`, `fireACC`, `siteDEC`,
`fireDEC`, `siteMaxFreq`, `fireMaxFreq`, `siteStat`, `fireStat`

**Sayfa 2 — Site basınç ayarları:**
`siteMinP` (Number, 2,7), `siteMaxP` (Number, 2,5), `siteSaveButton` (Button, 2,15)
— Save butonu `Touch Release`'e "Send Component ID" işaretli.

**Sayfa 3 — Yangın basınç ayarları:**
`fireMinP` (Number, 3,6), `fireMaxP` (Number, 3,4), `fireSaveButton` (Button, 3,15)

> Number bileşenleri 0-100 aralığında tam sayıdır (0.1 bar çözünürlük).
> Kod `siteMinP.value()/10` ile bar'a çevirir.

---

## 8. Buton olay kodları (downside)

DOWNSIDE Arduino'su `0x65` (dokunma) çerçevelerini yok sayar; girişler şu
çerçeveyle gelir: `<id> <değer 4 byte LE>`. Nextion tarafında `printh`
(sabit byte) + `prints` (değişkeni LE byte olarak gönderir) kullanılır.

**`bt0` — Start/Stop (touch release):**
```
if(bt0.val==1)
  printh 04 01 00 00 00
else
  printh 04 00 00 00 00
endif
```

**`c0` — Kuyu start (touch release):**
```
if(c0.val==1)
  printh 05 01 00 00 00
else
  printh 05 00 00 00 00
endif
```

**`c1` — Hidrant start (touch release):**
```
if(c1.val==1)
  printh 06 01 00 00 00
else
  printh 06 00 00 00 00
endif
```

**`x0` — Site alt limit (slider/slider release):**
```
printh 01
prints x0.val,4
```

**`x1` — Site üst limit (slider release):**
```
printh 02
prints x1.val,4
```

**Sayfa 1 "onload" (Preinitialize Event) — Arduino'ya sayfanın yüklendiğini
bildirir (kod bu sayede limitleri geri gönderir):**
```
printh 03 00 00 00 00
```

> `prints <değişken>,4` değişkenin en önemsiz 4 baytını little-endian olarak
> gönderir; Arduino `(byte<<8*i)` ile aynen okuyor. Bileşeniniz ProgressBar ise
> slider yerine `val` güncellenemez; bu durumda değer girişi için Slider
> (s) türü kullanın.

---

## Hızlı kontrol listesi

- [ ] Arka plan Picture bileşeni 0,0 / 800x480, `pic` doğru görselde
- [ ] Tüm bileşen adları kodla birebir (büyük/küçük harf dahil)
- [ ] Türkçe font UTF-8
- [ ] ProgressBar aralığı 0-100, `dez=0` (yatay)
- [ ] DOWNSIDE butonlarında `printh` olayları (Bölüm 8)
- [ ] UPSIDE butonlarında "Send Component ID" işaretli
- [ ] Derleme sonrası `.tft` dosyası ekrana yüklenmiş

// index.html (web_panel_data) -> gomulu (PROGMEM) web dosyasi
#pragma once

static const char WEB_INDEX_HTML[] PROGMEM = R"CAMKENTP9("
<!DOCTYPE html>
<html lang="tr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Çamkent Aqua — Kontrol Paneli</title>
<link rel="stylesheet" href="style.css?v=8">
    <script src="app.js?v=12" defer></script>
</head>
<body>

<div class="app">

  <!-- ================= HEADER ================= -->
  <header class="hero">
    <div class="hero-top">
      <div class="logo"><span class="logo-badge">💧</span><span>ÇAMKENT <em>AQUA</em></span></div>
      <div class="clock"><span id="time">--</span><small>CANLI VERİ</small></div>
    </div>
    <div class="hero-sub">Site Suyu &amp; Yangın Hattı Kontrol Sistemi &nbsp;·&nbsp; UPSIDE ⇄ DOWNSIDE &nbsp;·&nbsp; V01</div>
    <div class="hero-users">
      <div class="hero-user"><span>👥 Toplam Kullanıcı</span><strong id="summaryUsers">--</strong></div>
      <div class="hero-user"><span>🟢 Aktif Kullanıcı</span><strong id="summaryActive">--</strong></div>
    </div>
    <div class="status-leds">
      <div class="led"><span class="dot"></span>SİSTEM NORMAL</div>
      <div class="led"><span class="dot"></span>ESP32</div>
      <div class="led"><span class="dot"></span>MODBUS</div>
      <div class="led"><span class="dot"></span>UDP BAĞLANTI</div>
      <div class="led" id="ledSensSite"><span class="dot"></span>SİTE SENS</div>
      <div class="led" id="ledSensFire"><span class="dot"></span>YANGIN SENS</div>
      <div class="led" id="ledSensDownSite"><span class="dot"></span>ALT SİTE SENS</div>
      <div class="led" id="ledSensLine"><span class="dot"></span>ŞEBEKE SENS</div>
    </div>
  </header>

  <!-- ================= SUMMARY ================= -->
  <section class="summary">
    <div class="summary-box"><div>💧 Site Suyu</div><strong id="summarySite">--</strong></div>
    <div class="summary-box"><div>🔥 Yangın Rezervi</div><strong id="summaryFire">--</strong></div>
    <div class="summary-box"><div>⛰ Kot Farkı</div><strong>60 m · ~6 bar</strong></div>
    <div class="summary-box"><div>⚙ Site Pompası</div><strong id="summaryPump">--</strong></div>
  </section>

  <!-- ================= UPSIDE ================= -->
  <section class="station upside">
    <div class="station-head">
      <span class="badge up">▲ UPSIDE</span>
      <h2>Üst İstasyon — Depo Yönetimi</h2>
      <span class="station-tag">5 tank · site + yangın hattı · kot farkı ~60 m</span>
    </div>

    <div class="station-grid">
      <!-- TANKS -->
      <div class="card">
        <h3>Su Tankları</h3>
        <div class="tank-row">
          <div class="tank-lbl"><b>TANK 1</b><small>Kullanma suyu</small></div>
          <div class="tank-track"><div class="tank-fill water" id="tk1Fill"></div></div>
          <div class="tank-val" id="tk1Val">--</div>
          <div class="tank-ton" id="tk1Ton">--</div>
        </div>
        <div class="tank-row">
          <div class="tank-lbl"><b>YANGIN REZERVİ</b><small>4 tank · toplam 20 t</small></div>
          <div class="tank-track"><div class="tank-fill fire" id="tkFireFill"></div></div>
          <div class="tank-val" id="tkFireVal">--</div>
          <div class="tank-ton" id="tkFireTon">--</div>
        </div>
      </div>

      <!-- GAUGES -->
      <div class="card">
        <h3>Hat Basınçları</h3>
        <div class="gauges-row">
          <div class="gauge-wrap">
            <div id="gaugeSite"></div>
            <label>SİTE</label>
          </div>
          <div class="gauge-wrap">
            <div id="gaugeFire"></div>
            <label>YANGIN</label>
          </div>
        </div>
        <div class="gauge-note">YANGIN basıncı her saniye UDP ile DOWNSIDE'a iletilir</div>
      </div>

      <!-- CONTROLS -->
      <div class="card">
        <h3>Kumanda</h3>
        <div class="ctrl on" id="ctrlSitePump">
          <div class="row"><span class="ctrl-name">SİTE POMPASI</span><span class="ctrl-state" id="ctrlSitePumpSt">--</span></div>
          <span class="ctrl-val">sürücülü · 35–50 Hz kademeli</span>
          <div class="meta"><span>Frekans <b id="ctrlSiteFreq">--</b></span><span>Akım <b id="ctrlSiteCur">--</b></span><span>Gerilim <b id="ctrlSiteVolt">--</b></span></div>
        </div>
        <div class="ctrl off" id="ctrlFirePump">
          <div class="row"><span class="ctrl-name">YANGIN POMPASI <small>(küçük)</small></span><span class="ctrl-state" id="ctrlFirePumpSt">--</span></div>
          <span class="ctrl-val">sürücülü · 35–50 Hz kademeli</span>
          <div class="meta"><span>Frekans <b id="ctrlFireFreq">--</b></span><span>Akım <b id="ctrlFireCur">--</b></span><span>Gerilim <b id="ctrlFireVolt">--</b></span></div>
        </div>
        <div class="ctrl warn off" id="ctrlBigFire">
          <div class="row"><span class="ctrl-name">BÜYÜK YANGIN <small>(fire1)</small></span><span class="ctrl-state" id="ctrlBigFireSt">--</span></div>
          <span class="ctrl-val">gerçek yangın · bulut butonu · 25–50 Hz</span>
        </div>
        <div class="ctrl on" id="ctrlFillValve">
          <div class="row"><span class="ctrl-name">DOLUM VALFİ</span><span class="ctrl-state" id="ctrlFillValveSt">--</span></div>
          <span class="ctrl-val" id="ctrlFillValveNote">oto dolum</span>
        </div>
      </div>
    </div>

    <div class="alarm normal station-status" id="upAlarm"><span class="pulse"></span><span id="upAlarmText">UPSIDE verisi bekleniyor…</span></div>
  </section>

  <!-- ================= FLOW ================= -->
  <div class="flow off" id="flowBox">
    <div class="flow-pipe"><span class="flow-water"></span></div>
    <div class="flow-text"><span id="flowTxt">HİDRANT POMPASI beklemede</span>
      <small id="flowNote">hidrant çalışınca su yukarı (60 m) basılır</small>
    </div>
  </div>


  <!-- ================= DOWNSIDE ================= -->
  <section class="station downside">
    <div class="station-head">
      <span class="badge down">▼ DOWNSIDE</span>
      <h2>Alt İstasyon — Kuyu Başı</h2>
      <span class="station-tag">kuyu · hidrant · şebeke girişi · MCP röle kartı</span>
    </div>

    <div class="station-grid">
      <!-- PRESSURES -->
      <div class="card">
        <h3>Basınçlar</h3>

        <div class="press-card">
          <div class="press-top"><span class="press-name">ŞEBEKE</span><span class="pval"><span id="dps1Val">--</span></span></div>
          <span class="press-note">Belediye girişi · kontrol bu değerle yapılmaz</span>
          <div class="pbar"><div class="pfill" id="dps1Fill"></div></div>
        </div>

        <div class="press-card">
          <div class="press-top"><span class="press-name">SİTE</span><span class="pval"><span id="dps2Val">--</span></span></div>
          <span class="press-note">Alt 7.0 · Üst 9.0 bar (60 m kot payı dahil)</span>
          <div class="pbar"><div class="pfill" id="dps2Fill"></div><span class="tick" style="left:70%"></span><span class="tick" style="left:90%"></span></div>
        </div>

        <div class="press-card">
          <div class="press-top"><span class="press-name">YANGIN (UDP)</span><span class="pval"><span id="dps3Val">--</span></span></div>
          <span class="press-note">Upside'dan gelen değer · 2.0 AÇ · 2.5 KAPA</span>
          <div class="pbar"><div class="pfill red" id="dps3Fill"></div><span class="tick" style="left:40%"></span><span class="tick" style="left:50%"></span></div>
        </div>
      </div>

      <!-- PUMPS -->
      <div class="card">
        <h3>Pompa Çıkışları</h3>
        <div class="pump-grid">
          <div class="pump-tile off" id="dpk1"><div class="gear">⚙</div><div class="name">SİTE · İNV</div><div class="st" id="dpk1St">--</div></div>
          <div class="pump-tile off" id="dpk2"><div class="gear">⚙</div><div class="name">SİTE · YEDEK</div><div class="st" id="dpk2St">--</div></div>
          <div class="pump-tile on" id="dpk3"><div class="gear">🌀</div><div class="name">KUYU</div><div class="st" id="dpk3St">--</div></div>
          <div class="pump-tile off" id="dpk4"><div class="gear">💧</div><div class="name">HİDRANT</div><div class="st" id="dpk4St">--</div></div>
        </div>
        <div class="pump-note">Çıkışlar her saniye geri okunur · faz kontrolü aktif</div>
      </div>

      <!-- TANKS -->
      <div class="card">
        <h3>Depolar &amp; Güvenlik</h3>

        <div class="dt-card">
          <div class="dt-head"><span class="dt-name">YANGIN TANKI</span><span class="dt-cap">5 TON</span></div>
          <div class="dt-fill-note">kuyu doldurur · alt sensör kuruysa hidrant çalışmaz</div>
          <div class="tankbar fire">
            <div class="tfill" id="dpt1Fill"></div>
            <span class="mark" style="left:50%"></span>
            <span class="mark" style="left:100%"></span>
            <span class="lbl" id="dpt1Pct">--</span>
          </div>
          <div class="sensor-row">
            <span>Alt sensör: <b class="ok" id="dpt1S1">--</b></span>
            <span>Üst sensör: <b class="no" id="dpt1S2">--</b></span>
            <span class="chip mid" id="dpt1Chip">--</span>
          </div>
        </div>

        <div class="dt-card">
          <div class="dt-head"><span class="dt-name">SİTE TANKI</span><span class="dt-cap">10 TON</span></div>
          <div class="dt-fill-note">şehir şebekesi doldurur · alt seviye kuruysa site pompası çalışmaz</div>
          <div class="tankbar">
            <div class="tfill" id="dpt2Fill"></div>
            <span class="lbl" id="dpt2Pct">--</span>
          </div>
          <div class="sensor-row">
            <span>Alt seviye: <b class="ok" id="dpt2S1">--</b></span>
            <span class="chip on" id="dpt2Chip">--</span>
          </div>
        </div>
      </div>
    </div>

    <div class="alarm normal station-status" id="downAlarm"><span class="pulse"></span><span id="downAlarmText">Sistemler taranıyor…</span></div>
  </section>

  <footer>ÇAMKENT AQUA HMI V01 · Kontrol Paneli · UPSIDE ⇄ DOWNSIDE</footer>

</div>
</body>
</html>

)CAMKENTP9";

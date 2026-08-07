
/* =========================================================
   ÇAMKENT AQUA — Kontrol Paneli (UPSIDE + DOWNSIDE)
   CANLI SÜRÜM: demo veri yerine DOWNSIDE /data ucundan çeker.
   Oturum yoksa (401) giriş sayfasına yönlendirir.
   ========================================================= */

const $ = (id) => document.getElementById(id);

/* ---------------- CANVAS GAUGE ---------------- */
const SVGNS = "http://www.w3.org/2000/svg";
const drawGauge = (el, value, max, color) => {
  if (!el) return;
  const W = 150, H = 95, cx = W / 2, cy = H - 10, r = 62, th = 11;
  const pt = (a) => {
    const rad = Math.PI - a * Math.PI / 180;
    return [cx + r * Math.cos(rad), cy - r * Math.sin(rad)];
  };
  const arc = (a1, a2) => {
    const [x1, y1] = pt(a1), [x2, y2] = pt(a2);
    const large = (a2 - a1) > 180 ? 1 : 0;
    return `M ${x1} ${y1} A ${r} ${r} 0 ${large} 1 ${x2} ${y2}`;
  };
  const v = Math.max(0, Math.min(max, value));
  const a2 = (v / max) * 180;

  let svg = el.querySelector("svg");
  if (!svg) {
    svg = document.createElementNS(SVGNS, "svg");
    svg.setAttribute("viewBox", `0 0 ${W} ${H}`);
    svg.style.width = "150px";
    svg.style.display = "block";
    el.appendChild(svg);
  }
  const grad = document.createElementNS(SVGNS, "linearGradient");
  grad.id = "g" + (color.charCodeAt(0) * 7 % 997);
  grad.innerHTML = `<stop offset="0%" stop-color="${color}" stop-opacity=".55"/><stop offset="100%" stop-color="${color}"/>`;
  svg.innerHTML = `
    <defs></defs>
    <path d="${arc(0, 180)}" fill="none" stroke="rgba(255,255,255,.08)" stroke-width="${th}" stroke-linecap="round"/>
    <path d="${arc(0, a2)}" fill="none" stroke="url(#g${grad.id})" stroke-width="${th}" stroke-linecap="round"/>
    <text x="${cx}" y="${cy - 20}" text-anchor="middle" font-family="Consolas,monospace" font-size="24" font-weight="bold" fill="#e8f0ff">${v.toFixed(1)}</text>
    <text x="${cx}" y="${cy - 4}" text-anchor="middle" font-family="Consolas,monospace" font-size="10" fill="#8aa0c8">bar</text>`;
  const defs = svg.querySelector("defs");
  defs.appendChild(grad);
}

/* ---------------- DOM SYNC ---------------- */
const setFill = (id, pct) => {
  const el = $(id);
  if (el) el.style.width = Math.max(0, Math.min(100, pct)) + "%";
}
const setTxt = (id, t) => {
  const el = $(id);
  if (el) el.textContent = t;
}
const setSensor = (id, v) => {
  const el = $(id);
  if (!el) return;
  el.textContent = v ? "VAR" : "YOK";
  el.className = v ? "ok" : "no";
}
const setCtrl = (id, on) => {
  const el = $(id);
  if (!el) return;
  el.classList.remove("on", "off");
  el.classList.add(on ? "on" : "off");
}
const setPump = (id, stId, on) => {
  const el = $(id);
  if (el) el.classList.remove("on", "off"), el.classList.add(on ? "on" : "off");
  setTxt(stId, on ? "ÇALIŞIYOR" : "BEKLEMEDE");
}

/* ---------------- RENDER ---------------- */
let data = null;
let lastOk = 0;         // son başarılı güncelleme zamanı (ms)
let upOffline = false;  // UPSIDE verisine ulaşılamıyor mu

const render = () => {
  const s = data.site || {}, f = data.fire || {}, d = data.down || {};
  const sWater = s.water || { ton: 0, cap: 0 };
  const fTanks = (f.tanks || []).map(t => t || { ton: 0, cap: 0 });
  const sPump = s.pump || { on: false, freq: 0, cur: 0, volt: 0 };
  const fPump = f.pump || { on: false, freq: 0, cur: 0, volt: 0 };
  const fBig = f.big || { on: false };
  const fv = data.fillValve || { on: false };
  const dp = d.pumps || {};
  const ft = d.fireTank || { pct: 0, s1: false, s2: false };
  const st = d.siteTank || { pct: 0, s1: false };

  const fireTotal = fTanks.reduce((a, t) => a + t.ton, 0);
  const fireCap = fTanks.reduce((a, t) => a + t.cap, 0);

  /* summary */
  setTxt("summarySite", sWater.ton.toFixed(1) + " t");
  setTxt("summaryFire", fireTotal.toFixed(1) + " / " + fireCap.toFixed(0) + " t");
  setTxt("summaryPump", sPump.on ? "ÇALIŞIYOR" : "DURDU");

  /* upside tanks */
  setFill("tk1Fill", sWater.cap > 0 ? (sWater.ton / sWater.cap) * 100 : 0);
  setTxt("tk1Val", sWater.cap > 0 ? Math.round((sWater.ton / sWater.cap) * 100) + "%" : "--");
  setTxt("tk1Ton", sWater.ton.toFixed(1) + "t");

  setFill("tkFireFill", fireCap > 0 ? (fireTotal / fireCap) * 100 : 0);
  setTxt("tkFireVal", fireCap > 0 ? Math.round((fireTotal / fireCap) * 100) + "%" : "--");
  setTxt("tkFireTon", fireTotal.toFixed(1) + "t");

  /* gauges */
  drawGauge($("gaugeSite"), s.pressure || 0, 3.0, "#35d0ff");
  drawGauge($("gaugeFire"), f.pressure || 0, 3.0, "#ff8a3d");

  /* controls */
  setCtrl("ctrlSitePump", sPump.on);
  setTxt("ctrlSitePumpSt", sPump.on ? "ÇALIŞIYOR" : "DURDU");
  setTxt("ctrlSiteFreq", sPump.freq.toFixed(1) + " Hz");
  setTxt("ctrlSiteCur", sPump.cur.toFixed(1) + " A");
  setTxt("ctrlSiteVolt", Math.round(sPump.volt) + " V");

  setCtrl("ctrlFirePump", fPump.on);
  setTxt("ctrlFirePumpSt", fPump.on ? "ÇALIŞIYOR" : "DURDU");
  setTxt("ctrlFireFreq", fPump.freq.toFixed(1) + " Hz");
  setTxt("ctrlFireCur", fPump.cur.toFixed(1) + " A");
  setTxt("ctrlFireVolt", Math.round(fPump.volt) + " V");

  setCtrl("ctrlBigFire", fBig.on);
  setTxt("ctrlBigFireSt", fBig.on ? "DEVREYE ALINDI" : "HAZIR");

  setCtrl("ctrlFillValve", fv.on);
  setTxt("ctrlFillValveSt", fv.on ? "AÇIK" : "KAPALI");
  setTxt("ctrlFillValveNote", fv.on ? "yangın tanklarını dolduruyor" : "oto dolum bekliyor");

  /* downside pressures */
  setTxt("dps1Val", (d.p1 || 0).toFixed(2));
  setFill("dps1Fill", ((d.p1 || 0) / 12) * 100);
  setTxt("dps2Val", (d.p2 || 0).toFixed(2));
  setFill("dps2Fill", ((d.p2 || 0) / 12) * 100);
  setTxt("dps3Val", (d.p3 || 0).toFixed(2));
  setFill("dps3Fill", ((d.p3 || 0) / 5) * 100);

  /* downside pumps */
  setPump("dpk1", "dpk1St", !!(dp.inv && dp.inv.on));
  setPump("dpk2", "dpk2St", !!(dp.yedek && dp.yedek.on));
  setPump("dpk3", "dpk3St", !!(dp.kuyu && dp.kuyu.on));
  setPump("dpk4", "dpk4St", !!(dp.hidrant && dp.hidrant.on));

  const verEl = $("ver");
  if (verEl && data.ver) verEl.textContent = data.ver;

  /* downside tanks */
  setFill("dpt1Fill", ft.pct);
  setTxt("dpt1Pct", Math.round(ft.pct) + "%");
  setSensor("dpt1S1", ft.s1);
  setSensor("dpt1S2", ft.s2);
  setTxt("dpt1Chip", ft.pct < 50 ? "SEVİYE DÜŞÜK" : "SEVİYE YETERLİ");
  $("dpt1Chip").className = "chip " + (ft.pct < 50 ? "mid" : "on");

  setFill("dpt2Fill", st.pct);
  setTxt("dpt2Pct", Math.round(st.pct) + "%");
  setSensor("dpt2S1", st.s1);
  setTxt("dpt2Chip", st.pct < 30 ? "SEVİYE DÜŞÜK" : "SEVİYE YETERLİ");
  $("dpt2Chip").className = "chip " + (st.pct < 30 ? "mid" : "on");

  upStatus();
  downStatus();
}

/* ---------------- İSTASYON DURUMLARI ---------------- */
const upStatus = () => {
  const a = $("upAlarm"), t = $("upAlarmText");
  if (!a || !t) return;
  const s = data.site || {}, f = data.fire || {};
  const sWater = s.water || { ton: 0, cap: 0 };
  const fTanks = (f.tanks || []).map(x => x || { ton: 0, cap: 0 });
  const fireTotal = fTanks.reduce((acc, x) => acc + x.ton, 0);
  const fireCap = fTanks.reduce((acc, x) => acc + x.cap, 0);
  const firePct = fireCap > 0 ? (fireTotal / fireCap) * 100 : 0;
  a.classList.remove("normal", "warn", "critical");

  if (upOffline) {
    a.classList.add("warn");
    setTxt("upAlarmText", "UPSIDE ÇEVRİMDIŞI — VERİ GELMİYOR");
  } else if (sWater.ton < 1.5) {
    a.classList.add("critical");
    setTxt("upAlarmText", "DÜŞÜK SU SEVİYESİ — DEPO KONTROLÜ GEREKLİ! (" + sWater.ton.toFixed(1) + " t)");
  } else if (firePct < 30) {
    a.classList.add("warn");
    setTxt("upAlarmText", "YANGIN REZERVİ DÜŞÜK — %" + Math.round(firePct));
  } else {
    a.classList.add("normal");
    setTxt("upAlarmText", "ÜST İSTASYON NORMAL — site " + sWater.ton.toFixed(1) + " t · rezerv %" + Math.round(firePct));
  }
}

const downStatus = () => {
  const a = $("downAlarm"), t = $("downAlarmText");
  if (!a || !t) return;
  const d = data.down || {};
  const ft = d.fireTank || { pct: 100 };
  const p1 = d.p1 || 0, p2 = d.p2 || 0, p3 = d.p3 || 0;
  const alarms = d.alarms || [];
  a.classList.remove("normal", "warn", "critical");

  const kritik = alarms.some(x => /bos|hatasi|yok|calisamaz/i.test(x));
  if (kritik || ft.pct < 30) {
    a.classList.add("critical");
    setTxt("downAlarmText", (alarms.length ? alarms[0] + " — " : "") + "DÜŞÜK SU SEVİYESİ, DEPO KONTROLÜ GEREKLİ!");
  } else if (alarms.length) {
    a.classList.add("warn");
    setTxt("downAlarmText", alarms[0]);
  } else if (p2 < 2.0 || p3 > 2.9) {
    a.classList.add("warn");
    setTxt("downAlarmText", "BASINÇ UYARISI — HAT DEĞERLERİ SINIR DIŞINDA");
  } else {
    a.classList.add("normal");
    setTxt("downAlarmText", "ALT İSTASYON NORMAL — şebeke " + p1.toFixed(1) + " · site " + p2.toFixed(1) + " · yangın " + p3.toFixed(1) + " bar");
  }
}

/* ---------------- FETCH ---------------- */
const fetchData = () => {
  fetch("data", { cache: "no-store" })
    .then(r => {
      if (r.status === 401) { location.href = "login.html"; throw new Error("auth"); }
      if (!r.ok) throw new Error("http " + r.status);
      return r.json();
    })
    .then(j => {
      data = j;
      lastOk = Date.now();
      upOffline = j.upOnline === false;
      render();
    })
    .catch(e => {
      if (e && e.message === "auth") return;
      const a = $("downAlarm");
      if (a) {
        a.classList.remove("normal", "warn", "critical");
        a.classList.add("warn");
        const t = $("downAlarmText");
        if (t) t.textContent = "VERİ GELMİYOR — bağlantı denetleniyor…";
      }
    });
}

/* ---------------- CLOCK ---------------- */
const clock = () => {
  const d = new Date();
  const p = (n) => String(n).padStart(2, "0");
  setTxt("time", p(d.getHours()) + ":" + p(d.getMinutes()) + ":" + p(d.getSeconds()));
}

/* ---------------- BOOT ---------------- */
clock();
setInterval(clock, 1000);
fetchData();
setInterval(fetchData, 2000);


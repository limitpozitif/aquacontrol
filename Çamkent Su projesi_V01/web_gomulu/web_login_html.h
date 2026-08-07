// login.html (web_panel_data) -> gomulu (PROGMEM) web dosyasi
#pragma once

static const char WEB_LOGIN_HTML[] PROGMEM = R"CAMKENTP9("
<!DOCTYPE html>
<html lang="tr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Giriş — Çamkent Aqua</title>
<style>
  :root{--bg0:#0a1120;--bg1:#0e1730;--bg2:#131d3a;--line:#223157;--line2:#2e4070;
        --txt:#e8f0ff;--dim:#8aa0c8;--water:#35d0ff;--ok:#3ef59a;--bad:#ff5d6c;--warn:#ffd166}
  *{box-sizing:border-box;margin:0;padding:0}
  html,body{background:radial-gradient(1200px 700px at 20% -10%,#16224a 0%,#0a1120 55%) fixed,#0a1120;
            font:14px/1.45 "Segoe UI",system-ui,Arial,sans-serif;color:var(--txt);min-height:100vh}
  .wrap{max-width:380px;margin:0 auto;padding:40px 18px}
  .card{background:linear-gradient(180deg,rgba(19,29,58,.85),rgba(10,17,32,.9));
        border:1px solid var(--line);border-radius:14px;box-shadow:0 10px 30px rgba(0,0,0,.35);
        padding:26px 24px}
  .logo{font-weight:800;font-size:20px;letter-spacing:1px;display:flex;align-items:center;gap:10px;margin-bottom:6px}
  .logo em{font-style:normal;color:var(--water)}
  .sub{font-size:12px;color:var(--dim);margin-bottom:22px;letter-spacing:.4px}
  label{display:block;font-size:11px;letter-spacing:1.2px;color:var(--dim);margin:14px 0 6px}
  input{width:100%;background:rgba(0,0,0,.4);border:1px solid var(--line2);border-radius:8px;
        color:var(--txt);font-size:16px;padding:12px 14px;outline:none}
  input:focus{border-color:var(--water)}
  button{width:100%;margin-top:22px;background:linear-gradient(135deg,#1c7fb8,var(--water));
         border:0;border-radius:8px;color:#04212f;font-weight:800;font-size:15px;letter-spacing:1px;
         padding:14px;cursor:pointer}
  button:active{transform:translateY(1px)}
  .err{display:none;margin-top:14px;font-size:12.5px;color:var(--bad);
       border:1px solid rgba(255,93,108,.45);border-radius:8px;padding:10px;background:rgba(255,93,108,.08)}
  .info{margin-top:18px;font-size:11px;color:var(--dim);line-height:1.6;border-top:1px dashed var(--line);padding-top:14px}
  .spinner{display:none;text-align:center;margin-top:16px;font-size:12px;color:var(--dim);letter-spacing:1px}
  .btn2{margin-top:10px;background:transparent;border:1px solid var(--line2);border-radius:8px;
        color:var(--water);font-weight:700;font-size:13px;letter-spacing:1px;padding:12px;cursor:pointer}
  .btn2:hover{border-color:var(--water)}
  .sep{margin:18px 0 12px;display:flex;align-items:center;gap:10px;color:var(--dim);font-size:11px;
        letter-spacing:1px;font-weight:700}
  .sep::before,.sep::after{content:"";flex:1;height:1px;background:var(--line2)}
  select{width:100%;box-sizing:border-box;background:#0d1220;border:1px solid var(--line2);
        border-radius:8px;color:#cbd5f0;font:inherit;padding:12px;margin-bottom:12px;cursor:pointer}
  select:focus{outline:none;border-color:var(--water)}
  .status{display:none;margin-top:14px;font-size:12.5px;border-radius:8px;padding:10px;line-height:1.5}
  .status.ok{color:var(--ok);border:1px solid rgba(62,245,154,.45);background:rgba(62,245,154,.08)}
  .status.warn{color:var(--warn);border:1px solid rgba(255,209,102,.45);background:rgba(255,209,102,.08)}
  .status.bad{color:var(--bad);border:1px solid rgba(255,93,108,.45);background:rgba(255,93,108,.08)}
  .status.neu{color:var(--dim);border:1px solid var(--line2);background:rgba(0,0,0,.25)}
</style>
</head>
<body>
<div class="wrap">
  <div class="card">
    <div class="logo"><span>💧</span><span>ÇAMKENT <em>AQUA</em></span></div>
    <div class="sub">Güvenli giriş — telefon numaranız ve sistem şifrenizle</div>

    <label for="tel">TELEFON NUMARASI</label>
    <input id="tel" type="tel" inputmode="numeric" maxlength="11" pattern="[0-9]{11}" autocomplete="tel" placeholder="05XX XXX XX XX" autofocus>

    <label for="sifre">ŞİFRE</label>
    <input id="sifre" type="password" inputmode="numeric" autocomplete="current-password" placeholder="6 haneli şifre">

    <div class="sep">İLK GİRİŞ / NUMARA DEĞİŞİKLİĞİ</div>
    <label for="ad">AD SOYAD</label>
    <input id="ad" type="text" autocomplete="name" placeholder="Ad Soyad">
    <label for="hane">HANE NO</label>
    <input id="hane" type="text" inputmode="numeric" autocomplete="off" placeholder="Örn: 12">
    <label for="eski" id="eskiLabel" style="display:none">HANENİZ DOLU — SİLİNECEK ESKİ NUMARA</label>
    <select id="eski" style="display:none"></select>

    <button id="btn">GİRİŞ YAP</button>
    <button id="btnTalep" class="btn2">ERİŞİM TALEP ET</button>
    <div class="err" id="err"></div>
    <div class="status" id="st"></div>
    <div class="spinner" id="sp">Doğrulanıyor…</div>

    <div class="info">
      İlk giriş için ad soyad ve hane no ile "Erişim Talep Et" deyin; yönetici onaylayınca şifreniz bu alana otomatik dolar.<br>
      Bir haneye en fazla 3 numara kaydı eklenebilir. Haneniz doluysa silinecek eski kayıt için numara seçmenizi sistem sorar.<br>
      Giriş telefon numaranıza bağlıdır — her cihazdan kullanılabilir.
    </div>
  </div>
</div>

<script>
  const tel = document.getElementById("tel");
  const sifre = document.getElementById("sifre");
  const err = document.getElementById("err");
  const sp = document.getElementById("sp");
  const btn = document.getElementById("btn");

  function hata(msg) { err.textContent = msg; err.style.display = "block"; }

  function gir() {
    err.style.display = "none";
    const t = tel.value.trim();
    const s = sifre.value.trim();
    if (t.length !== 11) { hata("11 haneli telefon numarası girin (05XX XXX XX XX)."); return; }
    if (s.length < 4) { hata("Şifre girin."); return; }
    sp.style.display = "block";
    btn.disabled = true;

    fetch("/login?tel=" + encodeURIComponent(t) + "&sifre=" + encodeURIComponent(s),
          { cache: "no-store" })
      .then(r => r.json().catch(() => null))
      .then(d => {
        if (d && d.ok) { location.href = "/"; return; }
        sp.style.display = "none";
        btn.disabled = false;
        hata((d && d.msg) || "Giriş başarısız.");
      })
      .catch(() => {
        sp.style.display = "none";
        btn.disabled = false;
        hata("Bağlantı kurulamadı. Tekrar deneyin.");
      });
  }

  btn.addEventListener("click", gir);
  [tel, sifre].forEach(i => i.addEventListener("keydown", e => { if (e.key === "Enter") gir(); }));

  const btnTalep = document.getElementById("btnTalep");
  const st = document.getElementById("st");
  const ad = document.getElementById("ad");
  const hane = document.getElementById("hane");
  const eskiSel = document.getElementById("eski");
  const eskiLabel = document.getElementById("eskiLabel");
  let token = null;
  let pollTimer = null;
  let bekleDegisim = false;

  function durum(msg, cls) {
    st.textContent = msg;
    st.className = "status" + (cls ? " " + cls : " neu");
    st.style.display = "block";
  }

  function talepGonder(degisim) {
    const t = tel.value.trim();
    fetch("/talep?tel=" + encodeURIComponent(t) +
          "&ad=" + encodeURIComponent(ad.value.trim()) +
          "&hane=" + encodeURIComponent(hane.value.trim()) +
          "&degisim=" + (degisim ? "1" : "0") +
          (degisim && eskiSel.value ? "&eski=" + encodeURIComponent(eskiSel.value) : ""),
          { cache: "no-store" })
      .then(r => r.json())
      .then(d => {
        if (d.durum === "onaylandi") {
          sifre.value = d.sifre;
          durum("Şifreniz hazır. GİRİŞ YAP butonuyla devam edin.", "ok");
          btnTalep.style.display = "none";
          sifre.focus();
          return;
        }
        if (d.durum === "isleniyor") {
          eskiLabel.style.display = "none";
          eskiSel.style.display = "none";
          bekleDegisim = false;
          token = d.token;
          durum("Talebiniz alındı. Uygun ise onaylanacaktır; şifreniz geldiğinde burada görünecektir.", "neu");
          pollTimer = setInterval(poll, 2000);
        } else if (d.durum === "mesgul") {
          durum("Talep kuyruğu şu an dolu. Yönetici onayladıkça yeniden deneyin.", "warn");
          btnTalep.disabled = false;
        } else if (d.durum === "degisim_onay") {
          eskiSel.innerHTML = "";
          (d.eski || "").split("|").filter(x => x).forEach(ent => {
            const p = ent.split(";");
            const o = document.createElement("option");
            o.value = p[0];
            o.textContent = p[0] + " (" + (p[1] || "?") + ")";
            eskiSel.appendChild(o);
          });
          eskiLabel.style.display = "block";
          eskiSel.style.display = "block";
          bekleDegisim = true;
          durum("Bu hanede en fazla 3 numara kaydı olabilir. Listeden silinecek eski kaydı seçip talebi tekrar gönderin.", "warn");
          btnTalep.disabled = false;
        } else {
          durum("Talep alınamadı. Tekrar deneyin.", "bad");
          btnTalep.disabled = false;
        }
      })
      .catch(() => {
        durum("Bağlantı kurulamadı. Tekrar deneyin.", "bad");
        btnTalep.disabled = false;
      });
  }

  function talep() {
    err.style.display = "none";
    const t = tel.value.trim();
    if (t.length !== 11) { hata("11 haneli telefon numarası girin (05XX XXX XX XX)."); return; }
    if (ad.value.trim().length < 2) { hata("Ad soyad girin."); return; }
    if (hane.value.trim().length < 1) { hata("Hane no girin."); return; }
    btnTalep.disabled = true;
    durum("Talebiniz işleniyor…", "neu");
    if (bekleDegisim) { bekleDegisim = false; talepGonder(1); }
    else talepGonder(0);
  }

  function poll() {
    if (!token) return;
    fetch("/durum?tel=" + encodeURIComponent(tel.value.trim()) +
          "&token=" + encodeURIComponent(token), { cache: "no-store" })
      .then(r => r.json())
      .then(d => {
        if (d.durum === "onaylandi") {
          clearInterval(pollTimer); pollTimer = null;
          sifre.value = d.sifre;
          durum("Şifreniz hazır. GİRİŞ YAP butonuyla devam edin.", "ok");
          btnTalep.style.display = "none";
          sifre.focus();
        } else if (d.durum === "red") {
          clearInterval(pollTimer); pollTimer = null;
          durum("Talebiniz reddedildi. Yöneticiyle iletişime geçin.", "bad");
          btnTalep.disabled = false;
          token = null;
        } else if (d.durum === "zaman_asimi" || d.durum === "yok") {
          clearInterval(pollTimer); pollTimer = null;
          durum(d.durum === "zaman_asimi"
                ? "Talep süresi doldu. Yeniden deneyin."
                : "Talep bulunamadı. Yeniden deneyin.", "warn");
          btnTalep.disabled = false;
          token = null;
        }
      })
      .catch(() => {});
  }

  btnTalep.addEventListener("click", talep);
</script>
</body>
</html>

)CAMKENTP9";

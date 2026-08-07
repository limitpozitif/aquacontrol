
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
    if (t.length < 7) { hata("GeÃ§erli bir telefon numarasÄ± girin."); return; }
    if (s.length < 4) { hata("Åifre girin."); return; }
    sp.style.display = "block";
    btn.disabled = true;

    fetch("/login?tel=" + encodeURIComponent(t) + "&sifre=" + encodeURIComponent(s),
          { cache: "no-store" })
      .then(r => {
        if (r.redirected) { location.href = r.url; return; }
        return r.text().then(txt => {
          if (r.status === 302) { location.href = "/"; return; }
          sp.style.display = "none";
          btn.disabled = false;
          hata(txt || ("GiriÅŸ baÅŸarÄ±sÄ±z (" + r.status + ")"));
        });
      })
      .catch(() => {
        sp.style.display = "none";
        btn.disabled = false;
        hata("BaÄŸlantÄ± kurulamadÄ±. Tekrar deneyin.");
      });
  }

  btn.addEventListener("click", gir);
  [tel, sifre].forEach(i => i.addEventListener("keydown", e => { if (e.key === "Enter") gir(); }));


/* AQUA KONTROL — ortak site betikleri */

(function () {
  'use strict';

  /* --- Mobil menü (burger) --- */
  var burger = document.getElementById('burger');
  var nav = document.getElementById('navlinks');
  if (burger && nav) {
    burger.addEventListener('click', function () { nav.classList.toggle('open'); });
    nav.addEventListener('click', function (e) {
      if (e.target.tagName === 'A') nav.classList.remove('open');
    });
  }

  /* --- Reveal animasyonu --- */
  if ('IntersectionObserver' in window) {
    var io = new IntersectionObserver(function (entries) {
      entries.forEach(function (en) { if (en.isIntersecting) { en.target.classList.add('in'); io.unobserve(en.target); } });
    }, { threshold: 0.15 });
    document.querySelectorAll('.reveal').forEach(function (el) { io.observe(el); });
  }

  /* --- Hero kabarcıklar --- */
  (function () {
    var box = document.getElementById('bubbles');
    if (!box) return;
    for (var i = 0; i < 10; i++) {
      var b = document.createElement('span');
      b.className = 'bubble';
      var s = 8 + Math.random() * 22;
      b.style.width = s + 'px';
      b.style.height = s + 'px';
      b.style.left = (Math.random() * 100) + '%';
      b.style.bottom = (Math.random() * 40) + '%';
      b.style.animationDuration = (6 + Math.random() * 8) + 's';
      b.style.animationDelay = (Math.random() * 8) + 's';
      box.appendChild(b);
    }
  })();

  /* --- Animasyonlu sayaçlar --- */
  (function () {
    function anim(el) {
      var target = +el.getAttribute('data-count');
      var dur = 1400, t0 = null;
      function step(ts) {
        if (!t0) t0 = ts;
        var p = Math.min((ts - t0) / dur, 1);
        el.textContent = Math.floor(target * (1 - Math.pow(1 - p, 3)));
        if (p < 1) requestAnimationFrame(step);
      }
      requestAnimationFrame(step);
    }
    if ('IntersectionObserver' in window) {
      var cio = new IntersectionObserver(function (entries) {
        entries.forEach(function (en) {
          if (en.isIntersecting) { anim(en.target); cio.unobserve(en.target); }
        });
      }, { threshold: 0.5 });
      document.querySelectorAll('.stat-num').forEach(function (el) { cio.observe(el); });
    }
  })();

  /* --- Ziyaretçi sayaçları --- */
  (function () {
    var STORAGE_KEY_ACTIVE = 'aqua_active_tab';
    var STORAGE_KEY_TOTAL = 'aqua_total_visits';

    var total = parseInt(localStorage.getItem(STORAGE_KEY_TOTAL) || '0', 10);
    var totalEl = document.getElementById('total-count');
    var activeEl = document.getElementById('active-count');
    if (!totalEl || !activeEl) return;

    if (!sessionStorage.getItem(STORAGE_KEY_ACTIVE)) {
      total++;
      localStorage.setItem(STORAGE_KEY_TOTAL, total);
      sessionStorage.setItem(STORAGE_KEY_ACTIVE, '1');
    }

    var active = parseInt(localStorage.getItem(STORAGE_KEY_ACTIVE) || '0', 10);
    if (active < 1) active = 1;
    activeEl.textContent = active;
    totalEl.textContent = total;

    function activate() {
      var n = parseInt(localStorage.getItem(STORAGE_KEY_ACTIVE) || '0', 10);
      n++;
      localStorage.setItem(STORAGE_KEY_ACTIVE, n);
      localStorage.setItem('_aqua_ping', Date.now());
      activeEl.textContent = n;
    }

    function deactivate() {
      var n = parseInt(localStorage.getItem(STORAGE_KEY_ACTIVE) || '0', 10);
      n = Math.max(0, n - 1);
      localStorage.setItem(STORAGE_KEY_ACTIVE, n);
      activeEl.textContent = Math.max(1, n);
    }

    window.addEventListener('storage', function (e) {
      if (e.key === STORAGE_KEY_ACTIVE) {
        activeEl.textContent = Math.max(1, parseInt(e.newValue || '1', 10));
      }
    });

    window.addEventListener('beforeunload', function () { deactivate(); });

    document.addEventListener('visibilitychange', function () {
      if (document.hidden) {
        deactivate();
      } else {
        activate();
      }
    });
  })();

  /* --- Mobil akordeon (768px altı: bölümler kapatılabilir) --- */
  document.addEventListener('DOMContentLoaded', function () {
    var mm = window.matchMedia('(max-width: 768px)');
    if (!mm.matches) return;
    var secs = document.querySelectorAll('section');
    for (var i = 0; i < secs.length; i++) {
      var sec = secs[i];
      if (sec.classList.contains('hero')) continue;
      if (sec.classList.contains('cta')) continue;
      if (sec.classList.contains('cm-no-collapse')) continue;
      var h2 = sec.querySelector('h2');
      if (!h2) continue;
      var title = h2.textContent.trim();
      var btn = document.createElement('div');
      btn.className = 'cm-toggle';
      btn.innerHTML = '<span>' + title + '</span><span class="cm-caret">\u25B6</span>';
      sec.insertBefore(btn, sec.firstChild);
      var wrap = sec.querySelector('.wrap');
      if (wrap) {
        wrap.classList.add('cm-body');
      } else {
        var body = document.createElement('div');
        body.className = 'cm-body';
        while (sec.children.length > 1) {
          body.appendChild(sec.children[1]);
        }
        sec.appendChild(body);
      }
      (function (b, c) {
        c.classList.add('open');
        b.querySelector('.cm-caret').classList.add('open');
        b.addEventListener('click', function () {
          var open = c.classList.toggle('open');
          b.querySelector('.cm-caret').classList.toggle('open', open);
        });
      })(btn, sec);
    }
  });
})();
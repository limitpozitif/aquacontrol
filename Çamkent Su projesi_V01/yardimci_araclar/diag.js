window.addEventListener("load", function () {
  [7, 8, 9].forEach(function (n) {
    var s = document.querySelectorAll(".slide")[n - 1];
    var out = n + ": slide sH=" + s.scrollHeight + " | ";
    Array.prototype.forEach.call(s.children, function (c) {
      if (c.className === "bg") return;
      out += c.tagName + "." + (c.className + "").replace(/ /g, ".") + "(" + Math.round(c.offsetHeight) + "/" + Math.round(c.scrollHeight) + ") ";
    });
    var d = document.createElement("div");
    d.id = "ov" + n;
    d.style.cssText = "position:fixed;top:" + ((n - 1) * 40) + "px;left:0;background:#0ff;color:#000;font:bold 12px monospace;z-index:99999;white-space:pre";
    d.textContent = out;
    document.body.appendChild(d);
  });
});

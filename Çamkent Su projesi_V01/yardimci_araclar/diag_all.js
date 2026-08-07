window.addEventListener("load", function () {
  var slides = document.querySelectorAll(".slide");
  var out = "";
  for (var i = 0; i < slides.length; i++) {
    var s = slides[i];
    out += (i + 1) + ": slide sH=" + s.scrollHeight + " | ";
    Array.prototype.forEach.call(s.children, function (c) {
      if (c.className === "bg") return;
      out += c.tagName + "." + (c.className + "").replace(/ /g, ".") + "(" + Math.round(c.offsetHeight) + "/" + Math.round(c.scrollHeight) + ") ";
    });
    out += "\n";
  }
  var d = document.createElement("div");
  d.id = "ovall";
  d.style.cssText = "position:fixed;top:0;left:0;background:#0ff;color:#000;font:bold 11px monospace;z-index:99999;white-space:pre";
  d.textContent = out;
  document.body.appendChild(d);
});

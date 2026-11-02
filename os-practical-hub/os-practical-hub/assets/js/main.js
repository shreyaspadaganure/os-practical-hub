// Copy-code buttons
document.addEventListener('click', function (e) {
  var btn = e.target.closest('.copy-btn');
  if (!btn) return;
  var box = btn.closest('.codebox');
  var text = box.querySelector('pre').innerText.replace(/\n$/, '');
  function ok() {
    var old = btn.textContent;
    btn.textContent = 'Copied!';
    btn.classList.add('done');
    setTimeout(function () { btn.textContent = old; btn.classList.remove('done'); }, 1800);
  }
  function fallback() {
    var ta = document.createElement('textarea');
    ta.value = text; ta.setAttribute('readonly', '');
    ta.style.position = 'fixed'; ta.style.opacity = '0';
    document.body.appendChild(ta); ta.select();
    try { document.execCommand('copy'); ok(); } catch (err) { btn.textContent = 'Press Ctrl+C'; }
    document.body.removeChild(ta);
  }
  if (navigator.clipboard && window.isSecureContext) {
    navigator.clipboard.writeText(text).then(ok, fallback);
  } else { fallback(); }
});

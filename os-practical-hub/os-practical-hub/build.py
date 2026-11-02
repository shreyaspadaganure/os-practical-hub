#!/usr/bin/env python3
"""
OS Practical Hub - static site builder.

Run:  python3 build.py
Needs: pip install pygments   (only used here, at build time - the website itself has no dependencies)

Edit the PRACTICALS list below, the code in code/, or the theory in theory/,
then run this script again to regenerate index.html and practical-N.html.
"""
import html
from pathlib import Path
from pygments import highlight
from pygments.formatters import HtmlFormatter
from pygments.lexers import BashLexer, CLexer

ROOT = Path(__file__).parent

SITE_TITLE = "OS Practical Hub"
SITE_SUBTITLE = "Operating System Practical Programs"

# number, title, code file, shown file name, language, extra notice (HTML) shown under the code
PRACTICALS = [
    (1, "Address Book using Shell Script", "code/p1.sh", "addressbook.sh", "bash", None),
    (2, "Process Management using fork() and wait() \u2013 Zombie and Orphan Processes", "code/p2.c", "fork_sort.c", "c", None),
    (3, "Producer\u2013Consumer Problem using Semaphores and Mutex", "code/p3.c", "pc.c", "c", None),
    (4, "Dining Philosophers Problem using Semaphores", "code/p4.c", "dining.c", "c", None),
    (5, "Banker's Algorithm", "code/p5.c", "bankers.c", "c", None),
    (6, "Full-Duplex Communication using FIFO", "code/p6.c", "fifo1.c  (Process 1)", "c",
     "<strong>Note:</strong> the uploaded PDF contains only the code for <strong>Process 1</strong> (<code>fifo1.c</code>). "
     "The code for <strong>Process 2</strong> (<code>fifo2.c</code>), which appears in the output screenshot, is not in the PDF, "
     "so it is not shown here."),
    (7, "Full-Duplex Communication using Pipes", "code/p7.c", "pipes.c", "c", None),
    (8, "Disk Scheduling Algorithms \u2013 SSTF, SCAN and C-LOOK", "code/p8.c", "disk.c", "c", None),
]

LEXERS = {"bash": BashLexer(), "c": CLexer()}
FORMATTER = HtmlFormatter(nowrap=False)

HEAD = """<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>{title}</title>
<meta name="description" content="{desc}">
<link rel="stylesheet" href="assets/css/code.css">
<link rel="stylesheet" href="assets/css/style.css">
</head>
<body>
"""

FOOT = """<footer class="site-footer"><div class="wrap">OS Practical Hub</div></footer>
<script src="assets/js/main.js"></script>
</body>
</html>
"""


def build_index():
    items = "\n".join(
        f'    <li><a href="practical-{n}.html"><span class="num">Practical {n}</span>'
        f'<span class="ptitle">{html.escape(t)}</span></a></li>'
        for n, t, *_ in PRACTICALS
    )
    page = (
        HEAD.format(title=f"{SITE_TITLE} \u2013 {SITE_SUBTITLE}", desc=SITE_SUBTITLE)
        + f"""<header class="site-header"><div class="wrap">
  <h1>{SITE_TITLE}</h1>
  <p>{SITE_SUBTITLE}</p>
</div></header>
<main class="wrap">
  <ol class="practical-list" aria-label="Practicals">
{items}
  </ol>
</main>
"""
        + FOOT
    )
    (ROOT / "index.html").write_text(page, encoding="utf-8")


def build_practical(n, title, code_file, shown_name, lang, notice):
    code = (ROOT / code_file).read_text(encoding="utf-8").rstrip("\n") + "\n"
    theory = (ROOT / "theory" / f"p{n}.html").read_text(encoding="utf-8")
    code_html = highlight(code, LEXERS[lang], FORMATTER)
    notice_html = f'\n    <p class="notice">{notice}</p>' if notice else ""
    img = f"assets/img/practical-{n}-output.png"
    page = (
        HEAD.format(title=f"Practical {n} \u2013 {html.escape(title)} | {SITE_TITLE}", desc=html.escape(title))
        + f"""<header class="site-header"><div class="wrap">
  <h1><a href="index.html">{SITE_TITLE}</a></h1>
  <p>{SITE_SUBTITLE}</p>
</div></header>
<main class="wrap">
  <a class="back" href="index.html">&larr; Back to Practicals</a>
  <p class="page-kicker" style="margin-top:14px">Practical {n}</p>
  <h1 class="page-title" style="margin-top:2px">{html.escape(title)}</h1>

  <section class="block" id="theory">
    <h2>Theory</h2>
{theory}  </section>

  <section class="block" id="code">
    <h2>Code</h2>
    <div class="codebox">
      <div class="codebar">
        <span class="left"><span class="dots"><i></i><i></i><i></i></span><span class="fname">{html.escape(shown_name)}</span></span>
        <button type="button" class="copy-btn">Copy Code</button>
      </div>
{code_html}    </div>{notice_html}
  </section>

  <section class="block" id="output">
    <h2>Output</h2>
    <a href="{img}" target="_blank" rel="noopener"><img class="shot" src="{img}" alt="Output screenshot of Practical {n}: {html.escape(title)}"></a>
    <p class="shot-hint">Tap or click the screenshot to open it full size.</p>
  </section>

  <a class="back" href="index.html" style="margin-bottom:32px">&larr; Back to Practicals</a>
</main>
"""
        + FOOT
    )
    (ROOT / f"practical-{n}.html").write_text(page, encoding="utf-8")


if __name__ == "__main__":
    build_index()
    for p in PRACTICALS:
        build_practical(*p)
    print("Built index.html and", len(PRACTICALS), "practical pages.")

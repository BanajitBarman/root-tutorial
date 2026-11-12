#!/usr/bin/env python3
"""Assemble the manual and make the PDF.

chapters/*.md may contain these markers, each on its own line:
    {{code: macros/ch03_histograms/hist_basic.C}}    -> the file as a code block
    {{output: macros/ch03_histograms/hist_basic}}    -> its saved output (from
                                                       tools/run_examples.py),
                                                       without "Info in <TCanvas::Print>" lines
    {{output!: ...}}                                 -> the same, keeping every line
The result is ROOT_Tutorial_Manual.md (stand-alone Markdown) and, with --pdf,
ROOT_Tutorial_Manual.pdf.
"""
import glob, os, re, subprocess, sys

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT_MD = os.path.join(ROOT_DIR, "ROOT_Tutorial_Manual.md")
OUT_PDF = os.path.join(ROOT_DIR, "ROOT_Tutorial_Manual.pdf")
LANG = {".C": "cpp", ".py": "python", ".txt": "text", ".sh": "bash"}
MARK = re.compile(r"^\{\{(code|output!?):\s*(.+?)\s*\}\}\s*$")


def code_block(rel):
    path = os.path.join(ROOT_DIR, rel)
    with open(path) as fh:
        text = fh.read().rstrip("\n")
    lang = LANG.get(os.path.splitext(rel)[1], "text")
    return f"```{lang}\n{text}\n```"


def output_block(rel, keep_all):
    path = os.path.join(ROOT_DIR, "outputs", rel + ".txt")
    with open(path) as fh:
        lines = fh.read().rstrip("\n").split("\n")
    if not keep_all:
        lines = [l for l in lines if not l.startswith("Info in <TCanvas::Print>")]
    lines = [l.rstrip() for l in lines]
    return "```text\n" + "\n".join(lines) + "\n```"


def assemble():
    parts = []
    for ch in sorted(glob.glob(os.path.join(ROOT_DIR, "chapters", "*.md"))):
        out = []
        with open(ch) as fh:
            for line in fh:
                m = MARK.match(line.strip())
                if not m:
                    out.append(line.rstrip("\n"))
                elif m.group(1) == "code":
                    out.append(code_block(m.group(2)))
                else:
                    out.append(output_block(m.group(2), m.group(1) == "output!"))
        parts.append("\n".join(out).strip("\n"))
    with open(OUT_MD, "w") as fh:
        fh.write("\n\n".join(parts) + "\n")
    print("Wrote", OUT_MD)


def pdf():
    cmd = ["pandoc", OUT_MD, "-o", OUT_PDF,
           "--from=markdown+link_attributes",
           "--pdf-engine=xelatex",
           "--toc", "--toc-depth=2",
           "--top-level-division=chapter",
           "--syntax-highlighting=tango",
           "--resource-path=" + ROOT_DIR,
           "--lua-filter=" + os.path.join(ROOT_DIR, "tools", "pdf_filter.lua"),
           "-H", os.path.join(ROOT_DIR, "tools", "header.tex"),
           "-V", "documentclass=report",
           "-V", "papersize=a4",
           "-V", "fontsize=11pt",
           "-V", "geometry:margin=2.2cm",
           "-V", "mainfont=Palatino",
           "-V", "sansfont=Helvetica Neue",
           "-V", "monofont=Menlo",
           "-V", "monofontoptions=Scale=0.85",
           "-V", "colorlinks=true",
           "-V", "linkcolor=NavyBlue",
           "-V", "urlcolor=NavyBlue",
           "-V", "toccolor=black"]
    subprocess.run(cmd, check=True, cwd=ROOT_DIR)
    print("Wrote", OUT_PDF)


if __name__ == "__main__":
    assemble()
    if "--pdf" in sys.argv:
        pdf()

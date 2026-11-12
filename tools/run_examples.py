#!/usr/bin/env python3
"""Run every example in macros/ and solutions/ in batch mode.

Text output goes to outputs/<folder>/<name>.txt and every PNG a macro
creates is copied to figures/.  Usage:
    python3 tools/run_examples.py            # run everything
    python3 tools/run_examples.py ch03 ch05  # only folders matching these
"""
import os, shutil, subprocess, sys, time, glob

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PY = "/opt/homebrew/opt/python@3.14/bin/python3.14"
# Saves PNGs at 2x resolution so they look sharp in the PDF.
CPP_PRE = "gStyle->SetImageScaling(2.)"
PY_PRE = ("import ROOT; ROOT.gROOT.SetBatch(True); "
          "ROOT.gStyle.SetImageScaling(2.); import runpy, sys; "
          "sys.argv=[sys.argv[1]]; runpy.run_path(sys.argv[0], run_name='__main__')")

# Order matters: later examples read files written by earlier ones.
EXAMPLES = [
    "macros/ch01_install/check_root.C",
    "macros/ch01_install/check_pyroot.py",
    "macros/ch02_first_steps/hello.C",
    "macros/ch02_first_steps/squares.C",
    "macros/ch02_first_steps/unnamed.C",
    "macros/ch02_first_steps/cpp_basics.C",
    "macros/ch02_first_steps/first_plot.C",
    "macros/ch02_first_steps/hello_compiled.C",
    "macros/ch03_histograms/hist_basic.C",
    "macros/ch03_histograms/hist_draw_options.C",
    "macros/ch03_histograms/hist_compare.C",
    "macros/ch03_histograms/hist_2d.C",
    "macros/ch03_histograms/hist_operations.C",
    "macros/ch04_graphs_functions/functions.C",
    "macros/ch04_graphs_functions/graph_basic.C",
    "macros/ch04_graphs_functions/graph_errors.C",
    "macros/ch04_graphs_functions/multigraph.C",
    "macros/ch05_fitting/fit_gaus.C",
    "macros/ch05_fitting/fit_peak_background.C",
    "macros/ch05_fitting/fit_graph.C",
    "macros/ch05_fitting/fit_likelihood.C",
    "macros/ch06_files/file_write.C",
    "macros/ch06_files/file_read.C",
    "macros/ch07_trees/tree_write.C",
    "macros/ch07_trees/tree_inspect.C",
    "macros/ch07_trees/tree_draw.C",
    "macros/ch07_trees/tree_loop.C",
    "macros/ch07_trees/tree_reader.C",
    "macros/ch08_pyroot/pyroot_basic.py",
    "macros/ch08_pyroot/pyroot_tree.py",
    "macros/ch08_pyroot/pyroot_numpy.py",
    "macros/ch09_rdataframe/rdf_basic.py",
    "macros/ch09_rdataframe/rdf_basic_cpp.C",
    "macros/ch09_rdataframe/rdf_cutflow.py",
    "macros/ch09_rdataframe/rdf_vectors.py",
    "macros/ch09_rdataframe/rdf_snapshot.py",
    "macros/ch10_project/make_dimuon_toy.C",
    "macros/ch10_project/dimuon_analysis.py",
    "macros/ch10_project/dimuon_like_sign.py",
]
EXAMPLES += sorted(os.path.relpath(p, ROOT_DIR) for p in glob.glob(os.path.join(ROOT_DIR, "solutions/*/*.C")) +
                   glob.glob(os.path.join(ROOT_DIR, "solutions/*/*.py")))


def run(rel):
    path = os.path.join(ROOT_DIR, rel)
    folder, name = os.path.split(path)
    if not os.path.exists(path):
        print(f"  (missing) {rel}")
        return True
    before = {f: os.path.getmtime(os.path.join(folder, f))
              for f in os.listdir(folder)}
    if name.endswith(".C"):
        cmd = ["root", "-l", "-b", "-q", "-e", CPP_PRE, name]
    else:
        cmd = [PY, "-u", "-c", PY_PRE, name]
    t0 = time.time()
    res = subprocess.run(cmd, cwd=folder, capture_output=True, text=True)
    out = res.stdout + res.stderr
    sub = os.path.basename(folder)
    top = rel.split("/")[0]
    outdir = os.path.join(ROOT_DIR, "outputs", top, sub)
    os.makedirs(outdir, exist_ok=True)
    with open(os.path.join(outdir, os.path.splitext(name)[0] + ".txt"), "w") as fh:
        fh.write(out)
    figdir = os.path.join(ROOT_DIR, "figures")
    os.makedirs(figdir, exist_ok=True)
    new_png = []
    for f in os.listdir(folder):
        if f.endswith(".png"):
            p = os.path.join(folder, f)
            if f not in before or os.path.getmtime(p) > before[f]:
                shutil.copy2(p, figdir)
                new_png.append(f)
    bad = ("Error" in out or "error:" in out or "*** Break ***" in out
           or (res.returncode != 0 and name != "unnamed.C"))
    flag = "FAIL" if bad else "ok  "
    print(f"  {flag} {rel}  ({time.time()-t0:.1f}s)  png: {', '.join(new_png) or '-'}")
    return not bad


if __name__ == "__main__":
    pats = sys.argv[1:]
    todo = [e for e in EXAMPLES if not pats or any(p in e for p in pats)]
    ok = all([run(e) for e in todo])
    sys.exit(0 if ok else 1)

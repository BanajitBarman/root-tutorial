# check_pyroot.py -- check that ROOT works from Python (PyROOT)
# Run with:   python check_pyroot.py
import sys
print("Python version:", sys.version.split()[0])

import ROOT
print("ROOT version  :", ROOT.gROOT.GetVersion())

try:
    import numpy
    print("NumPy version :", numpy.__version__)
except ImportError:
    print("NumPy is missing (needed only for Chapter 8 and 9 extras)")

h = ROOT.TH1D("h", "PyROOT works!;x;Entries", 40, -4, 4)
h.FillRandom("gaus", 5000)
print("Histogram mean:", round(h.GetMean(), 3))
print("Everything is fine. You are ready for the tutorial.")

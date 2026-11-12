# 12 Where to go next

Congratulations — in two days you have learned the core of ROOT: histograms, graphs,
functions, fits, files, trees, PyROOT and RDataFrame, and you have used them for a real
analysis. Here is where to continue.

## 12.1 Documentation

| What | Where |
|------|-------|
| ROOT manual (explanations by topic) | <https://root.cern/manual/> |
| Reference guide (every class and method) | <https://root.cern/doc/master/> |
| ROOT tutorials (hundreds of small examples) | <https://root.cern/doc/master/group__Tutorials.html> |
| ROOT forum (ask questions; search first!) | <https://root-forum.cern.ch> |
| Install instructions and release notes | <https://root.cern/install/> |

> **How to ask a good question on the forum:** say which ROOT version and operating
> system you use, give a *short* macro that shows the problem (and the input file if
> needed), and copy the exact error message. Questions like this get answered quickly.

## 12.2 Topics we did not cover

- **RooFit** — a powerful library for advanced fits: unbinned likelihood fits, fits of
  several datasets together, complex signal and background models. Used in almost every
  precision measurement.
- **`TEfficiency`** — efficiencies with correct error bars.
- **`THStack`** — stacked histograms, e.g. several background sources on top of each other.
- **Styles** — `gStyle` settings and the style templates of experiments, to give all
  your plots the same look.
- **`TGraph2D`, `TF2`, `TH3`** — functions and data in more dimensions.
- **RNTuple** — the new data format that follows TTree. It is faster and smaller, and
  RDataFrame reads it with the same code.
- **Machine learning** — ROOT works with Python ML libraries (scikit-learn, PyTorch,
  XGBoost) through NumPy arrays, and has its own tools (TMVA, SOFIE).
- **Distributed analysis** — RDataFrame can run on many computers at once (with Dask or
  Spark).

## 12.3 The Python ecosystem around ROOT

The **Scikit-HEP** project (<https://scikit-hep.org>) has many Python packages for
particle physics. Some of them work with ROOT files without ROOT itself:

- **uproot** — reads and writes ROOT files with pure Python and NumPy;
- **awkward-array** — arrays with different lengths per event, like our `hitEnergy`;
- **hist** and **mplhep** — histograms and physics-style plots with matplotlib.

## 12.4 More training and data

- **HEP Software Foundation training** (<https://hsf-training.org>) — free courses on
  ROOT, Python, git, C++, and more.
- **CERN Open Data** (<https://opendata.cern.ch>) — real data from the LHC experiments,
  including ALICE and CMS, with examples. You already used one dataset in Chapter 10.
- **Your experiment's software** — the analysis frameworks of most experiments are built
  on ROOT, so everything in this manual will help you there.

Good luck with your analyses!

# 9 RDataFrame: modern analysis

`RDataFrame` is ROOT's modern, recommended way to analyse data in trees. Instead of
writing the event loop yourself, you *describe* what you want — which events to keep,
which new quantities to compute, which histograms to fill — and RDataFrame runs the
loop for you, in fast C++, and if you like on all the cores of your computer. The same
code works in Python and C++.

Examples for this chapter are in `macros/ch09_rdataframe`. They read
`../ch07_trees/toy_tree.root`, so run `tree_write.C` from Chapter 7 first.

## 9.1 The idea

An RDataFrame analysis is a chain of steps:

```text
data (tree) ──> Filter ──> Define ──> Filter ──> ... ──> results
                (keep some   (add a new                  (histograms, counts,
                 events)      column)                     means, new files)
```

- **`Filter("cut")`** keeps only the events that pass the cut.
- **`Define("name", "expression")`** adds a new column, computed from other columns.
- **Actions** produce results: `Count()`, `Mean("col")`, `Histo1D(...)`, `Snapshot(...)`.

The expressions are strings of C++ code, like the cuts in `TTree::Draw`. They are
compiled, so they are fast even when you write the analysis in Python.

## 9.2 A first example

{{code: macros/ch09_rdataframe/rdf_basic.py}}

{{output: macros/ch09_rdataframe/rdf_basic}}

![Two histograms filled by RDataFrame in a single pass over the data.](figures/rdf_basic.png)

Two things are new here.

**Lazy evaluation.** The lines that book `Count`, `Mean` and `Histo1D` do not read any
data; they just *book* (register) the results. The event loop runs only when you first
ask for a value, with `GetValue()` or by drawing a histogram. Then **all** booked results
are filled together, in **one** pass over the data — as the line
`The event loop ran 1 time(s)` shows. This is why RDataFrame is fast: book everything
first, then use the results.

> **Tip:** Avoid asking for a result in the middle of booking. If you call `GetValue()`
> on the first result before booking the others, RDataFrame has to loop over the data a
> second time for the rest.

**The histogram "model".** `Histo1D(("h_r", "title;x;y", 40, 0, 40), "r")` takes the
same arguments as `TH1D` (name, titles, bins, range) as a tuple, followed by the column
to fill. Without the model, `Histo1D("r")` chooses the binning itself.

`Display(columns, n)` prints the first `n` rows of some columns — handy for checking
that your `Define`s do what you expect.

## 9.3 The same in C++

RDataFrame looks almost the same in C++:

{{code: macros/ch09_rdataframe/rdf_basic_cpp.C}}

{{output: macros/ch09_rdataframe/rdf_basic_cpp}}

![The φ angle of the impact point, for events above 30 GeV.](figures/rdf_basic_cpp.png)

Differences from Python:

- the histogram model is written with braces: `{"hPhi", "title", 40, -4, 4}`;
- a result is used with `*`: `*nSel` is the number, and `hPhi->` calls methods of the
  histogram;
- `Define` can also take a C++ function (here a *lambda*, an unnamed function written
  in place) and the list of columns it needs. This is useful for longer calculations;
- the histograms belong to the dataframe and are deleted when the macro ends, so we draw
  a copy with `DrawClone()`.

## 9.4 Cuts and the cut-flow report

Give each `Filter` a name as its second argument. `Report()` then tells you how many
events passed each cut:

{{code: macros/ch09_rdataframe/rdf_cutflow.py}}

{{output: macros/ch09_rdataframe/rdf_cutflow}}

`eff` is the fraction of events that pass this cut (out of those that passed the
previous ones), and `cumulative eff` is the fraction that pass all cuts so far. A table
like this is part of every physics analysis.

## 9.5 Array columns (RVec)

In RDataFrame, an array branch like `hitEnergy` appears as an **RVec**: an array that
supports calculations on all elements at once, without writing a loop:

{{code: macros/ch09_rdataframe/rdf_vectors.py}}

{{output: macros/ch09_rdataframe/rdf_vectors}}

![Left: the number of hits above 2 GeV per event. Right: the energy of every hit.](figures/rdf_vectors.png)

The most useful operations on an RVec `v`:

| Expression | Result |
|------------|--------|
| `v.size()` | number of elements |
| `v > 2.0` | an array of true/false, one per element |
| `v[v > 2.0]` | only the elements above 2.0 |
| `Sum(v)`, `Mean(v)`, `Max(v)`, `Min(v)` | one number per event |
| `Sum(v > 2.0)` | how many elements are above 2.0 |
| `ArgMax(v)` | index of the largest element |
| `Sort(v)`, `Reverse(v)`, `Take(v, n)` | sorted, reversed, first n elements |
| `v[0]` | the first element (check that `v.size() > 0` first!) |
| `InvariantMass(pt, eta, phi, m)` | invariant mass of the particles (Chapter 10) |
| `DeltaR(eta1, eta2, phi1, phi2)` | distance in (η, φ) between particles |

These functions are part of `ROOT::VecOps` and can be used directly in the strings.
`Histo1D` of an RVec column fills one entry per element.

## 9.6 Saving results: Snapshot, NumPy and many cores

{{code: macros/ch09_rdataframe/rdf_snapshot.py}}

{{output: macros/ch09_rdataframe/rdf_snapshot}}

- **`Snapshot("tree", "file.root", [columns])`** writes the events that pass your
  filters, with the columns you choose (including `Define`d ones), to a new file. This
  is called a *skim*: a smaller file with only what you need, which is much faster to
  analyse again and again.
- **`AsNumpy([columns])`** gives the columns as a Python dictionary of NumPy arrays,
  ready for NumPy, pandas or matplotlib.
- **`ROOT.EnableImplicitMT()`** makes RDataFrame use all the cores of your computer. It
  must be called *before* the RDataFrame is created. With more than one thread, the
  order of the events is not guaranteed, so do not rely on it.

> **Note:** `df.Range(n)`, which processes only the first `n` events, does not work
> together with `EnableImplicitMT()`.

## 9.7 Other useful actions

| Action | Result |
|--------|--------|
| `Count()` | number of events |
| `Sum("x")`, `Mean("x")`, `Min("x")`, `Max("x")` | one number |
| `Stats("x")` | mean, RMS, minimum and maximum together |
| `Histo1D(model, "x")`, `Histo2D(model, "x", "y")` | histograms |
| `Profile1D(model, "x", "y")` | mean of y in bins of x |
| `Graph("x", "y")` | a TGraph |
| `Take["double"]("x")` | all values in a list |
| `AsNumpy(["x", "y"])` | NumPy arrays (Python only) |
| `Snapshot("tree", "file.root", ["x"])` | write a new file |
| `Report()`, `Display(["x"], n)` | cut flow, first rows |

A dataframe can read many files at once, with wildcards or a list:
`ROOT.RDataFrame("events", "data/run*.root")`. It can even read files over the network,
as you will see in Chapter 10.

## Key points

- `ROOT.RDataFrame("tree", "file.root")` opens the data; `Filter` keeps events;
  `Define` adds columns; actions (`Count`, `Histo1D`, ...) give results.
- Results are lazy: book everything first; the loop runs once, when a value is first used.
- Name your filters and use `Report()` for the cut flow.
- Array columns are RVecs: `Sum(v > 2)`, `v[v > 2]`, `Max(v)` work without loops.
- `Snapshot` writes a skim; `AsNumpy` gives NumPy arrays; `EnableImplicitMT()` uses all cores.

## Exercises

**Exercise 9.1 — A small analysis.** With RDataFrame and `toy_tree.root`:

1. Define the distance `r = sqrt(x*x + y*y)`.
2. Select events with energy above 30 GeV, and then with at least 10 hits, using two
   *named* filters.
3. Make a histogram of `r` for the selected events and save it as a PNG.
4. Print the mean of `r` for the selected events and the cut-flow report.
5. Write the selected events, with only the columns `energy` and `r`, to a file
   `selected.root`. Check the new file with `rootls -t selected.root`.

Solutions are in Appendix A and in `solutions/ch09`.

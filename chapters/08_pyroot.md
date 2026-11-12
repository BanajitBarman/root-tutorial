# 8 PyROOT: ROOT from Python

Everything you have done in C++ can also be done in Python, with exactly the same
classes and methods. This is called **PyROOT**. Python is popular because it is easy to
read and write, and because you can combine ROOT with Python's huge collection of
scientific packages (NumPy, pandas, matplotlib, scikit-learn, ...).

Examples for this chapter are in `macros/ch08_pyroot`. Some of them read
`../ch07_trees/toy_tree.root`, so run `tree_write.C` from Chapter 7 first.

## 8.1 From C++ to Python

You start with `import ROOT`. After that, every ROOT class and function is available as
`ROOT.something`. To translate a C++ macro into Python, follow these rules:

| C++ | Python |
|-----|--------|
| `auto h = new TH1D("h", "t", 10, 0, 1);` | `h = ROOT.TH1D("h", "t", 10, 0, 1)` |
| `h->Fill(x);` | `h.Fill(x)` |
| `h->GetXaxis()->SetTitle("x");` | `h.GetXaxis().SetTitle("x")` |
| `gStyle->SetOptStat(0);` | `ROOT.gStyle.SetOptStat(0)` |
| `TMath::Pi()` | `ROOT.TMath.Pi()` |
| `kRed + 1` | `ROOT.kRed + 1` |
| `true`, `false`, `nullptr` | `True`, `False`, `ROOT.nullptr` |
| `for (int i = 0; i < n; i++) { ... }` | `for i in range(n):` (and indent the block) |
| `std::cout << x << std::endl;` | `print(x)` |
| `// comment` | `# comment` |

In short: no `new`, no semicolons, no types, and `.` instead of `->` and `::`.

## 8.2 Your first PyROOT script

Here is the Gaussian fit of Section 5.2, in Python:

{{code: macros/ch08_pyroot/pyroot_basic.py}}

```bash
python pyroot_basic.py
```

{{output: macros/ch08_pyroot/pyroot_basic}}

![The Gaussian fit made from Python: the same result as in C++.](figures/pyroot_basic.png)

> **The window closes at once?** When a Python script ends, Python quits, and the
> canvas window closes with it. To keep it open, run the script with
> `python -i pyroot_basic.py`. The `-i` means "stay in interactive mode after the
> script"; type `exit()` or press Ctrl+D to quit. The PNG file is saved in any case.

## 8.3 Object lifetimes in Python

In C++ macros we used `new` so that objects stay alive. In Python, an object lives as
long as **some variable refers to it**. Inside a function, local variables disappear when
the function ends — and so does their canvas:

```python
def make_plot():
    c = ROOT.TCanvas("c", "", 800, 600)
    h = ROOT.TH1D("h", "", 10, 0, 1)
    h.Draw()
    # function ends: c and h are deleted, the window disappears

def make_plot_that_stays():
    c = ROOT.TCanvas("c", "", 800, 600)
    h = ROOT.TH1D("h", "", 10, 0, 1)
    h.Draw()
    return c, h          # give them back to the caller, who keeps them

c, h = make_plot_that_stays()
```

Code at the top level of a script (not inside a function), as in our examples, does not
have this problem.

## 8.4 Reading a tree in Python

Branches become attributes of the tree, so a loop over events is very short:

{{code: macros/ch08_pyroot/pyroot_tree.py}}

{{output: macros/ch08_pyroot/pyroot_tree}}

This is easy to write, but **slow**: a Python loop over millions of events can take
minutes where C++ takes seconds. For real analyses in Python, use `RDataFrame`
(Chapter 9), which runs the loop in fast C++ for you.

## 8.5 ROOT and NumPy

PyROOT works together with NumPy arrays:

{{code: macros/ch08_pyroot/pyroot_numpy.py}}

{{output: macros/ch08_pyroot/pyroot_numpy}}

![A histogram filled from a NumPy array and fitted with an exponential.](figures/pyroot_numpy.png)

- `h.Fill(array)` fills a whole NumPy array at once (fast).
- `h.values()` gives the bin contents as a NumPy array (without under- and overflow),
  and `h.variances()` gives the squared errors. You can then use them with matplotlib or
  any other Python tool.
- Chapter 9 shows how to get tree columns as NumPy arrays (`AsNumpy`).

## 8.6 Using C++ from Python

You can mix the two languages. `ROOT.gInterpreter.Declare` compiles a piece of C++, and
the result can be called from Python:

```python
import ROOT

ROOT.gInterpreter.Declare("""
double kinetic(double m, double v) { return 0.5 * m * v * v; }
""")
print(ROOT.kinetic(2.0, 3.0))       # prints 9.0

# Load an existing C++ macro and call its function
ROOT.gROOT.ProcessLine(".L ../ch02_first_steps/squares.C")
ROOT.squares(2)
```

This is useful when you have a fast C++ function (or an existing macro) that you want
to use in a Python analysis.

## 8.7 Jupyter notebooks

A Jupyter notebook mixes code, results, plots and text in one document in your web
browser. It is very convenient for exploring data and for sharing an analysis. If you
installed ROOT with conda (Section 1.3), Jupyter is already there. Start it from the
folder with your files:

```bash
conda activate root-tutorial
cd root-tutorial/macros/ch08_pyroot
jupyter lab
```

A browser window opens. Click *Python 3* under *Notebook* to make a new notebook, and
type code into the cells. Press **Shift+Enter** to run a cell.

```python
import ROOT
%jsroot on

h = ROOT.TH1D("h", "Notebook test;x;Entries", 50, -4, 4)
h.FillRandom("gaus", 10000)
c = ROOT.TCanvas()
h.Draw()
c.Draw()        # show the canvas in the notebook
```

- `c.Draw()` at the end of a cell shows the canvas in the notebook.
- `%jsroot on` makes the plots interactive (zoom, move the mouse over bins, right-click
  menus), using ROOT's JavaScript graphics. Without it you get static pictures.
- A cell that starts with `%%cpp` contains C++ instead of Python, so you can mix both.

You can also start a notebook that uses C++ in every cell with `root --notebook`.

## Key points

- `import ROOT`, then use `ROOT.TH1D`, `ROOT.TCanvas`, ... with `.` instead of `->`.
- Run scripts with `python script.py`, or `python -i script.py` to keep windows open.
- Python objects live as long as a variable refers to them — return canvases from functions.
- Python loops over events are slow; use RDataFrame for large data.
- `h.Fill(numpy_array)` and `h.values()` connect ROOT histograms and NumPy.
- In Jupyter: `%jsroot on` for interactive plots, `c.Draw()` to show a canvas.

## Exercises

**Exercise 8.1 — The muon lifetime in Python.** Redo Exercise 5.2 in Python. This time,
generate the 5000 decay times with NumPy
(`numpy.random.default_rng().exponential(2.197, size=5000)`), fill them into the histogram
with a single `Fill` call, and fit with `expo`. Print the lifetime with its error.
If you have Jupyter, try it in a notebook too.

Solutions are in Appendix A and in `solutions/ch08`.

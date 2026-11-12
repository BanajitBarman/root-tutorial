# 11 Common mistakes and how to fix them

Everybody makes these mistakes when starting with ROOT. The messages below are copied
from real ROOT sessions, so you can recognise them when they appear on your screen.

## 11.1 How to read an error message

- **Read the first error, not the last.** One mistake often causes many messages; the
  first one is usually the real problem.
- **Look for the file name and line number.** `semicolon.C:3:13: error: ...` means: file
  `semicolon.C`, line 3, column 13.
- **A crash prints a long list** starting with `*** Break *** segmentation violation`,
  followed by many lines of computer addresses (the *stack trace*). You can usually
  ignore the list itself. Look at the messages printed *just before* the crash, and think
  about which pointer could be `nullptr`.

## 11.2 Macros

**The macro does nothing.**

```text
Processing myplot.C...
warning: Failed to call `myplot()` to execute the macro.
Add this function or rename the macro. Falling back to `.L`.
```

The file `myplot.C` contains a function with a different name (here `plot()`). The
function name must be the same as the file name. Rename one of them.

**A missing semicolon.**

```text
semicolon.C:3:13: error: expected ';' at end of declaration
   int n = 5
            ^
            ;
```

The compiler even shows you where the semicolon should go.

**Wrong numbers from a division.** `int a = 7, b = 2; double r = a / b;` gives
`r = 3`, not 3.5, because `a / b` is computed with integers before it is stored. Write
`double(a) / b` or use `double` variables. This mistake gives **no error message** —
you just get wrong results, so it is worth remembering.

**Running a macro twice in the same session.**

```text
Warning in <TROOT::Append>: Replacing existing TH1: h (Potential memory leak).
```

You created a second histogram called `h` while the first still exists. This is only a
warning and is harmless in small macros. If you want to avoid it, quit and restart ROOT,
or give your histograms different names.

## 11.3 Plots

**My plot disappears when the macro ends.** You created the object without `new`
(`TH1D h(...)` or `TCanvas c(...)`), so it was deleted at the end of the macro. Use
`auto h = new TH1D(...)` (Section 2.6). In Python, keep a variable that refers to it
(Section 8.3).

**The second histogram is cut off at the top.** The first histogram drawn decides the y
range. Call `SetMaximum` on the first histogram (Section 3.6).

**My graph does not appear.** The first graph on a pad needs the `A` option to draw the
axes: `g->Draw("AP")`, not `g->Draw("P")`.

**My histogram is empty, but I filled it.** Your values are outside the range
`xmin`–`xmax`, so they all went into the underflow or overflow bins. Compare
`h->GetEntries()` (counts everything) with `h->Integral()` (counts only the visible
bins), and fix the range.

**Everything is drawn on top of each other.** You forgot to make a new canvas (or to
`cd()` to a different pad) before drawing the next thing. Without `"SAME"`, `Draw`
replaces the content of the *current* pad.

**Python: the window closes immediately.** Run the script with `python -i script.py`
(Section 8.2).

## 11.4 Files

**The file does not exist.**

```text
Error in <TFile::TFile>: file /home/me/root-tutorial/macros/ch06_files/my_resluts.root does not exist

 *** Break *** segmentation violation
```

The file name is wrong (here: `resluts`), or you are running the macro from a different
folder. ROOT tells you the full path where it looked. The crash afterwards happens
because the macro then uses the `nullptr` file. Check the pointer:

```cpp
TFile *f = TFile::Open("my_results.root");
if (!f || f->IsZombie()) { std::cout << "Cannot open file" << std::endl; return; }
```

**The object is not in the file.**

```text
h = 0x0

 *** Break *** segmentation violation
```

`f->Get<TH1D>("hEnergyy")` found no object with that name and returned `nullptr`
(printed as `0x0`). Check the names with `f->ls()` or `rootls -l file.root`, and remember
the folder: `"calibration/hGain"`.

**My histogram vanished after I closed the file.** The file owned it and deleted it.
Use `h->SetDirectory(nullptr)` before closing (Section 6.4).

## 11.5 Trees

**Wrong variable type.**

```text
Error in <TTree::SetBranchAddress>: The pointer type given "Float_t" (5) does not
correspond to the type needed "Double_t" (8) by the branch: energy
```

The variable must have exactly the type of the branch. Check with `tree->Print()`.
With `TTreeReader`:

```text
Error in <TTreeReaderValueBase::CreateProxy()>: The branch energy contains data of
type double. It cannot be accessed by a TTreeReaderValue<float>
```

**`Scan` shows fewer rows than expected.** The last argument of `Scan` is the number of
entries to look at, not the number of rows to print (Section 7.3).

## 11.6 RDataFrame and Python

**A misspelled column name.**

```text
cppyy.gbl.std.runtime_error: Template method resolution failed:
  ...
    runtime_error: Unknown column: "enrgy"
```

In Python, the message is long, but the last line tells you what is wrong. Check the
column names with `df.GetColumnNames()`.

**ROOT and Python versions do not match.**

```text
ImportError:
ROOT was built for Python 3.14.7, but you are running Python 3.9.6.
Python major.minor versions must match. Use a matching Python or ROOT build.
```

Use the Python that ROOT was built with (Sections 1.3 and 1.5). With conda, make sure
the environment is activated.

**A Filter that is not a cut.**

```text
runtime_error: Filter: the following expression does not evaluate to bool:
1
```

A `Filter` must be a true/false condition. To keep all events, write `"true"`.

## 11.7 Fits

**The fit curve is far away from the data**, or the printout contains `FAILED`,
`NOT POSDEF` or `CALL LIMIT`. Go through the checklist in Section 5.8: starting values,
fit range, parameter limits, fitting in steps.

**The fit looks fine but χ²/ndf is huge.** Either the function does not describe the
data (look for a structure that the function misses), or the errors are too small. With
weighted histograms, make sure you called `Sumw2()`.

**The fit is wrong when there are few entries.** Use the likelihood option `"L"`
(Section 5.7).

## 11.8 Everything is slow

- **Python loops over events are slow.** Use RDataFrame, which runs the loop in C++.
- **Interpreted C++ loops** can be made faster by compiling the macro: `.x macro.C+`.
- **Use all your cores** with `ROOT.EnableImplicitMT()` in RDataFrame.
- **Read only what you need.** RDataFrame and `TTree::Draw` read only the branches you
  use. In your own loops, use `TTreeReader`, which does the same.
- **Make a skim** of the events and columns you need with `Snapshot` (Section 9.6), and
  run your analysis on the smaller file.

# 6 ROOT files

So far, everything we made disappeared when we quit ROOT. To keep results, or to share
them, you save them in a **ROOT file** (extension `.root`). Experiments store all their
data in ROOT files, so reading them is an essential skill.

Examples for this chapter are in `macros/ch06_files`.

## 6.1 What is in a ROOT file?

A ROOT file can hold any ROOT object: histograms, graphs, functions, canvases, and the
data *trees* of Chapter 7. Inside, it is organised like a disk with folders (called
*directories*). ROOT files are:

- **compressed** — they take less space than plain text;
- **self-describing** — the file knows what kind of objects are inside, so any ROOT
  program (C++ or Python) can read it, years later;
- **fast** to read partially — you can read one histogram, or one column of a huge
  table, without reading the whole file.

## 6.2 Writing a file

{{code: macros/ch06_files/file_write.C}}

{{output: macros/ch06_files/file_write}}

**Opening.** `TFile::Open(name, mode)` opens a file. The mode says what you want to do:

| Mode | Meaning |
|------|---------|
| `"READ"` | read only (the default). The file must exist. |
| `"RECREATE"` | create a new file; if it exists, **overwrite** it |
| `"NEW"` or `"CREATE"` | create a new file; fail if it already exists |
| `"UPDATE"` | open an existing file to add or change objects |

**Who owns a histogram?** When a file is open, every histogram you create *belongs to*
the file's current directory. This is why `file->Write()` writes all of them in one go,
and why `file->Close()` **deletes them from memory**: the file cleans up what it owns.
Graphs, functions and canvases do not belong to the file, so we write them ourselves with
`object->Write("name")`.

**Folders.** `file->mkdir("calibration")` creates a folder in the file and returns it;
`calib->cd()` makes it the current directory, so new histograms go there. `file->cd()`
goes back to the top.

## 6.3 Reading a file

{{code: macros/ch06_files/file_read.C}}

{{output: macros/ch06_files/file_read}}

![Objects read back from `my_results.root`.](figures/file_read.png)

**Listing.** `file->ls()` prints the contents. Each `KEY` line shows the class of the
object, its name, a *cycle number* after the `;`, and its title. The cycle number counts
how many times an object with that name was written; normally you can ignore it, and
ROOT always gives you the highest one.

**Getting objects.** `file->Get<TH1D>("hEnergy")` reads the object called `hEnergy` and
gives you a pointer of type `TH1D*`. Objects in folders are found with a path:
`"calibration/hGain"`.

> **Always check the pointer!** If the name is wrong (or the file is wrong), `Get` returns
> `nullptr`, and using it crashes ROOT with a scary `*** Break *** segmentation violation`
> message. A simple `if (!h) { ... }` check saves you a lot of confusion. The same is
> true for `TFile::Open`: check that the file opened.

## 6.4 Keeping a histogram after closing the file

Histograms you read from a file also belong to that file. If you close the file, they
are deleted. To keep a histogram, take it away from the file first:

```cpp
TFile *f = TFile::Open("my_results.root");
auto h = f->Get<TH1D>("hEnergy");
h->SetDirectory(nullptr);   // h now belongs to you, not to the file
f->Close();                 // h is still there
h->Draw();
```

This is why `file_read.C` does not close the file. When ROOT quits, it closes all files
anyway.

## 6.5 Files at the ROOT prompt

You can open a file directly when starting ROOT. ROOT calls it `_file0` (the next one
`_file1`, ...), and you can use the objects in it just by typing their names:

```bash
root -l my_results.root
```

```text
Attaching file my_results.root as _file0...
(TFile *) 0x125d7bda0
root [1] .ls
TFile**         my_results.root
 TFile*         my_results.root
  KEY: TDirectoryFile   calibration;1   calibration
  KEY: TGraph   gResponse;1     Detector response
  KEY: TH1D     hEnergy;1       Deposited energy
  KEY: TH1D     hAngle;1        Polar angle
root [2] hEnergy->GetMean()
(double) 29.493118
root [3] hEnergy->Draw()
```

This "type the name" trick works only at the prompt, not in macros.

## 6.6 Looking inside files with the browser

The quickest way to see what is in a file is the browser:

```bash
rootbrowse my_results.root
```

Click on the file, open the folders, and double-click a histogram to draw it. This is
very useful for files you get from someone else.

## 6.7 Command-line tools

ROOT comes with a few small programs that you run directly in the terminal.

**`rootls`** lists a file, like `ls` for folders:

```bash
rootls my_results.root
rootls -l my_results.root               # long listing with class names and titles
rootls -l my_results.root:calibration   # look inside a folder
```

```text
calibration  gResponse  hAngle  hEnergy

TDirectoryFile  Nov 04 15:1 2026 calibration;1 "calibration"
TGraph          Nov 04 15:1 2026 gResponse;1   "Detector response"
TH1D            Nov 04 15:1 2026 hAngle;1      "Polar angle"
TH1D            Nov 04 15:1 2026 hEnergy;1     "Deposited energy"

TH1D  Nov 04 15:1 2026 hGain;1 "Channel gains"
```

`rootls -t file.root` also shows the structure of trees (Chapter 7).

**`hadd`** adds up files. Histograms with the same name are added bin by bin, and trees
are joined one after another. This is how you combine the results of many jobs, for
example one file per run:

```bash
hadd merged.root run1.root run2.root        # or: hadd merged.root run*.root
```

```text
Info in <hadd>: target file: merged.root
Info in <hadd>: compression setting for all output: 101
hadd Source file 1: run1.root
hadd Source file 2: run2.root
hadd Target path: merged.root:/
hadd Target path: merged.root:/calibration
```

If both input files have 10 000 entries in `hEnergy`, the merged `hEnergy` has 20 000.
Use `hadd -f` to overwrite an existing output file.

**`rootcp`** copies objects between files, and **`rooteventselector`** copies part of a
tree. Run any of them with `--help` to see their options.

## Key points

- `TFile::Open("f.root", "RECREATE")` creates a file; `TFile::Open("f.root")` reads one.
- Histograms created while a file is open belong to it: `file->Write()` saves them and
  `file->Close()` deletes them from memory.
- `file->Get<TH1D>("name")` reads an object; check that the result is not `nullptr`.
- `h->SetDirectory(nullptr)` keeps a histogram alive after the file is closed.
- `rootls`, `rootbrowse` and `hadd` are handy command-line tools.

## Exercises

**Exercise 6.1 — Save and read back.** Take your solution of Exercise 3.1 (the central
limit theorem). Change it so that the four histograms are written into a folder called
`sums` in a file `clt.root`. Then, in the same macro, close the file, open it again for
reading, list its contents with `ls()`, read back the histogram for the sum of twelve
numbers and draw it. Check: its mean should be 6 and its standard deviation 1.
Finally, look at `clt.root` with `rootls -l clt.root:sums` and with `rootbrowse`.

Solutions are in Appendix A and in `solutions/ch06`.

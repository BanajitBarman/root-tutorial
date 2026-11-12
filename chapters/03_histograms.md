# 3 Histograms

The histogram is the most important object in data analysis. In this chapter you learn
to create, fill, draw, style and combine histograms, in one and two dimensions.

Examples for this chapter are in `macros/ch03_histograms`.

## 3.1 What is a histogram?

A histogram counts how many values fall into each of a set of intervals, called
**bins**. For example, if you measure the energy of 10 000 particles, a histogram with
100 bins from 0 to 100 GeV tells you how many particles had 0–1 GeV, how many had
1–2 GeV, and so on. The shape of the histogram shows the *distribution* of the values.

ROOT has histogram classes for 1, 2 and 3 dimensions. The last letter gives the type
used to store the bin contents:

| Class | Dimensions | Bin content stored as |
|-------|------------|-----------------------|
| `TH1D`, `TH1F`, `TH1I` | 1 | double, float, int |
| `TH2D`, `TH2F`         | 2 | double, float |
| `TH3D`, `TH3F`         | 3 | double, float |

Use `TH1D` (and `TH2D`) unless you have a reason not to.

## 3.2 Create, fill, draw

{{code: macros/ch03_histograms/hist_basic.C}}

```bash
root -l hist_basic.C
```

{{output: macros/ch03_histograms/hist_basic}}

![The output of `hist_basic.C`: 10 000 Gaussian random numbers in 50 bins.](figures/hist_basic.png)

Let us go through it.

**Creating.** `new TH1D("h", "Gaussian random numbers;x;Entries", 50, -5, 5)` takes:

1. a **name** (`"h"`) — ROOT uses it to find the object, for example in a file. Every
   histogram should have a different name;
2. the **titles**: plot title, x-axis title and y-axis title, separated by `;`;
3. the **number of bins** (50);
4. the **lower and upper edge** of the x axis (−5 and 5). Each bin is
   (5 − (−5)) / 50 = 0.2 wide.

**Filling.** `h->Fill(x)` adds 1 to the bin that contains `x`. `TRandom3` is ROOT's
random-number generator; `rng.Gaus(0, 1)` returns a random number from a Gaussian
distribution with mean 0 and standard deviation 1. The number in `TRandom3 rng(42)` is
the *seed*: the same seed always gives the same random numbers, so you get exactly the
same plot as in this manual.

**Drawing.** `Draw()` draws the histogram on the current canvas. The box in the top
right corner is the **statistics box**: the number of entries, the mean and the
standard deviation (*Std Dev*, often called RMS in ROOT) of the filled values.

## 3.3 Bins and their contents

The bins are numbered **from 1 to N** (here N = 50). Bin 0 is the *underflow* bin: it
counts values below the lower edge. Bin N+1 is the *overflow* bin: it counts values
above the upper edge. They are not drawn, but they are counted in the entries.

| Method | What it gives |
|--------|---------------|
| `h->GetNbinsX()` | number of bins N |
| `h->FindBin(x)` | number of the bin that contains `x` |
| `h->GetBinContent(i)` | content of bin `i` |
| `h->GetBinError(i)` | error of bin `i` (√content for a normal histogram) |
| `h->GetBinCenter(i)`, `h->GetBinLowEdge(i)`, `h->GetBinWidth(i)` | position of bin `i` |
| `h->SetBinContent(i, value)` | set the content of bin `i` yourself |
| `h->GetEntries()` | how many times `Fill` was called |
| `h->Integral()` | sum of the bin contents (without under/overflow) |
| `h->GetMean()`, `h->GetStdDev()` | mean and standard deviation |
| `h->GetMaximum()`, `h->GetMaximumBin()` | largest bin content, and which bin it is |

## 3.4 Draw options

`Draw` takes an *option* string that controls how the histogram looks:

{{code: macros/ch03_histograms/hist_draw_options.C}}

![The same histogram drawn with four different options.](figures/hist_draw_options.png)

`c->Divide(2, 2)` splits the canvas into 2 × 2 *pads*, and `c->cd(1)` makes pad 1 the
current one, so the next `Draw` goes there. The most useful options:

| Option | Meaning |
|--------|---------|
| (none) or `HIST` | bars outline (with `HIST`, error bars are never drawn) |
| `E`, `E1` | points with error bars (`E1` adds small lines at the ends) |
| `P` | markers only |
| `L`, `C` | a line or a smooth curve through the bin contents |
| `BAR` | bars with gaps |
| `TEXT` | write the bin contents as numbers |
| `SAME` | draw on top of what is already on the pad (do not clear it) |

Options can be combined, for example `"E1 SAME"` or `"HIST TEXT"`. For 2D histograms
there are more options (Section 3.9). The full list is on the `THistPainter` page of the
reference guide: <https://root.cern/doc/master/classTHistPainter.html>.

## 3.5 Making plots look good

**Colours.** ROOT has named colours: `kBlack`, `kRed`, `kBlue`, `kGreen`, `kOrange`,
`kMagenta`, `kCyan`, `kAzure`, `kViolet`, `kPink`, `kGray`, ... Add or subtract a
number for lighter or darker shades: `kBlue + 1` is darker blue, `kAzure - 9` is a very
light blue. `SetFillColorAlpha(kRed, 0.2)` gives a transparent fill.

**Lines, fills and markers.**

| Method | Examples |
|--------|----------|
| `SetLineColor(c)`, `SetLineWidth(w)`, `SetLineStyle(s)` | style 1 = solid, 2 = dashed, 3 = dotted |
| `SetFillColor(c)`, `SetFillStyle(s)` | style 1001 = solid, 3004 = hatched, 0 = none |
| `SetMarkerStyle(m)`, `SetMarkerColor(c)`, `SetMarkerSize(s)` | 20 = filled circle, 21 = filled square, 24 = open circle |

**The statistics box.** `gStyle->SetOptStat(0)` hides it for all plots;
`h->SetStats(0)` hides it for one histogram. `gStyle->SetOptStat("emr")` shows only
entries (e), mean (m) and RMS/std dev (r). `gStyle` holds the global style settings.

**Axis ranges.** `h->SetMinimum(0)` and `h->SetMaximum(500)` fix the y range.
`h->GetXaxis()->SetRangeUser(-2, 2)` zooms the x axis. `gPad->SetLogy()` makes the
y axis logarithmic (useful for spectra that fall over many orders of magnitude).

**Text.** Titles and legends understand a LaTeX-like syntax, with `#` instead of `\`:
`#sigma`, `#mu`, `#pi`, `#sqrt{x}`, `x^{2}`, `p_{T}`, `#pm`, `#rightarrow`,
`#bar{p}`. For example, `"m_{#mu#mu} (GeV/c^{2})"` writes the letter m with the
subscript μμ, followed by (GeV/c²).

## 3.6 Several histograms on one plot

To compare two distributions, draw the second one with the `SAME` option and add a
legend:

{{code: macros/ch03_histograms/hist_compare.C}}

![Two distributions with different numbers of entries, normalised to the same area.](figures/hist_compare.png)

Three things to notice:

1. **Normalising.** The two histograms have 10 000 and 5 000 entries. To compare their
   *shapes*, we divide each one by its integral with `Scale(1.0 / h->Integral())`, so
   both have area 1.
2. **The first histogram drawn sets the y range.** If the second one is taller, its top
   is cut off. That is why we call `SetMaximum` on the first one, with some room to spare.
3. **The legend.** `new TLegend(x1, y1, x2, y2)` takes the corners as *fractions* of the
   pad (0 = left/bottom, 1 = right/top). The last argument of `AddEntry` says what to
   show next to the label: `"l"` line, `"f"` fill, `"p"` marker, `"e"` error bar,
   or combinations like `"lf"` and `"pe"`.

## 3.7 Histogram arithmetic

Histograms can be added, scaled, divided and rebinned. A typical example is an
**efficiency**: the fraction of particles that a detector sees, as a function of energy.

{{code: macros/ch03_histograms/hist_operations.C}}

{{output: macros/ch03_histograms/hist_operations}}

![Detection efficiency, computed by dividing two histograms, compared with the true efficiency.](figures/hist_operations.png)

The most useful operations:

| Method | What it does |
|--------|--------------|
| `h->Sumw2()` | store the bin errors properly; call it before filling if you will scale, add or divide |
| `h->Clone("newName")` | make a copy with a new name |
| `h->Add(h2)`, `h->Add(h2, -1)` | add or subtract `h2`, bin by bin |
| `h->Scale(c)` | multiply every bin (and its error) by `c` |
| `h->Divide(h1, h2, 1, 1, "B")` | `h = h1 / h2` bin by bin, with binomial errors |
| `h->Multiply(h2)` | multiply bin by bin |
| `h->Rebin(n, "newName")` | merge every `n` neighbouring bins |
| `h->Integral(b1, b2)` | sum of the bins from `b1` to `b2` |

> **Tip:** For efficiencies, ROOT also has a dedicated class, `TEfficiency`, which
> gives better error bars when the efficiency is close to 0 or 1. Look it up once you
> are comfortable with histograms.

## 3.8 Several pads on one canvas

You have already seen `c->Divide(2, 2)` and `c->cd(n)`. A few more details:

- Pads are numbered from 1, left to right and then top to bottom.
- `gPad` always points to the current pad, so `gPad->SetLogy()` makes the current pad
  logarithmic, and `gPad->SetGrid()` adds a grid.
- `c->Divide(1, 2)` gives two pads one above the other.

## 3.9 Two-dimensional histograms

A 2D histogram counts pairs of values (x, y). It shows whether two quantities are
related (*correlated*).

{{code: macros/ch03_histograms/hist_2d.C}}

{{output: macros/ch03_histograms/hist_2d}}

![A 2D histogram drawn with the options COLZ (left) and LEGO2 (right).](figures/hist_2d.png)

The tilted shape shows that y tends to be large when x is large: the two variables are
correlated, with a correlation factor of 0.6. Useful 2D draw options:

| Option | Meaning |
|--------|---------|
| `COLZ` | colours for the bin contents, with a colour scale (Z) — the most common |
| `BOX` | boxes whose size shows the content |
| `CONT` | contour lines |
| `LEGO`, `LEGO2` | 3D bars |
| `SURF`, `SURF2` | 3D surface |

`ProjectionX()` and `ProjectionY()` add up a 2D histogram along one axis to make a 1D
histogram. `ProfileX()` makes a *profile*: the mean y value in each x bin, with its
error — a quick way to see how y depends on x.

## 3.10 Random numbers

`TRandom3` is the generator to use. Its most useful methods:

| Method | Distribution |
|--------|--------------|
| `Uniform(a, b)` | uniform between a and b (`Uniform()` is between 0 and 1) |
| `Gaus(mean, sigma)` | Gaussian (normal) |
| `Exp(tau)` | exponential with mean tau (like decay times) |
| `Poisson(mean)` | Poisson (counting experiments) |
| `Landau(mpv, sigma)` | Landau (energy loss in thin detectors) |
| `Integer(n)` | a random integer from 0 to n−1 |

`TRandom3 rng(0)` uses a different seed every time you run, so you get different
numbers each time. A histogram can also fill itself from a function:
`h->FillRandom("gaus", 1000)` fills 1000 values distributed like a Gaussian.

## Key points

- `new TH1D(name, "title;x title;y title", nbins, xmin, xmax)` creates a histogram;
  `Fill(x)` fills it; `Draw()` draws it.
- Bins are numbered 1 to N; 0 and N+1 are underflow and overflow.
- `Draw("E")` for error bars, `Draw("SAME")` to draw on top, `Draw("COLZ")` for 2D.
- To compare shapes, normalise with `Scale(1.0 / h->Integral())`.
- Use `Sumw2()` before scaling, adding or dividing, so the errors are right.
- `TLegend`, `SetLineColor`, `SetMarkerStyle`, `gStyle->SetOptStat(0)` make plots readable.

## Exercises

**Exercise 3.1 — The central limit theorem.** Make four histograms, in four pads of one
canvas. Fill the first with 100 000 values of `rng.Uniform(0, 1)`. Fill the second with
100 000 values of the *sum of two* uniform random numbers, the third with sums of three,
and the fourth with sums of twelve. What happens to the shape? Fit the last three with
a Gaussian (`h->Fit("gaus")`, see Chapter 5) to see how close they are.

**Exercise 3.2 — Calculating π with random numbers.** Generate one million random
points (x, y), with x and y uniform between −1 and 1. Count how many fall inside the
circle x² + y² < 1. The fraction inside is π/4 (the area of the circle divided by the
area of the square), so π ≈ 4 × (inside / total). Print your estimate.
Also fill a histogram of x² + y² for all points. Can you explain its shape?
*Bonus:* draw the first 5000 points, inside in blue and outside in red. (Hint: use two
`TGraph` objects, from Chapter 4.)

Solutions are in Appendix A and in `solutions/ch03`.

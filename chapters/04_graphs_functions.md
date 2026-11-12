# 4 Graphs and functions

A histogram counts how often values occur. Very often, though, you have *measured
points*: a voltage for each current, a period for each pendulum length, a temperature
at each hour. For these you use a **graph**. And to describe data with a formula, you
use a **function**. This chapter covers both.

Examples for this chapter are in `macros/ch04_graphs_functions`.

## 4.1 Functions: TF1

`TF1` is a function of one variable, x. You can make one in three ways:

{{code: macros/ch04_graphs_functions/functions.C}}

{{output: macros/ch04_graphs_functions/functions}}

![Three functions made in three different ways.](figures/functions.png)

**Way 1: a formula.** The formula is a string, written like C++. The variable is `x`,
and `[0]`, `[1]`, `[2]`, ... are **parameters**: numbers that you can change later, and
that a fit can adjust (Chapter 5). You can use all the normal mathematical functions
(`sin`, `exp`, `sqrt`, `pow(x, 3)`, ...) and `TMath` functions, for example
`"TMath::Gaus(x, [0], [1])"`. You can also give the parameters names, which is useful in
fits: `[A]*exp(-x/[tau])`.

**Way 2: a built-in function.** These names are ready to use:

| Name | Formula |
|------|---------|
| `gaus` | `[0]*exp(-0.5*((x-[1])/[2])^2)` — height, mean, sigma |
| `expo` | `exp([0] + [1]*x)` |
| `pol0`, `pol1`, ..., `pol9` | polynomials: `[0] + [1]*x + [2]*x^2 + ...` |
| `landau` | the Landau distribution — `[0]` height, `[1]` (close to the) peak position, `[2]` width |

You can add them up, with the number in brackets giving the index of their first
parameter: `"gaus(0) + pol1(3)"` is a Gaussian with parameters 0–2 plus a straight line
with parameters 3–4. We use this a lot in Chapter 5.

**Way 3: a C++ function.** When the function is too complicated for one line, write it
as a C++ function with the signature `double f(double *x, double *p)`. `x[0]` is the
variable and `p[0]`, `p[1]`, ... are the parameters. Then pass the function and the
number of parameters to `TF1`.

Things you can do with a `TF1`:

| Method | What it does |
|--------|--------------|
| `SetParameters(a, b, ...)`, `SetParameter(i, v)` | set parameter values |
| `SetParNames("A", "tau", ...)` | give the parameters names |
| `Eval(x)` | the value at x |
| `Integral(a, b)` | the integral from a to b |
| `Derivative(x)` | the derivative at x |
| `GetMaximumX()`, `GetMinimumX()` | where the maximum / minimum is |
| `GetX(y)` | the x where the function equals y |
| `SetNpx(n)` | draw with n points (default 100); increase it for fast-changing functions |
| `SetRange(a, b)` | change the x range |

## 4.2 Graphs: TGraph

A `TGraph` is a list of (x, y) points. The simplest way to make one is from two arrays:

{{code: macros/ch04_graphs_functions/graph_basic.C}}

{{output: macros/ch04_graphs_functions/graph_basic}}

![A TGraph drawn with points and lines (option "APL").](figures/graph_basic.png)

The draw options for graphs are different from those for histograms:

| Option | Meaning |
|--------|---------|
| `A` | draw the **axes** — needed for the first graph on a pad! |
| `P` | draw markers at the points |
| `L` | connect the points with straight lines |
| `C` | connect the points with a smooth curve |
| `*` | a star at each point |

So `"AP"` draws axes and markers, `"APL"` adds lines. To draw a second graph on the
same axes, leave out the `A`: `g2->Draw("P")`.

> **Common mistake:** `g->Draw("P")` on an empty canvas shows nothing, because no
> axes were drawn. The first graph needs `A`.

You can also add points one at a time, which is handy inside a loop:

```cpp
auto g = new TGraph();
for (int i = 0; i < 10; i++) {
   g->AddPoint(i, i * i);       // x, y
}
```

## 4.3 Graphs with error bars

Real measurements have uncertainties. `TGraphErrors` stores an error on x and on y for
every point. A very convenient feature: it can read the data straight from a text file.
The file `pendulum.txt` contains measurements of the period of a pendulum for different
lengths:

{{code: macros/ch04_graphs_functions/pendulum.txt}}

The four columns are x, y, error on x, error on y. Lines that start with `#` are
skipped, so you can use them for comments.

{{code: macros/ch04_graphs_functions/graph_errors.C}}

![Pendulum measurements with error bars, compared with the theoretical curve.](figures/graph_errors.png)

**Text on a plot.** `TLatex` writes text anywhere on the plot. With `SetNDC()` the
position is given in *normalised device coordinates* (fractions of the pad, from 0 to
1), so it does not depend on the axis ranges. Without `SetNDC()` the position is in the
units of the axes. `SetTextFont(42)` is ROOT's normal sans-serif font, and
`SetTextSize` is a fraction of the pad height.

Related classes:

- `TGraphAsymmErrors` — different errors up and down (or left and right);
- `TLine`, `TArrow`, `TBox`, `TEllipse` — draw shapes, for example
  `auto line = new TLine(x1, y1, x2, y2); line->Draw();`.

## 4.4 Several graphs together

`TMultiGraph` collects several graphs and draws them on common axes, choosing the axis
ranges so that all points fit:

{{code: macros/ch04_graphs_functions/multigraph.C}}

![Two graphs on the same axes with TMultiGraph.](figures/multigraph.png)

## 4.5 Histogram or graph?

| Use a **histogram** when... | Use a **graph** when... |
|---|---|
| you **count** how often values occur | you have **measured points** (x, y) |
| e.g. the energy of many particles | e.g. voltage against current |
| the errors come from counting (√N) | each point has its own measured error |

Both can be fitted with functions, which is the topic of the next chapter.

## Key points

- `new TF1("f", "formula with x and [0], [1]...", xmin, xmax)` makes a function;
  built-in names are `gaus`, `expo`, `polN`, `landau`.
- `new TGraph(n, x, y)` makes a graph from arrays; `new TGraphErrors("file.txt")` reads
  x, y, ex, ey from a text file.
- The first graph on a pad needs the `A` (axes) option: `Draw("AP")`.
- `TMultiGraph` draws several graphs together; `TLatex` with `SetNDC()` writes text.

## Exercises

**Exercise 4.1 — Trigonometry.** Draw sin(x), cos(x) and sin(x)·cos(x) for x from 0 to
2π on one canvas, in three different colours, with a legend. Then use `Integral` to
calculate the integral of sin²(x) from 0 to π. (The exact answer is π/2.)

**Exercise 4.2 — Ohm's law.** The file `ohm.txt` (in the same folder) contains
measurements of the voltage across a resistor for different currents:

{{code: macros/ch04_graphs_functions/ohm.txt}}

Make a `TGraphErrors` from the file and draw it with proper axis titles. (We fit it in
Exercise 5.1.)

Solutions are in Appendix A and in `solutions/ch04`.

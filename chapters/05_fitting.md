# 5 Fitting

Fitting means finding the values of a function's parameters that describe the data
best. It is how you measure things: the mass of a particle from the position of a peak,
a lifetime from an exponential decay, the gravitational acceleration from pendulum data.
ROOT's fitting is fast, reliable, and gives you the errors on the parameters.

Examples for this chapter are in `macros/ch05_fitting`.

## 5.1 What does a fit do?

For each data point (or histogram bin) *i* you have a measured value $y_i$ with error
$\sigma_i$, and the function predicts $f(x_i)$. The **least-squares** or **χ² fit** chooses the
parameters that make

$$\chi^2 = \sum_i \frac{\left(y_i - f(x_i)\right)^2}{\sigma_i^2}$$

as small as possible. ROOT uses the minimisation program **Minuit** for this.

After the fit, two numbers tell you whether the function describes the data well:

- **ndf**, the *number of degrees of freedom* = number of points (bins) − number of
  free parameters;
- **χ²/ndf** should be close to 1. Much larger than 1 means the function does not
  describe the data (or the errors are too small). Much smaller than 1 usually means the
  errors are too big.
- The **probability** (`Prob`) is the chance of getting a χ² this large or larger if
  the function were correct. Very small values (below about 0.01) are a warning sign.

## 5.2 Your first fit

{{code: macros/ch05_fitting/fit_gaus.C}}

![A Gaussian fit. The statistics box now also shows the fit results.](figures/fit_gaus.png)

`h->Fit("gaus")` does everything: it creates a Gaussian function, chooses sensible
starting values, fits, prints the result, and draws the fitted curve on the histogram.
The printout is:

{{output: macros/ch05_fitting/fit_gaus}}

How to read the Minuit printout:

| Line | Meaning |
|------|---------|
| `Minimizer is Minuit2 / Migrad` | the fitting program used |
| `Chi2`, `NDf` | χ² and the number of degrees of freedom: 52.9 / 43 = 1.23 — a good fit |
| `Edm` | *estimated distance to minimum*; very small means the fit converged |
| `NCalls` | how many times the function was evaluated |
| `Constant`, `Mean`, `Sigma` | the fitted parameters with their errors |

The word `(limited)` next to `Sigma` means that ROOT did not allow sigma to become
negative during the fit. You can ignore it.

**The result** is mean = 5.09 ± 0.05 and sigma = 1.54 ± 0.04. The data were generated
with mean 5.0 and sigma 1.5, so the fit found the true values within about 1–2 errors,
as it should.

`gStyle->SetOptFit(1111)` adds the fit results (χ²/ndf, probability, parameters) to the
statistics box. Each digit switches on one item: `SetOptFit(111)` leaves out the
probability.

## 5.3 Getting the results in your program

After `h->Fit("gaus")`, the fitted function is stored *inside* the histogram:
`h->GetFunction("gaus")` gives it back. Then:

| Method | What it gives |
|--------|---------------|
| `GetParameter(i)` | value of parameter i |
| `GetParError(i)` | error of parameter i |
| `GetChisquare()` | χ² |
| `GetNDF()` | number of degrees of freedom |
| `GetProb()` | fit probability |

## 5.4 A peak on top of a background

The most common fit in physics: a peak (the *signal*) sitting on top of a smooth
*background*. We describe it with the sum of two functions, `gaus(0) + expo(3)`:

{{code: macros/ch05_fitting/fit_peak_background.C}}

{{output: macros/ch05_fitting/fit_peak_background}}

![A Gaussian peak on an exponential background. The fit separates the signal (blue) from the background (green).](figures/fit_peak_background.png)

Important ideas in this example:

**Starting values.** With more than one function, ROOT cannot guess the starting values
for you. You must give reasonable ones with `SetParameters`. Look at the plot: the peak
is near 4 GeV, it is about 200 events above the background, and it is about 0.5 GeV
wide. Bad starting values are the most common reason for a fit to fail.

> **Tip:** Before fitting, draw the function with your starting values on top of the
> data: `fTotal->Draw("SAME")`. If it is not roughly in the right place, the fit will
> struggle.

**The fit result.** With the option `"S"`, `Fit` returns a `TFitResultPtr`, which holds
everything about the fit: `result->Chi2()`, `result->Ndf()`, `result->Parameter(i)`,
`result->ParError(i)`, `result->IsValid()`, and the **covariance matrix**
`result->GetCovarianceMatrix()`, which says how the parameter errors are correlated.
`result->Print("V")` prints all of it.

**Counting signal events.** The parameter `[0]` of `gaus` is the *height* of the peak,
not the number of events. The number of events is the area under the Gaussian divided
by the bin width:

$$N_\text{signal} = \frac{\int \text{gaus}\, dx}{\text{bin width}} = \frac{A\,\sigma\sqrt{2\pi}}{\text{bin width}}$$

Here we get 1993 ± 75 signal events — we generated 2000. The error is calculated by
`IntegralError`, using the part of the covariance matrix that belongs to the three
signal parameters.

## 5.5 Fitting a graph

Graphs are fitted in the same way. Here we use the pendulum data from Chapter 4 to
**measure g**. Since T = 2π√(L/g), the square T² = (4π²/g)·L is a straight line in L
with slope 4π²/g:

{{code: macros/ch05_fitting/fit_graph.C}}

{{output: macros/ch05_fitting/fit_graph}}

![Fitting a straight line to T² against L gives the gravitational acceleration.](figures/fit_graph.png)

We measure g = 9.75 ± 0.15 m/s², in agreement with the known 9.81 m/s². The intercept
`p0` is compatible with zero, as the physics says it should be. Note also how the error
on T² is calculated from the error on T (*error propagation*): σ(T²) = 2T·σ(T).

For a `TGraphErrors`, the fit uses the y errors and also takes the x errors into account.

## 5.6 Fit options

The second argument of `Fit` is a string of options. You can combine them, e.g. `"LRQ"`.

| Option | Meaning |
|--------|---------|
| `Q` | quiet: do not print the result |
| `S` | return a `TFitResultPtr` with all the results |
| `R` | fit only in the range of the function (as given when making the `TF1`) |
| `L` | use a (log-)likelihood fit instead of χ²; better for histograms with few entries |
| `0` | do not draw the fit result |
| `+` | keep previous fit functions attached to the histogram (otherwise they are replaced) |
| `E` | better error estimate with MINOS (slower) |
| `I` | use the integral of the function over each bin instead of its value at the bin centre |

To fit only part of a histogram you can also give the range directly:

```cpp
h->Fit("gaus", "", "", 3.0, 7.0);    // options, draw options, xmin, xmax
```

## 5.7 Few entries: use a likelihood fit

The χ² fit uses √N as the error of a bin with N entries. When N is small, this is a bad
approximation, and for empty bins (N = 0) it does not work at all — ROOT simply ignores
them. The result is a biased fit. The **likelihood fit** (option `"L"`) uses the correct
Poisson probabilities and works also for empty bins.

{{code: macros/ch05_fitting/fit_likelihood.C}}

{{output: macros/ch05_fitting/fit_likelihood}}

![With only 60 entries, the χ² fit (red) is clearly wrong; the likelihood fit (blue) is good.](figures/fit_likelihood.png)

The χ² fit gives a Gaussian that is too wide and too low; the likelihood fit gets close
to the true values, and its area equals the number of entries exactly.

> **Rule of thumb:** if many bins have fewer than about 10 entries, use the `"L"` option.

## 5.8 When a fit fails

If the fitted curve is nowhere near the data, or ROOT prints warnings such as
`FAILED` or `NOT POSDEF`, try these steps:

1. **Better starting values.** Draw the starting function on the data (Section 5.4).
2. **Fit a smaller range** with `"R"` or the xmin/xmax arguments, around the region you
   care about.
3. **Limit parameters** to sensible ranges: `f->SetParLimits(2, 0.01, 1.0);` keeps
   parameter 2 between 0.01 and 1.
4. **Fix parameters** you know: `f->FixParameter(1, 3.0969);`
5. **Fit in steps**: first fit only the background in a range without the peak, then use
   those values as starting values for the full fit.
6. **Check the result** in your code: `if (!result->IsValid()) { ... }`.

> **The Fit Panel.** For experimenting, right-click on a histogram in a canvas and choose
> *FitPanel*. You can choose the function, range and options with the mouse and see the
> result at once.

## Key points

- `h->Fit("gaus")` fits and draws; `h->GetFunction("gaus")` gives the result back.
- Check χ²/ndf ≈ 1 and that the parameter errors look reasonable.
- For combined functions like `gaus(0) + expo(3)`, always give starting values.
- The number of signal events is the integral of the signal function divided by the bin width.
- Use the `"L"` option for histograms with few entries per bin.

## Exercises

**Exercise 5.1 — The resistance.** Fit the Ohm's-law graph from Exercise 4.2 with a
straight line (`pol1`). What is the resistance and its error? (The voltage is in V and
the current in mA, so the slope is in kΩ.)

**Exercise 5.2 — The muon lifetime.** Muons decay with a lifetime of τ = 2.197 μs.
Generate 5000 decay times with `rng.Exp(2.197)` and fill them in a histogram with 40
bins from 0 to 10 μs. Fit with `expo`. The fitted function is exp(p0 + p1·t), so the
lifetime is τ = −1/p1. Calculate τ and its error (error propagation:
σ(τ) = τ·σ(p1)/|p1|). Is your result compatible with 2.197 μs? Draw the histogram with a
logarithmic y axis: what does the exponential look like?

Solutions are in Appendix A and in `solutions/ch05`.

# Appendix A: Solutions to the exercises

Try the exercises yourself before you look here! There is always more than one correct
solution; these are just examples. All files are in the folder `solutions`, one
sub-folder per chapter, and they have been run to make the outputs shown here.

## A.2 First steps

**Exercise 2.1 — A table.**

{{code: solutions/ch02/table.C}}

{{output: solutions/ch02/table}}

**Exercise 2.2 — Your own function plot.**

{{code: solutions/ch02/damped.C}}

![Solution of Exercise 2.2.](figures/damped.png)

`SetNpx(300)` makes the curve smoother: with the default of 100 points over 15 units, the
peaks look slightly angular. Zoom into `damped.pdf` and `damped.png`: the PDF stays sharp,
the PNG becomes blocky.

## A.3 Histograms

**Exercise 3.1 — The central limit theorem.**

{{code: solutions/ch03/clt.C}}

![The sum of N uniform random numbers for N = 1, 2, 3 and 12, with Gaussian fits.](figures/clt.png)

One uniform number gives a flat distribution, the sum of two gives a triangle, the sum
of three is already round, and the sum of twelve is almost exactly a Gaussian with mean
6 and standard deviation 1. This is the *central limit theorem*: sums of many random
numbers are Gaussian, whatever the distribution of the individual numbers. It is why
Gaussians appear everywhere in physics. `Form(...)` creates a text from a pattern, like
`printf`, which is handy for making names like `hSum12`.

**Exercise 3.2 — Calculating π.**

{{code: solutions/ch03/pi_mc.C}}

{{output: solutions/ch03/pi_mc}}

![Left: random points inside (blue) and outside (red) the circle. Right: x² + y².](figures/pi_mc.png)

The histogram of x² + y² is flat up to 1: inside the circle, equal steps in r² cover
equal areas. Above 1, only the corners of the square are left, which get smaller and
smaller until x² + y² = 2. The error on π gets 10 times smaller when you use 100 times
more points.

## A.4 Graphs and functions

**Exercise 4.1 — Trigonometry.**

{{code: solutions/ch04/trig.C}}

{{output: solutions/ch04/trig}}

![Solution of Exercise 4.1.](figures/trig.png)

**Exercise 4.2 — Ohm's law.**

{{code: solutions/ch04/ohm.C}}

![Solution of Exercise 4.2.](figures/ohm.png)

## A.5 Fitting

**Exercise 5.1 — The resistance.**

{{code: solutions/ch05/ohm_fit.C}}

{{output: solutions/ch05/ohm_fit}}

![A straight-line fit gives the resistance.](figures/ohm_fit.png)

The resistance is R = 2.195 ± 0.013 kΩ, and the intercept is compatible with zero, as it
should be for a resistor. χ²/ndf = 1.09/8 is small, which suggests that the errors of
0.05 V in the file are a bit larger than the real scatter of the points.

**Exercise 5.2 — The muon lifetime.**

{{code: solutions/ch05/muon_lifetime.C}}

{{output: solutions/ch05/muon_lifetime}}

![The decay-time distribution on a logarithmic scale: an exponential becomes a straight line.](figures/muon_lifetime.png)

The result, 2.202 ± 0.036 μs, agrees with 2.197 μs. If you use a different random seed,
you get a slightly different value each time; about two times out of three it is within
one error (0.036 μs) of the true value. On a logarithmic y axis an exponential is a
straight line, which makes it easy to see whether the data follow an exponential.

## A.6 ROOT files

**Exercise 6.1 — Save and read back.**

{{code: solutions/ch06/clt_write_read.C}}

{{output: solutions/ch06/clt_write_read}}

![The histogram read back from `clt.root`.](figures/clt_read.png)

Note that the folder `sums` is shown by `ls()`, but not its contents. Use
`rootls -l clt.root:sums` to look inside it.

## A.7 Trees

**Exercise 7.1 — Compare selections.**

{{code: solutions/ch07/tree_compare.C}}

![Energy distribution of all events, and of events with more than 20 hits.](figures/tree_compare.png)

The number of hits grows with the energy (on average one hit per 2 GeV). To have more
than 20 hits, an event needs an energy of roughly 20 GeV or more, so the selected events
have no low energies. A cut on one variable changes the distribution of other, correlated
variables — something to keep in mind in every analysis.

**Exercise 7.2 — Count in a loop.**

{{code: solutions/ch07/tree_count.C}}

{{output: solutions/ch07/tree_count}}

Both methods agree. The one-line `GetEntries` is perfect for checks like this.

## A.8 PyROOT

**Exercise 8.1 — The muon lifetime in Python.**

{{code: solutions/ch08/muon_lifetime.py}}

{{output: solutions/ch08/muon_lifetime}}

![The same fit in Python, with data from NumPy.](figures/muon_lifetime_py.png)

## A.9 RDataFrame

**Exercise 9.1 — A small analysis.**

{{code: solutions/ch09/rdf_exercise.py}}

{{output: solutions/ch09/rdf_exercise}}

![The distance r of the selected events.](figures/rdf_exercise.png)

Almost every event above 30 GeV also has 10 or more hits, so the second cut removes
very few events (99.5% pass). The report calls this the efficiency of the cut.

## A.10 Mini-project: worked solution (spoiler!)

### The analysis

Here is one complete solution of Steps 2–8, in Python. It is also in
`macros/ch10_project/dimuon_analysis.py`.

{{code: macros/ch10_project/dimuon_analysis.py}}

{{output: macros/ch10_project/dimuon_analysis}}

![Step 4: the invariant mass of opposite-charge muon pairs. There is a clear peak near 3.1 GeV, and a smaller one near 3.7 GeV.](figures/dimuon_overview.png)

![Steps 5 and 7: the fit of the peak and the final plot.](figures/dimuon_fit.png)

**The answer.** The peak is at 3096.1 ± 0.8 MeV. The Particle Data Group lists the
**J/ψ meson** (a bound state of a charm quark and its antiquark) at 3096.9 MeV, and it
does decay to μ⁺μ⁻. Our result agrees within one error. The J/ψ was discovered in 1974,
in two experiments at the same time, in exactly this kind of plot — the discovery is
known as the *November Revolution* of particle physics.

The width of 33.5 MeV is not the natural width of the J/ψ (which is only 0.09 MeV): it
is the resolution of our (toy) detector, which measures the muon momenta with an
uncertainty of 1.5%.

### Extra task A: same-charge pairs

{{code: macros/ch10_project/dimuon_like_sign.py}}

{{output: macros/ch10_project/dimuon_like_sign}}

![Left: opposite-charge and same-charge pairs. Right: their difference.](figures/dimuon_like_sign.png)

The same-charge pairs have no peak: they come only from random combinations of
unrelated muons. Subtracting them removes this *combinatorial* background. What is left
is the J/ψ peak plus a smooth spectrum of opposite-charge pairs from other processes
(in our toy data: a "continuum" of correlated pairs).

### Extra task B: the second particle

{{code: solutions/ch10/psi2s_fit.py}}

{{output: solutions/ch10/psi2s_fit}}

![Fit of both peaks: the J/ψ and the ψ(2S).](figures/psi2s_fit.png)

The second peak, at 3661 ± 12 MeV, is the **ψ(2S)** (also called ψ′), an excited state of
the J/ψ, at 3686.1 MeV in the PDG tables. With only about 300 signal events on a large
background, our measurement is much less precise, and it is two errors away from the
true value — this happens about one time in twenty. The ratio of the two peaks has a
large uncertainty (about 30%) for the same reason.

### What is inside the toy data

The generator `make_dimuon_toy.C` hides two particles in the data: the J/ψ (3.0969 GeV,
in 3% of the events) and the ψ(2S) (3.6861 GeV, in 0.15% of the events). They are
produced with a realistic spread of momenta and decay into two muons. The rest of the
events contain either a pair of muons with a smoothly falling mass distribution, or two
unrelated muons. The detector only sees muons with $p_\mathrm{T}$ > 1 GeV and |η| < 2.4, and
measures $p_\mathrm{T}$ with 1.5% resolution.

{{code: macros/ch10_project/make_dimuon_toy.C}}

### Extra task D: real CMS data

{{code: macros/ch10_project/cms_open_data.py}}

{{output: macros/ch10_project/cms_open_data}}

![The dimuon mass spectrum from real CMS data (first 2 million of 61 million events), on logarithmic axes.](figures/cms_dimuon_spectrum.png)

With the same selection as for the toy data, the real data show a whole century of
particle physics in one plot: the η (548 MeV), the ρ and ω (775 and 783 MeV), the φ
(1019 MeV), the J/ψ and ψ(2S), the three Υ states (9.46, 10.02 and 10.36 GeV; they
contain a bottom quark and antiquark), and the Z boson (91.2 GeV). The broad bump around
30 GeV is *not* a particle: it is caused by the way CMS selected (triggered) these events.

Logarithmic bins are used (`numpy.logspace`), so that every peak gets enough bins on the
logarithmic x axis.

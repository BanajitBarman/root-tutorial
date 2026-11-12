# 10 Mini-project: find the hidden particle

Time to put everything together. You get a dataset of collision events with muons. Some
of the events contain an unknown particle that decays into a muon and an antimuon. Your
job: **find the particle, measure its mass, and identify it.** This is exactly how many
particles were discovered.

Work in teams of two or three. The files are in `macros/ch10_project`.

## 10.1 The data

First create the dataset (it takes a few seconds):

```bash
cd root-tutorial/macros/ch10_project
root -l -q make_dimuon_toy.C
```

This writes `dimuon_toy.root` with a tree called `Events`. Each entry is one collision
event with one or more muons. The branches are:

| Branch | Type | Meaning |
|--------|------|---------|
| `nMuon` | integer | number of muons in the event |
| `Muon_pt` | array of float | transverse momentum $p_\mathrm{T}$ of each muon (GeV) |
| `Muon_eta` | array of float | pseudorapidity η of each muon |
| `Muon_phi` | array of float | azimuthal angle φ of each muon (radians) |
| `Muon_mass` | array of float | muon mass (0.1057 GeV) |
| `Muon_charge` | array of int | charge of each muon, +1 or −1 |

So `Muon_pt[0]` is the transverse momentum of the first muon in the event, and
`Muon_charge[1]` is the charge of the second one.

> **The coordinates of particle physics.** The beams travel along the z axis.
> **$p_\mathrm{T}$** is the momentum perpendicular to the beam. **φ** is the angle around the beam
> (from −π to π). **η** (pseudorapidity) describes the angle θ to the beam:
> η = −ln tan(θ/2). A particle at 90° to the beam has η = 0; large |η| means close to
> the beam. Together, ($p_\mathrm{T}$, η, φ, m) fully describe a particle's four-momentum.

## 10.2 The physics: invariant mass

When a particle X decays into two muons, X → μ⁺μ⁻, energy and momentum are conserved.
So the **invariant mass** of the two muons,

$$m_{\mu\mu}^2 = (E_1 + E_2)^2 - |\vec p_1 + \vec p_2|^2 ,$$

is equal to the mass of X, whatever direction or speed X had. If you compute $m_{\mu\mu}$ for
many events and fill a histogram:

- muon pairs that come from X all give the **same** mass: they form a **peak** at the
  mass of X (as wide as the detector resolution);
- muon pairs that do not come from a decay (random pairs, other processes) give a
  **smooth background**.

You do not need to code the formula yourself: RDataFrame has a function
`ROOT::VecOps::InvariantMass(pt, eta, phi, mass)` that computes the invariant mass of all
particles in the arrays.

## 10.3 Your tasks

Use RDataFrame, in Python or C++, as you prefer. Start a new file, e.g.
`my_analysis.py`, and build it up step by step, checking the output at each step.

**Step 1 — Look at the data.** How many events are there? Use `rootls -t dimuon_toy.root`
and `df.Display([...], 5).Print()` to look at a few events. Make histograms of `nMuon`
and of `Muon_pt`. (*Hint:* `Histo1D` of an array column fills every element.)

**Step 2 — Select muon pairs.** Keep only events with **exactly two muons**, and then
only those where the two muons have **opposite charge**. Use named filters, and print
the cut-flow report at the end.

**Step 3 — Compute the invariant mass** with `Define` and `InvariantMass`.

**Step 4 — Look for a peak.** Make a histogram of the mass from 0 to 10 GeV with 40 MeV
bins (how many bins is that?). Can you see a peak? Where? Is there more than one?

**Step 5 — Fit the peak.** Make a second histogram that zooms into the region around the
biggest peak, with finer bins. Fit it with a Gaussian plus a background function
(`"gaus(0) + expo(3)"` or `"gaus(0) + pol2(3)"`), with sensible starting values and the
`"R"` option. Find:

- the **mass** of the particle (the mean of the Gaussian), with its error;
- the **width** (sigma) — this is the detector resolution, not the true width of the particle;
- the **number of signal events** (area of the Gaussian divided by the bin width).

**Step 6 — Identify the particle.** Compare your mass with the list of particles of the
Particle Data Group (<https://pdg.lbl.gov>, *Summary Tables*). Which known particle has
this mass and decays into two muons?

**Step 7 — Make a final plot** good enough for a talk:

- data with error bars, the fit curve, and the background part of the fit drawn separately;
- axis titles with units, e.g. `m_{#mu#mu} (GeV)` and `Events / 10 MeV`;
- a legend, and your results (mass, width, signal) written on the plot with `TLatex`;
- saved as **PDF** and PNG.

**Step 8 — Save your work** — histograms and fit function — in a ROOT file.

> **Hints.** Each step uses something from earlier chapters:
>
> - Filters: `df.Filter("nMuon == 2", "Exactly two muons")` (Chapter 9)
> - Charges: `"Muon_charge[0] != Muon_charge[1]"`
> - Mass: `df.Define("Dimuon_mass", "ROOT::VecOps::InvariantMass(Muon_pt, Muon_eta, Muon_phi, Muon_mass)")`
> - Fitting a histogram from RDataFrame: get the real histogram first with
>   `h = result.GetValue()`, then `h.Fit(...)` (Chapter 5)
> - Number of bins and bin width: `h.GetNbinsX()`, `h.GetBinWidth(1)`
> - Peak position for a starting value: `h.GetBinCenter(h.GetMaximumBin())`

## 10.4 Extra tasks for fast teams

**A. Same-charge pairs.** Make the mass histogram also for pairs with the **same** charge
(++ or −−), and draw it on top of the opposite-charge one. A real particle decays to
μ⁺μ⁻, never to μ⁺μ⁺. So what do the same-charge pairs tell you? Subtract the same-charge
histogram from the opposite-charge one and look at the result. (This *like-sign
subtraction* is a standard technique, for example in heavy-ion experiments.)

**B. A second particle.** Is there a second, smaller peak? Fit both peaks together with
`"gaus(0) + gaus(3) + expo(6)"`. What is the second particle? What is the ratio of the
number of events in the two peaks?

**C. Optimise the cuts.** Muons from the particle tend to have a higher $p_\mathrm{T}$ than the
random background muons. Add a cut that requires both muons to have $p_\mathrm{T}$ above 1.5, 2,
2.5 GeV. For each cut, fit again and compute the signal S and background B within ±3σ
of the peak, the ratio S/B, and the *significance* S/√(S+B). Which cut gives the
cleanest peak? Which gives the most significant one? Are these the same?

**D. Real data!** The branch names in our toy data are the same as in a real dataset
published by the CMS experiment at CERN: 61 million events from 2012. Run your analysis
on it — only the file name changes! The script `cms_open_data.py` shows how. It needs an
internet connection and reads the first 2 million events in about two minutes:

```bash
python cms_open_data.py
```

Look at the result: how many particles can you recognise?

## 10.5 Presenting your result

At the end of Day 2 each team shows its final plot and says in two minutes:

1. what selection you used and how many events passed;
2. the mass, width and number of signal events from your fit, with errors;
3. which particle you think it is, and why;
4. one thing that surprised you or that you would do better with more time.

A good physics plot has: axis titles with units, a legend, readable text sizes, no
unnecessary decoration, and the important numbers on it.

> **Spoiler warning.** A complete worked solution, the results, and the plot from the
> real CMS data are in Appendix A, Section A.10. Try the project yourself first!

## Key points

- An invariant-mass peak reveals a particle; its position is the mass.
- A typical analysis: select events → compute a quantity → histogram → fit → compare
  with known values → make a clear plot.
- Everything you learned in this tutorial was needed for this project — well done!

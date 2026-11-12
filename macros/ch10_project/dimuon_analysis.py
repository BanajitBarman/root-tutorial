# dimuon_analysis.py -- mini-project solution: find the hidden particle
# Run with:   python dimuon_analysis.py     (run make_dimuon_toy.C first)
#
# Steps: select events with two muons of opposite charge, compute their
# invariant mass, histogram it, fit the peak, report mass / width / yield,
# and make a clean final plot.
import ROOT

ROOT.gStyle.SetOptStat(0)

# ---------------------------------------------------------------- 1. Data
df = ROOT.RDataFrame("Events", "dimuon_toy.root")

# ---------------------------------------------------------------- 2. Selection
df_2mu = df.Filter("nMuon == 2", "Exactly two muons")
df_os = df_2mu.Filter("Muon_charge[0] != Muon_charge[1]", "Opposite charge")

# Invariant mass of the two muons (a ready-made RVec helper function)
df_mass = df_os.Define("Dimuon_mass",
                       "ROOT::VecOps::InvariantMass(Muon_pt, Muon_eta, Muon_phi, Muon_mass)")

# ---------------------------------------------------------------- 3. Histograms
h_full = df_mass.Histo1D(("h_full", "Opposite-charge muon pairs;m_{#mu#mu} (GeV);Events / 40 MeV",
                          250, 0.0, 10.0), "Dimuon_mass")
h_zoom = df_mass.Histo1D(("h_zoom", ";m_{#mu#mu} (GeV);Events / 10 MeV",
                          100, 2.6, 3.6), "Dimuon_mass")
report = df.Report()

# Overview plot: where is the peak?
c1 = ROOT.TCanvas("c1", "Overview", 800, 600)
h_full.SetLineColor(ROOT.kBlue + 1)
h_full.Draw("HIST")
c1.SaveAs("dimuon_overview.png")

# ---------------------------------------------------------------- 4. Fit
h = h_zoom.GetValue()            # the real TH1D behind the booked result
peak_guess = h.GetBinCenter(h.GetMaximumBin())

model = ROOT.TF1("model", "gaus(0) + expo(3)", 2.6, 3.6)
model.SetParNames("Height", "Mass", "Width", "BkgConst", "BkgSlope")
model.SetParameters(h.GetMaximum(), peak_guess, 0.03, 6.0, -0.5)
model.SetNpx(500)
model.SetLineColor(ROOT.kRed)
model.SetLineWidth(3)
result = h.Fit(model, "S R 0")   # S = keep result, R = function range, 0 = draw later

mass, mass_err = model.GetParameter(1), model.GetParError(1)
width, width_err = abs(model.GetParameter(2)), model.GetParError(2)

# Signal and background parts of the model
sig = ROOT.TF1("sig", "gaus", 2.6, 3.6)
sig.SetParameters(model.GetParameter(0), mass, width)
bkg = ROOT.TF1("bkg", "expo", 2.6, 3.6)
bkg.SetParameters(model.GetParameter(3), model.GetParameter(4))
bkg.SetLineColor(ROOT.kGreen + 2)
bkg.SetLineStyle(2)
bkg.SetLineWidth(3)

# Counting: events within +- 3 sigma of the peak
bw = h.GetBinWidth(1)
lo, hi = mass - 3 * width, mass + 3 * width
S = sig.Integral(lo, hi) / bw
B = bkg.Integral(lo, hi) / bw
n_signal = sig.Integral(2.6, 3.6) / bw
significance = S / (S + B) ** 0.5

# ---------------------------------------------------------------- 5. Results
print("\n================ RESULTS ================")
print(f"Mass        = {1000*mass:.1f} +- {1000*mass_err:.1f} MeV")
print(f"Width       = {1000*width:.1f} +- {1000*width_err:.1f} MeV (detector resolution)")
print(f"Signal      = {n_signal:.0f} events")
print(f"In +-3 sigma: S = {S:.0f}, B = {B:.0f}, S/B = {S/B:.2f}")
print(f"Significance S/sqrt(S+B) = {significance:.1f}")
print(f"chi2/ndf    = {result.Chi2():.1f} / {result.Ndf()}")
print("PDG: J/psi mass = 3096.9 MeV")
print("=========================================\n")
report.Print()

# ---------------------------------------------------------------- 6. Final plot
c2 = ROOT.TCanvas("c2", "Final plot", 800, 600)
h.SetMarkerStyle(20)
h.SetMarkerSize(0.8)
h.SetMaximum(1.35 * h.GetMaximum())
h.Draw("E")
bkg.Draw("SAME")
model.Draw("SAME")

leg = ROOT.TLegend(0.60, 0.70, 0.88, 0.86)
leg.SetBorderSize(0)
leg.AddEntry(h, "Toy data", "pe")
leg.AddEntry(model, "Gaussian + exponential fit", "l")
leg.AddEntry(bkg, "Background part", "l")
leg.Draw()

text = ROOT.TLatex()
text.SetNDC()
text.SetTextFont(42)
text.SetTextSize(0.037)
text.DrawLatex(0.15, 0.84, "#bf{ROOT tutorial} mini-project")
text.DrawLatex(0.15, 0.78, f"m = {1000*mass:.1f} #pm {1000*mass_err:.1f} MeV")
text.DrawLatex(0.15, 0.73, f"#sigma = {1000*width:.1f} #pm {1000*width_err:.1f} MeV")
text.DrawLatex(0.15, 0.68, f"N_{{signal}} = {n_signal:.0f}")
c2.SaveAs("dimuon_fit.png")

# ---------------------------------------------------------------- 7. Save
out = ROOT.TFile("dimuon_results.root", "RECREATE")
h_full.Write()
h.Write()
model.Write()
out.Close()
print("Saved dimuon_overview.png, dimuon_fit.png and dimuon_results.root")

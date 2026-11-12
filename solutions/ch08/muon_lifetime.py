# Solution to Exercise 8.1 -- the muon lifetime fit in Python, data made with NumPy
import ROOT
import numpy as np

ROOT.gStyle.SetOptFit(111)
times = np.random.default_rng(4).exponential(2.197, size=5000)   # microseconds

h = ROOT.TH1D("h", "Muon decay times (Python);t (#mus);Decays / 0.25 #mus", 40, 0, 10)
h.Fill(times)

c = ROOT.TCanvas("c", "Muon lifetime", 800, 600)
c.SetLogy()
h.Fit("expo", "L")
h.SetMarkerStyle(20)
h.Draw("E")
c.SaveAs("muon_lifetime_py.png")

f = h.GetFunction("expo")
tau = -1 / f.GetParameter(1)
tau_err = tau * f.GetParError(1) / abs(f.GetParameter(1))
print(f"Lifetime = {tau:.3f} +- {tau_err:.3f} us   (true value 2.197 us)")

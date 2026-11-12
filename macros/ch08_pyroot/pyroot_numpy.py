# pyroot_numpy.py -- moving numbers between NumPy and ROOT
# Run with:   python pyroot_numpy.py
import ROOT
import numpy as np

rng = np.random.default_rng(1)

# NumPy array -> ROOT histogram (Fill accepts a whole array)
x = rng.exponential(2.0, size=100000)
h = ROOT.TH1D("h", "Exponential (filled from NumPy);t (#mus);Events", 40, 0, 10)
h.Fill(x)
print("Entries:", h.GetEntries())

# ROOT histogram -> NumPy array of bin contents
counts = h.values()
print("First five bins:", counts[:5])
print("Type:", type(counts))

# Fit with an exponential: expo = exp([0] + [1]*t), so lifetime = -1/[1]
c = ROOT.TCanvas("c", "NumPy", 800, 600)
c.SetLogy()
h.Fit("expo", "Q")          # Q = quiet
h.Draw("E")
c.SaveAs("pyroot_numpy.png")
slope = h.GetFunction("expo").GetParameter(1)
print(f"Fitted lifetime = {-1/slope:.3f}  (generated 2.0)")
